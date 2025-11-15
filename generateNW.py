"""
Auto-generate NDS Watcher definitions from MKDS headers using libclang.
Arguments will be passed to libclang.
"""

from pathlib import Path
import os
import sys
import re
import subprocess
from collections import defaultdict
from clang.cindex import Config, Index, CursorKind, TypeKind

# -----------------------
# 1) Configurable paths
# -----------------------
LLVM_LIB = os.environ.get("LIBCLANG_PATH")
if LLVM_LIB == None:
	clang_path = Path(str(subprocess.check_output('which clang', shell=True), 'utf-8').strip())
	actual_clang = (clang_path.parent / clang_path.readlink()).resolve()
	LLVM_LIB = str(actual_clang.parent.parent / 'lib/libclang.so')
assert Path(LLVM_LIB).exists(), f"could not find LLVM at {LLVM_LIB}"

DEVKITPRO_ROOT = os.environ.get("DEVKITPRO", "/opt/devkitpro")
PROJECT_ROOT = os.environ.get('NW_SOURCE', "mkds")
OUT_DIR = Path(os.environ.get('NW_OUT', "out"))

# -----------------------
# Helpers
# -----------------------
def sanitize_name(name: str) -> str:
	"""Make a valid identifier. Probably not necessary.
	It would matter for anonymous/unnamed types, but we do something else with those."""
	if not name:
		raise Exception("anon")
	ident = re.sub(r"\W", "_", name)
	if ident[0].isdigit():
		ident = "_" + ident
	return ident

project_path_abs = str(Path(PROJECT_ROOT).resolve()) + '/'
project_path_rel = PROJECT_ROOT + '/'
def is_project_file(path: str) -> bool:
	# Using pathlib to more properly check paths is very slow (since we'll be doing this many thousands of times)
	# This works.
	return path.startswith(project_path_rel) or path.startswith(project_path_abs)

def is_in_project_file(cursor) -> bool:
	loc = cursor.location
	if bool(loc.file):
		fname = str(loc.file.name)
		return is_project_file(fname)
	return False

primitive_types_map = {
	"signed char": "s8",
	"char": "s8",
	"unsigned char": "u8",
	"signed": "s32",
	"unsigned": "u32",
	"short": "s16",
	"signed short": "s16",
	"unsigned short": "u16",
	"int": "s32",
	"signed int": "s32",
	"unsigned int": "u32",
	"long": "s32",
	"unsigned long": "u32",
	"long long": "s64",
	"unsigned long long": "u64",
	"float": "float",
	"double": "double",
}
watcher_primitives = set(['s8', 'u8', 's16', 'u16', 's32', 'u32', 's64', 'u64', 'void', 'fx32', 'fx16', 'code'])
def c_primitive_map(base: str) -> str | None:
	base = base.replace('const ', '').replace('volatile ', '').replace('restrict ', '')
	return primitive_types_map.get(base)

def is_c_primitive(type_name: str):
	type_name = type_name.strip('* ')
	return c_primitive_map(type_name) is not None

class DependencySet():
	"""Tracks which types need to be defined for NDS Watcher and which already have been."""
	def __init__(self):
		self._unfulfilled = {}
		self._fulfilled = set()

	def get_unfulfilled(self):
		for k in self._unfulfilled:
			return self._unfulfilled[k]
		return None

	def add(self, type_node):
		type_node = unwrap_dependency(type_node)
		name = get_friendly_name(type_node)
		if name in watcher_primitives:
			return
		if name not in self._unfulfilled and name not in self._fulfilled:
			self._unfulfilled[name] = type_node

	def set_fulfilled(self, name):
		self._fulfilled.add(name)
		self._unfulfilled.pop(name, None)

	def replace(self, type_node):
		type_node = unwrap_dependency(type_node)
		name = get_friendly_name(type_node)
		if name in self._fulfilled:
			raise Exception(f'Dependency {name} was already fulfilled.')
		self._unfulfilled[name] = type_node

	def has_unfulfilled(self):
		return len(self._unfulfilled) > 0

# -----------------------
# Globals
# -----------------------
typedefs_by_header: dict[str, list['TypedefInfo']] = defaultdict(list)
records_by_header: dict[str, list[tuple[str,list[tuple[str,str]],bool]]] = defaultdict(list)  # header -> [(name, fields, is_union)]
enums_by_header: dict[str, list[tuple[str, list[tuple[str,int]]]]] = defaultdict(list)  # header -> [(enum_name, [(const, val)])]
usr_to_record: dict[str, any] = {}

# -----------------------
# Clang AST helpers
# -----------------------
def _unwrap_elaborated(t):
	while t.kind == TypeKind.ELABORATED:
		t = t.get_named_type()
	return t

def get_final_type(t):
	t = _unwrap_elaborated(t)
	while t.kind == TypeKind.TYPEDEF:
		t = _unwrap_elaborated(t.get_canonical())
	return t

def get_base_type(t):
	"""If t is a pointer or array, returns the type it points to. (if that is a pointer/array, then its base type)"""
	t = get_final_type(t) # Need to unwrap the typedefs so that we can see the other type kinds.
	b = None
	if t.kind == TypeKind.POINTER:
		b = t.get_pointee()
	if t.kind == TypeKind.CONSTANTARRAY:
		b = t.get_array_element_type()
	if t.kind == TypeKind.INCOMPLETEARRAY:
		b = t.get_array_element_type()

	if b is None:
		return t
	else:
		return get_base_type(b)

path_map = {}
def get_full_path(path):
	mapped = path_map.get(path)
	if mapped is None:
		mapped = str(Path(path).resolve()) # stats file; slow
		path_map[path] = mapped
	return mapped
def get_node_file(node):
	return get_full_path(str(node.location.file))

def unwrap_dependency(dtype):
	while dtype.kind in (TypeKind.CONSTANTARRAY, TypeKind.INCOMPLETEARRAY, TypeKind.POINTER, TypeKind.ELABORATED):
		dtype = _unwrap_elaborated(dtype)
		if dtype.kind in (TypeKind.CONSTANTARRAY, TypeKind.INCOMPLETEARRAY):
			dtype = dtype.get_array_element_type()
		elif dtype.kind == TypeKind.POINTER:
			dtype = dtype.get_pointee()
	return dtype

def get_friendly_name(type, node = None) -> str:
	if node is None:
		node = type.get_declaration()
	
	type = _unwrap_elaborated(type)

	# Primitive?
	base = type.spelling
	mapped = c_primitive_map(base)
	if mapped:
		return mapped

	if type.kind == TypeKind.TYPEDEF:
		return sanitize_name(type.spelling)

	# Record
	if type.kind in (TypeKind.RECORD, TypeKind.ENUM):
		tag: str = type.spelling
		tag = tag.replace('struct ', '').replace('union ', '').replace('enum ', '')
		tag = tag.replace('const ', '').replace('volatile ', '').replace('restrict ', '')
		tag = tag.strip()
		while ' *' in tag:
			tag = tag.replace(' *', '*')

		return sanitize_name(tag)
	
	if type.kind == TypeKind.POINTER:
		ptee = type.get_pointee()
		if ptee.kind in (TypeKind.VOID, TypeKind.INVALID):
			return "void*"
		return f"{get_friendly_name(ptee, node)}*"
	if type.kind == TypeKind.CONSTANTARRAY:
		ptee = type.get_array_element_type()
		return f"{get_friendly_name(ptee, node)}[{type.get_array_size()}]"
	if type.kind == TypeKind.INCOMPLETEARRAY:
		ptee = type.get_array_element_type()
		return f"{get_friendly_name(ptee, node)}[]"

	if type.kind in (TypeKind.FUNCTIONPROTO, TypeKind.FUNCTIONNOPROTO):
		return 'code'
	
	if type.kind == TypeKind.VOID:
		return 'void'

	raise Exception(f'unknown type {base} at {node.location} and is {type.kind}')

def field_is_anonymous(field_node):
	# The build-in is_anonymous function actually checks if the type is anonymous.
	return 'anonymous at ' in field_node.spelling or field_node.spelling == ''

class TypedefInfo():
	renamed_type: any
	new_name: str
	old_name: str

	def __init__(self, renamed_type, new_name, old_name):
		self.renamed_type = renamed_type
		self.new_name = new_name
		self.old_name = old_name

def get_typedef_info(typedef):
	name = get_friendly_name(typedef.type)
	underlying = typedef.underlying_typedef_type
	underlying_name = get_friendly_name(underlying)

	if name != underlying_name and name not in watcher_primitives:
		return TypedefInfo(underlying, sanitize_name(name), underlying_name)

	return TypedefInfo(underlying, '', '')

# -----------------------
# Collection
# -----------------------
seen_usrs: set[str] = set()
def collect(node):
	usr = node.get_usr() # Unified Symbol Resolution
	if usr in seen_usrs:
		return
	elif node.kind in (CursorKind.STRUCT_DECL, CursorKind.UNION_DECL, CursorKind.ENUM_DECL) and not node.is_definition():
		# Ignore forward declaractions. They have the same USR as the definition, but may be in a different file.
		return
	elif len(usr) != 0:
		seen_usrs.add(usr)

	get_children = True
	project_file = is_in_project_file(node)
	if node.kind == CursorKind.TYPEDEF_DECL:
		get_children = False
		if project_file:
			tdef_info = get_typedef_info(node)
			if tdef_info.new_name != '':
				header_file = get_node_file(node)
				typedefs_by_header[header_file].append(tdef_info)
	elif node.kind == CursorKind.ENUM_DECL:
		get_children = False
		if project_file:
			header_file = get_node_file(node)
			enums_by_header[header_file].append(node.type)
	elif node.kind in (CursorKind.STRUCT_DECL, CursorKind.UNION_DECL):
		get_children = False
		if project_file:
			header_file = get_node_file(node)
			records_by_header[header_file].append(node.type)
		else:
			# To get full definitions for dependencies, we may require USR -> type definition map
			usr_to_record[node.get_usr()] = node.type
	elif node.kind in (CursorKind.FUNCTION_DECL, CursorKind.VAR_DECL):
		get_children = False
	
	if get_children:
		for c in node.get_children():
			collect(c)

def parse_header(path: str, clang_args: list):
	index = Index.create()
	tu = index.parse(path, args=[
		"-x", "c", # language
		'-target', 'arm-none-eabi',
		"-std=c99",
		"-fsyntax-only",
		"-DSDK_ARM9", "-DSDK_TS", "-D_DEBUG", "-DUNICODE", "-D_UNICODE", # defines for Nitro
		"-Dstatic_assert(a)=_Static_assert(a, \"assertion failed\")", # why do static asserts gotta be weird?
		"-Wno-typedef-redefinition", # ... so? If the typedef changes, clang will give an error instead.
		"-Wno-comment", # don't warn about /* inside multi-line comment, silly
		"-Wno-pragma-once-outside-header",
		# our includes
		f"-I{PROJECT_ROOT}",
		f"-Iinclude",
		# devkit's includes
		f"-I{DEVKITPRO_ROOT}/devkitARM/lib/gcc/arm-none-eabi/14.2.0/include",
		f"-I{DEVKITPRO_ROOT}/devkitARM/lib/gcc/arm-none-eabi/14.2.0/include-fixed",
		f"-I{DEVKITPRO_ROOT}/devkitARM/arm-none-eabi/sys-include",
		f"-I{DEVKITPRO_ROOT}/devkitARM/arm-none-eabi/include",
	] + clang_args)

	error = False
	for diag in tu.diagnostics:
		# Weird.
		diag_str = str(diag)
		if "devkitARM/arm-none-eabi/include/sys/lock.h:51:1: warning: non-void function does not return a value" not in diag_str:
			print(diag_str)
		if diag.severity > 2:
			error = True
	if error:
		raise Exception("libclang error; see above")
	
	collect(tu.cursor)

# -----------------------
# NDS Watcher file generation
# -----------------------
class HeaderParseResult:
	typedefs: list[tuple[str, str]]
	definitions: dict[str, str]
	dependencys: dict[str, any] # name -> libclang type # Can I remove the name now?
	types_defined: set[str]

	def __init__(self, tdefs: list[tuple[str, str]], defs: dict[str, str], deps: dict, types_defined: set[str]):
		self.typedefs = tdefs
		self.definitions = defs
		self.dependencys = deps
		self.types_defined = types_defined

class TypeParseResult:
	name: str
	file_contents: str
	dependencys: dict[str, any]

	def __init__(self, name: str, file_contents: str, deps: dict):
		self.name = name
		self.file_contents = file_contents
		self.dependencys = deps

class TypeInfo:
	name: str
	size: int
	field_lines: list[str]
	deps: dict

	def __init__(self, name, size, field_lines, deps):
		self.name = name
		self.size = size
		self.field_lines = field_lines
		self.deps = deps

def get_type_watcher_info(type_node) -> TypeInfo:
	lines = []
	deps = {}
	type_decl = type_node.get_declaration()
	is_union = type_decl.kind == CursorKind.UNION_DECL
	for field in type_node.get_fields():
		# If this is an anonymous record field, in-line it if possible.
		is_anon = field_is_anonymous(field)
		if is_anon and field.type.kind == TypeKind.RECORD and (type_decl.kind == CursorKind.STRUCT_DECL or field.type.get_declaration().kind == CursorKind.UNION_DECL):
			r = get_type_watcher_info(field.type)
			deps.update(r.deps)
			lines.extend(r.field_lines)
		else:
			tname = get_friendly_name(field.type)
			bfw = field.get_bitfield_width()
			fname = sanitize_name(field.spelling) if not is_anon else ('[]' if bfw == -1 else '_')
			type_size = field.type.get_size()
			if field.type.kind == TypeKind.POINTER and field.type.get_pointee().get_size() <= 0:
				# Pointer to incomplete type or void
				if tname != 'void*':
					deps[tname] = field.type
			elif type_size > 0 or field.type.kind == TypeKind.INCOMPLETEARRAY:
				# Commplete arrays can have size 0, but probably shouldn't.
				deps[tname] = field.type
			else:
				raise Exception(f'Field type {tname} for {type_node.spelling}.{fname} is invalid or incomplete. {field.location}')

			if bfw != -1:
				tname = f'{tname}:{bfw}'
			lines.append(f'{tname} {fname}')
	if is_union:
		lines = [ ', '.join(lines) ]

	name = get_friendly_name(type_node)
	if name in watcher_primitives:
		raise Exception('Getting info for primitive!')
	if len(lines) == 0:
		raise Exception(f'Type {name} has no fields?')
	if '*' in name:
		raise Exception(f'Getting info for pointer! {name} {type_node.spelling} {type_node.kind}')
	if type_node.get_size() <= 0:
		# Incomplete type. Can happen if it is declared but not defined.
		# Actually, there's no reason we should even be here trying to define a type that isn't defined.
		raise Exception(f'Attempted to define incomplete type {type_node.spelling}, {type_node.get_declaration().location}')
	else:
		return TypeInfo(name, type_node.get_size(), lines, deps)

def get_type_watcher_file(type_node) -> TypeParseResult:
	decl = type_node.get_declaration()
	if decl.kind == CursorKind.ENUM_DECL:
		return get_enum_watcher_file(type_node)
	
	main_type_info = get_type_watcher_info(type_node)

	collective_deps = {}
	collective_deps.update(main_type_info.deps)

	# Anonymous types should be placed inside this same file.
	type_count = 0
	included_types = [ main_type_info ]
	included_names: set[str] = set()
	included_names.add(main_type_info.name)
	while type_count != len(included_types):
		type_count = len(included_types)

		# Add anonymous types to this file if they're not already included
		new_deps = {}
		for dname in collective_deps:
			if dname in included_names:
				continue
			dtype = collective_deps[dname]
			if dtype.get_declaration().is_anonymous():
				info = get_type_watcher_info(dtype)
				included_types.append(info)
				included_names.add(dname)
				new_deps.update(info.deps)
		collective_deps.update(new_deps)

	# Build the NDS_Watcher definitions
	type_definitions = []
	for t in included_types:
		type_definitions.append(f'{t.name} 0x{t.size:x}\nfields {len(t.field_lines)}\n{'\n'.join(t.field_lines)}\n')
	file_contents = '\n'.join(type_definitions)

	# More friendly anonymous type names for included types
	replaced_count = 0
	for t in sorted(included_names):
		if t == main_type_info.name:
			continue
		if collective_deps[t].get_declaration().is_anonymous():
			replaced_count += 1
			file_contents = file_contents.replace(t, f'anonymousType{replaced_count}')

	# remove deps that are in this file
	for t in included_types:
		collective_deps.pop(t.name, None)

	return TypeParseResult(get_friendly_name(type_node), file_contents, collective_deps)

def get_enum_watcher_file(enum_node):
	lines = []
	for child in enum_node.get_declaration().get_children():
		if child.kind == CursorKind.ENUM_CONSTANT_DECL:
			lines.append(f'{sanitize_name(child.spelling)} {child.enum_value}')
	# For now, all enums are size 4.
	return TypeParseResult(get_friendly_name(enum_node), f'enum {get_friendly_name(enum_node)} 0x4\nvalues {len(lines)}\n{'\n'.join(lines)}\n', {})
	
def to_watcher_files(header_file) -> HeaderParseResult:
	files: dict = {}
	typedefs: list[tuple[str, str]] = []
	types_defined = set()
	deps = {}

	# Enums
	for enum in enums_by_header.get(header_file, []):
		result = get_enum_watcher_file(enum)
		types_defined.add(result.name)
		files[f'{result.name}.txt'] = result.file_contents

	# typedef
	for tdef_info in typedefs_by_header[header_file]:
		types_defined.add(tdef_info.new_name)
		deps[tdef_info.old_name] = tdef_info.renamed_type
		typedefs.append((tdef_info.old_name, tdef_info.new_name))
	
	# struct/union
	for type in records_by_header[header_file]:
		tpr = get_type_watcher_file(type)
		deps.update(tpr.dependencys)
		types_defined.add(tpr.name)
		files[f'{tpr.name}.txt'] = tpr.file_contents

	return HeaderParseResult(typedefs, files, deps, types_defined)

# -----------------------
# Main
# -----------------------
def main():
	Config.set_library_file(LLVM_LIB)
	OUT_DIR.mkdir(exist_ok=True, parents=True)
	if not Path(DEVKITPRO_ROOT).exists():
		raise Exception(f'Did not find devkitPro at {DEVKITPRO_ROOT} - Please set environment variable DEVKITPRO')

	#CURSOR_UP = "\033[1A"
	CLEAR = "\x1b[2K"

	using_dot_all = (Path(PROJECT_ROOT) / '_all.h').exists()
	if using_dot_all:
		headers = [ str((Path(PROJECT_ROOT) / "_all.h").absolute()) ]
	else:
		headers = list(Path(PROJECT_ROOT).rglob("*.h"))
		headers = [ get_full_path(h) for h in headers ]
		if not headers:
			print(f"No headers found under {PROJECT_ROOT}")
			sys.exit(1)
	
	# scan
	print("parsing")
	clang_args = sys.argv[1:]
	for header in headers:
		print(CLEAR, end=f'Parsing {header}\r')
		parse_header(str(header), clang_args)

	if using_dot_all:
		headers = set(records_by_header.keys()) \
		| set(typedefs_by_header.keys()) \
		| set(enums_by_header.keys())

	# collect types
	all_typedefs = set()
	dependencys = DependencySet()
	print("generating")
	for h in headers:
		if not is_project_file(h):
			continue

		result = to_watcher_files(h)
		path = Path(h).parent.relative_to(project_path_abs)
		for file in result.definitions:
			out_dir = OUT_DIR / path
			out_dir.mkdir(parents = True, exist_ok = True)
			out_file = out_dir / file
			if out_file.exists():
				raise Exception(f'Duplicate flie name {out_file}')
			else:
				out_file.write_text(result.definitions[file])
				print(CLEAR, end=f'Generated {out_file}\r')
		all_typedefs.update(result.typedefs)
		for d in result.dependencys:
			dependencys.add(result.dependencys[d])
		for d in result.types_defined:
			dependencys.set_fulfilled(d)
	print(CLEAR, end='')
	print('Done generating types for project files.')
	print('Moving to dependencies...')
	
	# collect dependencys
	deps_dir = OUT_DIR / 'dependencys'
	if dependencys.has_unfulfilled():
		deps_dir.mkdir()
	while dependencys.has_unfulfilled():
		dtype = dependencys.get_unfulfilled()
		
		if dtype.kind == TypeKind.TYPEDEF:
			tdef_node = dtype.get_declaration()
			tdef_info = get_typedef_info(tdef_node)

			if tdef_info.new_name != '':
				dependencys.add(tdef_info.renamed_type)
				dependencys.set_fulfilled(tdef_info.new_name)
				all_typedefs.add((tdef_info.old_name, tdef_info.new_name))
			else:
				# The typedef was probably something like `typedef struct Foo Foo;`. We resolve "struct Foo" to "Foo" already.
				# So the new dependency has the same name, and we don't need this typedef.
				dependencys.replace(tdef_info.renamed_type)
			continue

		if dtype.get_size() < 0:
			# This will happen if we had a pointer to an incomplete type. E.g. a type that was forward declared but never defined.
			type_decl = dtype.get_declaration()
			if not type_decl.is_definition() and type_decl.get_usr() in usr_to_record:
				dtype = usr_to_record[type_decl.get_usr()]

		if dtype.get_size() < 0:
			all_typedefs.add(('void', get_friendly_name(dtype)))
			dependencys.set_fulfilled(get_friendly_name(dtype))
		else:
			result = get_type_watcher_file(dtype)
			dname = get_friendly_name(dtype)
			if dname != result.name:
				raise Exception(f'Parsing type {dname} returned name {result.name}')
			out_file = OUT_DIR / f'dependencys/{result.name}.txt'
			if out_file.exists():
				raise Exception(f'Duplicate flie name {out_file}')
			else:
				out_file.write_text(result.file_contents)
				print(CLEAR, end=f'Generated {out_file}                     \r')
			for d in result.dependencys:
				dependencys.add(result.dependencys[d])
			dependencys.set_fulfilled(dname)

	print(CLEAR, end='')
	print('Done generating all types. Writing typedefs file...')

	if len(all_typedefs) > 0:
		out_file = OUT_DIR / 'typedefs.txt'
		lines = []
		for a, b in sorted(all_typedefs):
			lines.append(f'{a} {b}\n')
		out_file.write_text(''.join(lines))

	print(f"✅ All bindings generated in {OUT_DIR.resolve()}")

if __name__ == "__main__":
	main()

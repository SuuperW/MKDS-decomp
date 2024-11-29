import os

def create_all_header(dir_path, guard_base = '', base_path = None):
	"""
	Create a _all.h file in the specified directory that includes all header files in that directory,
	and recursively includes _all.h from subdirectories.
	"""
	basename: str = os.path.basename(dir_path)
	if base_path == None:
		base_path = os.path.abspath(dir_path)
	if basename != '.':
		# Ghidra does not like periods in #defines.
		guard_base += basename.upper().replace('.', '_') + '_'

	# Get a list of all files in the directory
	files = os.listdir(dir_path)
	
	# List to hold header file includes for the current directory
	header_files = []
	
	# Recursively traverse subdirectories to create _all.h files
	for filename in files:
		filepath = os.path.join(dir_path, filename)
		
		# If the file is a directory, process it recursively
		if os.path.isdir(filepath):
			if filename != 'include' and not filename.startswith('.'):
				# Recursively create _all.h for the subdirectory
				if create_all_header(filepath, guard_base, base_path):
					# Include the _all.h of the subdirectory in the current directory's _all.h
					# Ghidra will (silently!) fail if we have a self-relative path with a slash. It MUST be base-relative.
					header_files.append(f'#include "{os.path.relpath(filepath, base_path).replace('\\', '/')}/_all.h"')
		
		# If the file is a C header file, add it to the list
		elif filename.endswith('.h') and filename != "_all.h":
			header_files.append(f'#include "{filename}"')
	
	if len(header_files) == 0:
		return False
	
	# Create _all.h file in the current directory
	all_header_path = os.path.join(dir_path, "_all.h")
	
	# Write the includes to the _all.h file
	with open(all_header_path, 'w') as f:
		# Add a header guard to the _all.h file
		guard_name = f'_{guard_base}ALL_H'
		f.write(f'#ifndef {guard_name}\n')
		f.write(f'#define {guard_name}\n\n')
		
		# Write all the #include directives
		for header in header_files:
			f.write(header + '\n')
		
		# End the header guard
		f.write(f'\n#endif // {guard_name}\n')

	print(f"Created {all_header_path} with {len(header_files)} includes.")
	return True

if __name__ == '__main__':
	# Start the script by calling the function on the current directory
	create_all_header('.')

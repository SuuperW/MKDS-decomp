# Add traditional header guards to files that have #pragma once.
# We do this because Ghidra does not understand #pragma once.

import os

# Function to insert a blank line as the second line of a file
def insert_header_guard(file_path: str, guardDefine: str):
	try:
		with open(file_path, 'r') as file:
			lines = file.readlines()

		# Check for #pragma once w/o header guard.
		if lines[0].startswith("#pragma once"):
			index = 1
			while index < len(lines) and len(lines[index]) == 1:
				index += 1
			if lines[index].startswith("#if"):
				return
		# Insert guard
		if lines[1] != '\n':
			lines.insert(1, '\n')
		lines.insert(2, f'#ifndef {guardDefine}\n')
		lines.insert(3, f'#define {guardDefine}\n')
		lines.insert(4, '\n')
		if not lines[-1].endswith('\n'):
			lines[-1] += '\n'
		if not lines[-1] == '\n':
			lines.append('\n')
		lines.append('#endif\n')

		# Fix slashes for Ghidra, it needs regular ones
		for i in range(len(lines)):
			if lines[i].startswith('#include '):
				lines[i] = lines[i].replace('\\', '/')

		# Write the modified content back to the file
		with open(file_path, 'w') as file:
			file.writelines(lines)
		print(f"Modified file: {file_path}")
	except Exception as e:
		print(f"Could not process file {file_path}: {e}")

# Function to walk through all files in the current directory and subdirectories
def process_files_in_directory(root_dir):
	for root, dirs, files in os.walk(root_dir):
		for file_name in files:
			file_path = os.path.join(root, file_name)
			file_path = file_path.replace('\\', '/')
			# Process the file if it is a regular file (and not inside /include)
			if os.path.isfile(file_path) and file_path.endswith('.h') and not f'{root_dir}/include' in file_path:
				guardDefine = 'HP_' + file_path[len(root_dir)+1:].upper()
				guardDefine = guardDefine.replace(' ', '_').replace('.', '_').replace('/', '_')
				insert_header_guard(file_path, guardDefine)

# Main execution starts here
if __name__ == '__main__':
	current_directory = os.getcwd()
	process_files_in_directory(current_directory)

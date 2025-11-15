"""
We do not use pytest because
1) pytest is stupid: It allows unit tests to affect each other, with no option to prevent this. (All tests run in the same interpreter, module init logic runs only once across all tests.)
2) pytest-forked is stupid: It allows the code under test to see the --forked argument.
3) All our test files would look identical anyway.
"""

import os
from pathlib import Path
import shutil
import subprocess
import sys

from tests.file_comparer import compare

def test(path: str):
	print(f'Running test {path}')
	out_path = str(Path(path) / 'out')
	if Path(out_path).exists():
		shutil.rmtree(out_path)

	src_path = str(Path(path) / 'src')
	if not Path(src_path).exists():
		print(f'test path {src_path} does not exist')
		return False

	os.environ['NW_SOURCE'] = src_path
	os.environ['NW_OUT'] = out_path
	output = subprocess.run([sys.executable, "generateNW.py", f"-I{path}/include"], stderr=subprocess.STDOUT, stdout=subprocess.PIPE, text=True)

	diffs = compare(path)
	if len(diffs) != 0:
		print(f'Test {path} failed: {str(diffs)}')
		if output.returncode != 0:
			print(output.stdout)
		else:
			print('Generator reported success. Output:')
			print(output.stdout)
		return False
	else:
		shutil.rmtree(out_path)
		return True

def find_dirs_with_src(root_path):
	result = []
	for dir_name in next(os.walk(root_path))[1]:
		test_dir = Path(root_path) / dir_name
		if (test_dir / 'src').is_dir():
			result.append(str(test_dir))

	return result

success_count = 0
all_count = 0
if len(sys.argv) == 1:
	for dir in find_dirs_with_src('tests'):
		if test(dir):
			success_count += 1
		all_count += 1
	print(f'Ran all tests. {success_count}/{all_count} passed.')
else:
	dir = 'tests/' + sys.argv[1]
	if test(dir):
		print(f'Ran 1 test. Passed.')
	else:
		print(f'Ran 1 test. Failed.')

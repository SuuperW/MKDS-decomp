import sys
import re

filePath = sys.argv[1]
contents = None
with open(filePath, 'r') as fs:
	contents = fs.read()

replacements = [
	['ulonglong', 'u64'],
	['longlong', 's64'],
	['uint', 'u32'],
]

for r in replacements:
	contents = contents.replace(r[0], r[1])

with open(filePath, 'w') as fs:
	fs.write(contents)

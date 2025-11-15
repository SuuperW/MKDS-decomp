import filecmp
from pathlib import Path


def list_text_files(root: Path):
	"""Return a set of all .txt files recursively relative to root."""
	return {p.relative_to(root) for p in root.rglob("*.txt")}


def compare(path: str):
	"""Compare text files in two directories recursively."""
	dir1 = (Path(path) / 'expect').resolve()
	dir2 = (Path(path) / 'out').resolve()

	files1 = list_text_files(dir1)
	files2 = list_text_files(dir2)

	only_in_1 = sorted(files1 - files2)
	only_in_2 = sorted(files2 - files1)

	# Files present in both
	common = files1 & files2
	different = []

	for rel_path in common:
		f1 = dir1 / rel_path
		f2 = dir2 / rel_path

		# Compare file content
		if not filecmp.cmp(f1, f2, shallow=False):
			different.append(str(rel_path))

	different.extend(only_in_1)
	different.extend(only_in_2)

	return different

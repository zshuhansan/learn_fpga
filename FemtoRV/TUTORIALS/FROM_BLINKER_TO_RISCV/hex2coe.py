import re, sys

src = sys.argv[1]
dst = sys.argv[2]

words = []
with open(src, "r", encoding="utf-8") as f:
    for line in f:
        for tok in re.findall(r"[0-9a-fA-F]{8}", line):
            words.append(tok.lower())

with open(dst, "w", encoding="utf-8") as f:
    f.write("memory_initialization_radix=16;\n")
    f.write("memory_initialization_vector=\n")
    for i, w in enumerate(words):
        f.write(w)
        f.write(",\n" if i != len(words) - 1 else ";\n")
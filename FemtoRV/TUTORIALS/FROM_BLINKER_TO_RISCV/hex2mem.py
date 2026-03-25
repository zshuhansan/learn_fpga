import re, sys
words=[]
for line in open(sys.argv[1]):
    words += re.findall(r"[0-9a-fA-F]{8}", line)
open(sys.argv[2],"w").write("\n".join(w.lower() for w in words) + "\n")
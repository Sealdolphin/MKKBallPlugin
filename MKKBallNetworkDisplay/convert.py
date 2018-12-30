#!/usr/bin/python3

import sys

tickets = set()

for line in sys.stdin:
    xx = line.strip().split()
    if len(xx) < 4:
        continue
    if xx[3] == "Bejött":
        tickets.add(xx[2])
    else:
        tickets.discard(xx[2])

print("\n".join(tickets))

import sys
import os
import re
import subprocess
import json

if len(sys.argv) != 3:
	print("Usage: python3 minMemorySearch.py <path-to-exec> <path-to-bench>")
	exit(-1)

if not os.path.isfile(sys.argv[1]):
	print("Path to executables invalid")
	exit(-1)

if not os.path.isfile(sys.argv[2]):
	print("Path to benchmark directory invalid")
	exit(-1)

sim_lattice = sys.argv[1]
benchmark = sys.argv[2]
filename = os.path.basename(benchmark)
for gcopt in ['gc-plain', 'gc-live']:
	maxMem = 10000
	minMem = 1
	cmdlist = [sim_lattice, benchmark, str(maxMem), gcopt]
	while re.search("Terminating", subprocess.run(cmdlist, stderr=subprocess.PIPE, stdout=subprocess.PIPE).stderr.decode('utf-8')):
		minMem = maxMem
		maxMem = maxMem * 2
		cmdlist[2] = str(maxMem)
		print(filename + ": Trying with " + str(maxMem))

	while maxMem != minMem:
		mid = (minMem + maxMem) // 2
		cmdlist[2] = str(mid)
		notWorking = re.search("Terminating", subprocess.run(cmdlist, stderr=subprocess.PIPE, stdout=subprocess.PIPE).stderr.decode('utf-8'))

		if notWorking:
			minMem = mid + 1
		else:
			maxMem = mid

	print(filename, gcopt, str(minMem))
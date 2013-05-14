import csv
import matplotlib.pyplot as plt
from sys import argv, stdout
import  numpy as np

# Use for writing command-line scripts
def get_args(*names):
    assert len(names) == len(set(names))
    if len(argv) == 1:
        stdout.write("Enter "),
        for n in names:
          stdout.write(n)
          if n == names[-1]: stdout.write(': ')
          else: stdout.write(', ')
        params = raw_input().split()
    else:
        params = argv[1:]
    assert len(params) == len(names) # too many or too few arguments inputted on command line
    params = tuple([ str(n) for n in params ])
    return params



args = get_args("fileName")

data = csv.reader(open(args[0], 'rb'), delimiter=",", quotechar='|')
bfs, bfs2bfs, dfs2bfs, dfs2dfs, dfs = [], [], [], [], []

data.next()
for row in data:
    print row
    bfs.append(float(row[1]))
    bfs2bfs.append(float(row[2]))
    dfs2bfs.append(float(row[3]))
    dfs2dfs.append(float(row[4]))
    dfs.append(float(row[5]))

x = range(len(bfs))
plt.plot(x, sorted(bfs), label='bfs')
plt.plot(x, sorted(bfs2bfs), label='bfs2bfs')
plt.plot(x, sorted(dfs2bfs), label='dfs2bfs')
plt.plot(x, sorted(dfs2dfs), label='dfs2dfs')
plt.plot(x, sorted(dfs), label='dfs')
plt.legend()
plt.show()

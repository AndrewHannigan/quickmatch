from csv_helper import *
import matplotlib.pyplot as plt
import numpy as np
import math


CSV_DIR = "../../csv/"


#print "Avg BFS2BFS N=100000 edges:\t", avg_col(CSV_DIR + "B2B_100000.csv", 5)
#print "Avg BFS2BFS N=50000 edges:\t", avg_col(CSV_DIR + "B2B_50000.csv", 5)
#print "Avg BFS2BFS N=25000 edges:\t", avg_col(CSV_DIR + "B2B_25000.csv", 5)
#print "Avg BFS2BFS N=12500 edges:\t", avg_col(CSV_DIR + "B2B_12500.csv", 5)
#print "Avg BFS2BFS N=6200 edges:\t", avg_col(CSV_DIR + "B2B_6200.csv", 5)
#print "Avg BFS2BFS N=3100 edges:\t", avg_col(CSV_DIR + "B2B_3100.csv", 5)
#print "Avg BFS2BFS N=1550 edges:\t", avg_col(CSV_DIR + "B2B_1550.csv", 5)
#print "Avg BFS2BFS N=775 edges:\t", avg_col(CSV_DIR + "B2B_775.csv", 5)
#
#
#print "Avg BFS edges: ", avg_col(CSV_DIR + "alttest_N100000d3.csv", 3)
#print "Avg DFS edges: ", avg_col(CSV_DIR + "alttest_N100000d3.csv", 4)
#print "Avg BFS2BFS edges: ", avg_col(CSV_DIR + "alttest_N100000d3.csv", 5)
#print "Avg DFS2BFS edges: ", avg_col(CSV_DIR + "alttest_N100000d3.csv", 6)
#print "Avg DFS2DFS edges: ", avg_col(CSV_DIR + "alttest_N100000d3.csv", 7)
#print "Avg Hopcroft edges: ", avg_col(CSV_DIR + "alttest_N100000d3.csv", 8)



# BFS2BFS time complexity for increasing N
fig = plt.figure()
ax = fig.add_subplot(1,1,1)
ax.set_title("d = 3")
x = [775,1550,3100,6200,12500,25000,50000,100000]
y = [175.0, 254.0, 369.0, 521.0, 800.0, 1155.0, 1702.0, 2598.0]
ax.plot(x,y,label="Average", marker="o")
normalized_y = [y[i] / ((0.0023)*(x[i])**(0.6)) for i in range(len(x))]
print normalized_y
ax.plot(x,normalized_y,label="Normalized O(N^0.6)",marker="x")
ax.set_xlabel("N")
ax.set_ylabel("edges traversed")
ax.set_xscale("log",basex=2)
ax.legend(loc=0)
print "BFS2BFS for increasing N DONE"


def plot_dtest_avg(ax, x, y, N):
  ax.plot(x,y,label="Average", marker = '.')
  ax.set_xlabel("d")
  ax.set_ylabel("Miss Ratio")
  print "dtest_avg N = " + str(N) + " DONE"


def plot_dtest_worst(ax, x, y, N):
  ax.plot(x,y,label="Worst Case", marker = '.')
  ax.set_xlabel("d")
  ax.set_ylabel("Miss Ratio")
  x1,x2,y1,y2 = ax.axis()
#  ax.axis((x1,x2,y1-1,y2))
  print "dtest_worst N = " + str(N) + " DONE"

# dtest
fig1 = plt.figure()
ax1 = fig1.add_subplot(2,2,1)
ax1.set_title("N = 100")
ax2 = fig1.add_subplot(2,2,2)
ax2.set_title("N = 1000")
ax3 = fig1.add_subplot(2,2,3)
ax3.set_title("N = 10000")
ax4 = fig1.add_subplot(2,2,4)
ax4.set_title("N = 100000")


N100_file = "../../dtest_N100d20interval2.csv"
N1000_file = "dtest_N1000d40interval4.csv"
N10000_file = "dtest_N10000d44interval4.csv"
N50000_file = "dtest_N50000d19interval2.csv"


x,y = avg_plot_data(N100_file, 1, 0)
y = [y[i]/100. for i in range(len(y))]
plot_dtest_avg(ax1, x, y, 100)

x,y = avg_plot_data(CSV_DIR + N1000_file, 1, 0)
y = [y[i]/1000. for i in range(len(y))]
plot_dtest_avg(ax2, x, y, 1000)

x,y = avg_plot_data(CSV_DIR + N10000_file, 1, 0)
y = [y[i]/10000. for i in range(len(y))]
plot_dtest_avg(ax3, x, y, 10000)

x,y = avg_plot_data(CSV_DIR + N50000_file, 1, 0)
y = [y[i]/100000. for i in range(len(y))]
plot_dtest_avg(ax4, x, y, 100000)


fig1 = plt.figure()
ax1 = fig1.add_subplot(2,2,1)
ax1.set_title("N = 100")
ax2 = fig1.add_subplot(2,2,2)
ax2.set_title("N = 1000")
ax3 = fig1.add_subplot(2,2,3)
ax3.set_title("N = 10000")
ax4 = fig1.add_subplot(2,2,4)
ax4.set_title("N = 100000")

x,y = max_plot_data(N100_file, 1, 0)
y = [y[i]/100. for i in range(len(y))]
plot_dtest_worst(ax1, x, y, 100)

x,y = max_plot_data(CSV_DIR + N1000_file, 1, 0)
y = [y[i]/1000. for i in range(len(y))]
plot_dtest_worst(ax2, x, y, 1000)

x,y = max_plot_data(CSV_DIR + N10000_file, 1, 0)
y = [y[i]/10000. for i in range(len(y))]
plot_dtest_worst(ax3, x, y, 10000)

x,y = max_plot_data(CSV_DIR + N50000_file, 1, 0)
y = [y[i]/100000. for i in range(len(y))]
plot_dtest_worst(ax4, x, y, 100000)






# ntest
d3_file = "ntest_d3N25to51200interval2.csv"

fig = plt.figure()
ax = fig.add_subplot(1,1,1)
ax.set_title("d = 3")
x,y = avg_plot_data(CSV_DIR + d3_file, 2, 0)
y = [y[i]/float(x[i]) for i in range(1,len(y))]
ax.plot(x[1:],y, label="Average", marker = 'o')
ax.set_xlabel("N")
ax.set_ylabel("Miss Ratio")
ax.set_xscale("log", basex=2)
print "ntest_avg DONE"


fig = plt.figure()
ax = fig.add_subplot(1,1,1)
ax.set_title("d = 3")
x,y = avg_plot_data(CSV_DIR + d3_file, 2, 0)
ax.plot(x,y,label="Average", marker="o")
normalized_y = [y[i] / (0.02*(x[i])**(0.4)) for i in range(len(x))]
ax.plot(x,normalized_y,label="Normalized O(N^0.4)",marker="x")
ax.set_xlabel("N")
ax.set_ylabel("Number of Missed Matches")
ax.set_xscale("log",basex=2)
ax.legend(loc=0)
print "ntest_avg_normalization DONE"


fig = plt.figure()
ax = fig.add_subplot(1,1,1)
ax.set_title("d = 3")
x,y = max_plot_data(CSV_DIR + d3_file, 2, 0)
y = [y[i] / float(x[i]) for i in range(len(x))]
ax.plot(x,y,label="Maximum", marker = 'o')
ax.set_xlabel("N")
ax.set_ylabel("Miss Ratio")
ax.set_xscale("log",basex=2)
print "ntest_worst DONE"



#alttest --> N=100000, d=3
reader,f,ncol = open_csv(CSV_DIR + "alttest_N100000d3.csv")
bfs_steps = []
dfs_steps = []
bfs2bfs_steps = []
dfs2bfs_steps = []
dfs2dfs_steps = []
hopcroft_steps = []
bfs_time = []
dfs_time = []
bfs2bfs_time = []
dfs2bfs_time = []
dfs2dfs_time = []
hopcroft_time = []
for row in reader:
    bfs_steps.append(float(row[3]))
    dfs_steps.append(float(row[4]))
    bfs2bfs_steps.append(float(row[5]))
    dfs2bfs_steps.append(float(row[6]))
    dfs2dfs_steps.append(float(row[7]))
    hopcroft_steps.append(float(row[8]))
    bfs_time.append(float(row[9]))
    dfs_time.append(float(row[10]))
    bfs2bfs_time.append(float(row[11]))
    dfs2bfs_time.append(float(row[12]))
    dfs2dfs_time.append(float(row[13]))
    hopcroft_time.append(float(row[14]))


fig = plt.figure()
#fig.suptitle("Distribution of Costs for Finding a Single Alternating Path for N = 100000, d = 3")
ax1 = fig.add_subplot(1,1,1)
ax1.set_title("Number of Edges Traversed for d=3, N=100000")
x = range(len(bfs_steps))
ax1.plot(x,sorted(hopcroft_steps), label='hopcroft')
ax1.plot(x,sorted(bfs_steps), label='bfs')
ax1.plot(x,sorted(dfs_steps), label='dfs')
ax1.plot(x,sorted(bfs2bfs_steps), label='bfs2bfs')
ax1.plot(x,sorted(dfs2bfs_steps), label='bfs2dfs')
ax1.plot(x,sorted(dfs2dfs_steps), label='dfs2dfs')
ax1.set_ylabel("edges visited")
ax1.set_xlabel("trial # (sorted by number of edges traverse)")
ax1.legend(loc=2)

fig = plt.figure()
ax2 = fig.add_subplot(1,1,1)
ax2.set_title("CPU Time for d=3, N=100000")
ax2.plot(x,sorted(hopcroft_time), label='hopcroft')
ax2.plot(x,sorted(bfs_time), label='bfs')
ax2.plot(x,sorted(dfs_time), label='dfs')
ax2.plot(x,sorted(bfs2bfs_time), label='bfs2bfs')
ax2.plot(x,sorted(dfs2bfs_time), label='bfs2dfs')
ax2.plot(x,sorted(dfs2dfs_time), label='dfs2dfs')
ax2.set_ylabel("time (seconds)")
ax2.set_xlabel("trial # (sorted by CPU time)")
ax2.legend(loc=2)

f.close()
plt.show()

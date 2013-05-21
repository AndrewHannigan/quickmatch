from csv_helper import *
import matplotlib.pyplot as plt
import numpy as np


CSV_DIR = "../../csv/"


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

N100_file = "dtest_N100d20interval2.csv"
N1000_file = "dtest_N1000d40interval4.csv"
N10000_file = "dtest_N10000d44interval4.csv"
N50000_file = "dtest_N50000d19interval2.csv"

x,y = avg_plot_data(CSV_DIR + N100_file, 1, 0)
ax1.plot(x,y,label="Average")
ax1.set_xlabel("d")
ax1.set_ylabel("Missed Matches")
ax1.legend()

x,y = avg_plot_data(CSV_DIR + N1000_file, 1, 0)
ax2.plot(x,y,label="Average")
ax2.set_xlabel("d")
ax2.set_ylabel("Missed Matches")
ax2.legend()

x,y = avg_plot_data(CSV_DIR + N10000_file, 1, 0)
ax3.plot(x,y,label="Average")
ax3.set_xlabel("d")
ax3.set_ylabel("Missed Matches")
ax3.legend()

x,y = avg_plot_data(CSV_DIR + N50000_file, 1, 0)
ax4.plot(x,y,label="Average")
ax4.set_xlabel("d")
ax4.set_ylabel("Missed Matches")
ax4.legend()

fig1.suptitle("Average case QuickMatch performance for variable d")




fig1 = plt.figure()
ax1 = fig1.add_subplot(2,2,1)
ax1.set_title("N = 100")
ax2 = fig1.add_subplot(2,2,2)
ax2.set_title("N = 1000")
ax3 = fig1.add_subplot(2,2,3)
ax3.set_title("N = 10000")
ax4 = fig1.add_subplot(2,2,4)
ax4.set_title("N = 100000")

x,y = max_plot_data(CSV_DIR + N100_file, 1, 0)
ax1.plot(x,y,label="Maximum")
ax1.set_xlabel("d")
ax1.set_ylabel("Missed Matches")
ax1.legend()

x,y = max_plot_data(CSV_DIR + N1000_file, 1, 0)
ax2.plot(x,y,label="Maximum")
ax2.set_xlabel("d")
ax2.set_ylabel("Missed Matches")
ax2.legend()

x,y = max_plot_data(CSV_DIR + N10000_file, 1, 0)
ax3.plot(x,y,label="Maximum")
ax3.set_xlabel("d")
ax3.set_ylabel("Missed Matches")
ax3.legend()

x,y = max_plot_data(CSV_DIR + N50000_file, 1, 0)
ax4.plot(x,y,label="Maximum")
ax4.set_xlabel("d")
ax4.set_ylabel("Missed Matches")
ax4.legend()

fig1.suptitle("Worst case QuickMatch performance for variable d")



# ntest
d3_file = "ntest_d3N25to51200interval2.csv"

fig = plt.figure()
ax1 = fig.add_subplot(1,1,1)
ax1.set_title("d = 3")

x,y = avg_plot_data(CSV_DIR + d3_file, 2, 0)
ax1.plot(x,y,label="Average")
ax1.legend()

fig.suptitle("Average case QuickMatch performance for variable N")


fig = plt.figure()
ax1 = fig.add_subplot(1,1,1)
ax1.set_title("d = 3")

x,y = max_plot_data(CSV_DIR + d3_file, 2, 0)
ax1.plot(x,y,label="Maximum")
ax1.legend()

fig.suptitle("Worst case QuickMatch performance for variable N")


#alttest --> N=1000
reader,f,ncol = open_csv(CSV_DIR + "alttest_N1000d3.csv")
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
    bfs_steps.append(row[3])
    dfs_steps.append(row[4])
    bfs2bfs_steps.append(row[5])
    dfs2bfs_steps.append(row[6])
    dfs2dfs_steps.append(row[7])
    hopcroft_steps.append(row[8])
    bfs_time.append(row[9])
    dfs_time.append(row[10])
    bfs2bfs_time.append(row[11]) 
    dfs2bfs_time.append(row[12]) 
    dfs2dfs_time.append(row[13])
    hopcroft_time.append(row[14])


fig = plt.figure()
fig.suptitle("Distribution of Costs for Finding a Single Alternating Path when N = 1000")
ax1 = fig.add_subplot(1,2,1)
ax1.set_title("Number of Edges Visited")
ax1.plot(sorted(bfs_steps), label='bfs')
ax1.plot(sorted(dfs_steps), label='dfs')
ax1.plot(sorted(bfs2bfs_steps), label='bfs2bfs')
ax1.plot(sorted(dfs2bfs_steps), label='bfs2dfs')
ax1.plot(sorted(bfs2bfs_steps), label='dfs2dfs')
ax1.plot(sorted(bfs_steps), label='hopcroft')

ax2 = fig.add_subplot(1,2,2)
ax2.set_title("CPU Time")
ax2.plot(sorted(bfs_steps), label='bfs')
ax2.plot(sorted(bfs_steps), label='dfs')
ax2.plot(sorted(bfs_steps), label='bfs2bfs')
ax2.plot(sorted(bfs_steps), label='bfs2dfs')
ax2.plot(sorted(bfs_steps), label='dfs2dfs')
ax2.plot(sorted(bfs_steps), label='hopcroft')

f.close()



plt.show()



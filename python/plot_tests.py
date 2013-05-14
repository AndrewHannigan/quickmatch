from csv_helper import *
import matplotlib.pyplot as plt
import numpy as np


CSV_DIR = "../../csv/"

# X AXIS: d
# Y AXIS: avg num unmatched
fig = plt.figure()
ax1 = fig.add_subplot(3,2,1)
ax1.set_title("N = 100")
ax2 = fig.add_subplot(3,2,2)
ax2.set_title("N = 1000")
ax3 = fig.add_subplot(3,2,3)
ax3.set_title("N = 10000")
ax4 = fig.add_subplot(3,2,4)
ax4.set_title("N = 100000")
ax5 = fig.add_subplot(3,2,5)
ax5.set_title("N = 10")
ax6 = fig.add_subplot(3,2,6)
ax6.set_title("N = 300")



x,y = avg_plot_data(CSV_DIR + "dtest_N100d49.csv", 1, 0)
ax1.plot(x,y,label="Average")
x,y = max_plot_data(CSV_DIR + "dtest_N100d49.csv", 1, 0)
ax1.plot(x,y,label="Maximum")
ax1.legend()

x,y = avg_plot_data(CSV_DIR + "dtest_N1000d39.csv", 1, 0)
ax2.plot(x,y,label="Average")
x,y = max_plot_data(CSV_DIR + "dtest_N1000d39.csv", 1, 0)
ax2.plot(x,y,label="Maximum")
ax2.legend()

x,y = avg_plot_data(CSV_DIR + "dtest_N10000d29.csv", 1, 0)
ax3.plot(x,y,label="Average")
x,y = max_plot_data(CSV_DIR + "dtest_N10000d29.csv", 1, 0)
ax3.plot(x,y,label="Maximum")
ax3.legend()

x,y = avg_plot_data(CSV_DIR + "dtest_N100000d19.csv", 1, 0)
ax4.plot(x,y,label="Average")
x,y = max_plot_data(CSV_DIR + "dtest_N100000d19.csv", 1, 0)
ax4.plot(x,y,label="Maximum")
ax4.legend()

x,y = avg_plot_data(CSV_DIR + "dtest_N10d9.csv", 1, 0)
ax5.plot(x,y,label="Average")
x,y = max_plot_data(CSV_DIR + "dtest_N10d9.csv", 1, 0)
ax5.plot(x,y,label="Maximum")
ax5.legend()

x,y = avg_plot_data(CSV_DIR + "dtest_N300d140interval10.csv", 1, 0)
ax6.plot(x,y,label="Average")
x,y = max_plot_data(CSV_DIR + "dtest_N300d140interval10.csv", 1, 0)
ax6.plot(x,y,label="Maximum")
ax6.legend()

plt.show()



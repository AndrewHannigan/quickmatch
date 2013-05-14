from csv_helper import *
import matplotlib.pyplot as plt
import numpy as np


CSV_DIR = "../../csv/"


# plotting num unmatched
fig1 = plt.figure()
ax1 = fig1.add_subplot(2,2,1)
ax1.set_title("N = 100")
ax2 = fig1.add_subplot(2,2,2)
ax2.set_title("N = 1000")
ax3 = fig1.add_subplot(2,2,3)
ax3.set_title("N = 10000")
ax4 = fig1.add_subplot(2,2,4)
ax4.set_title("N = 100000")

x,y = avg_plot_data(CSV_DIR + "dtest_N100d99interval2.csv", 1, 0)
ax1.plot(x,y,label="Average")
x,y = max_plot_data(CSV_DIR + "dtest_N100d99interval2.csv", 1, 0)
ax1.plot(x,y,label="Maximum")
ax1.legend()

x,y = avg_plot_data(CSV_DIR + "dtest_N1000d49interval8.csv", 1, 0)
ax2.plot(x,y,label="Average")
x,y = max_plot_data(CSV_DIR + "dtest_N1000d49interval8.csv", 1, 0)
ax2.plot(x,y,label="Maximum")
ax2.legend()

x,y = avg_plot_data(CSV_DIR + "dtest_N10000d29interval6.csv", 1, 0)
ax3.plot(x,y,label="Average")
x,y = max_plot_data(CSV_DIR + "dtest_N10000d29interval6.csv", 1, 0)
ax3.plot(x,y,label="Maximum")
ax3.legend()

x,y = avg_plot_data(CSV_DIR + "dtest_N100000d19interval4.csv", 1, 0)
ax4.plot(x,y,label="Average")
x,y = max_plot_data(CSV_DIR + "dtest_N100000d19interval4.csv", 1, 0)
ax4.plot(x,y,label="Maximum")
ax4.legend()





# plotting p
fig2 = plt.figure()
ax1 = fig2.add_subplot(2,2,1)
ax1.set_title("N = 100")
ax2 = fig2.add_subplot(2,2,2)
ax2.set_title("N = 1000")
ax3 = fig2.add_subplot(2,2,3)
ax3.set_title("N = 10000")
ax4 = fig2.add_subplot(2,2,4)
ax4.set_title("N = 100000")

x,y = avg_plot_data(CSV_DIR + "dtest_N100d99interval2.csv", 1, 3)
ax1.plot(x,y,label="Average")
x,y = max_plot_data(CSV_DIR + "dtest_N100d99interval2.csv", 1, 3)
ax1.plot(x,y,label="Maximum")
ax1.legend()

x,y = avg_plot_data(CSV_DIR + "dtest_N1000d49interval8.csv", 1, 3)
ax2.plot(x,y,label="Average")
x,y = max_plot_data(CSV_DIR + "dtest_N1000d49interval8.csv", 1, 3)
ax2.plot(x,y,label="Maximum")
ax2.legend()

x,y = avg_plot_data(CSV_DIR + "dtest_N10000d29interval6.csv", 1, 3)
ax3.plot(x,y,label="Average")
x,y = max_plot_data(CSV_DIR + "dtest_N10000d29interval6.csv", 1, 3)
ax3.plot(x,y,label="Maximum")
ax3.legend()

x,y = avg_plot_data(CSV_DIR + "dtest_N100000d19interval4.csv", 1, 3)
ax4.plot(x,y,label="Average")
x,y = max_plot_data(CSV_DIR + "dtest_N100000d19interval4.csv", 1, 3)
ax4.plot(x,y,label="Maximum")
ax4.legend()


plt.show()



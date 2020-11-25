# Python script to generate plots from the CSV files
import matplotlib.pyplot as plt
import csv
from scipy.interpolate import make_interp_spline,BSpline
import numpy as np

def plot_graph():

    x=[]
    y=[]
    file_name1 = "./data/nthreads_s_lock.csv"
    file_name2 = "./data/nthreads_rw_lock.csv"
    file_name3 = "./data/nthreads_hoh_lock.csv"
    i = 0
    s = 0
    with open(file_name1, 'r') as csvfile:
        plots= csv.reader(csvfile, delimiter=',')
        for row in plots:
            i+=1
            s+= float (row[1])
            x.append(float(row[0]))
            y.append(float(float(s)/float(i)))

    x_new = np.linspace(min(x), max(x), 2000) 
    spl = make_interp_spline(x, y, k=3)
    y_smooth = spl(x_new)

    plt.plot(x_new,y_smooth, color="orange", label="S_Lock", linewidth=1.5)

    x=[]
    y=[]
    i=0 
    s = 0
    with open(file_name2, 'r') as csvfile:
        plots= csv.reader(csvfile, delimiter=',')
        for row in plots:
            i+=1
            s+= float (row[1])
            x.append(float(row[0]))
            y.append(float(float(s)/float(i)))

    x_new = np.linspace(min(x), max(x), 2000) 
    spl = make_interp_spline(x, y, k=3)
    y_smooth = spl(x_new)

    plt.plot(x_new,y_smooth, color="green", label="RW_Lock", linewidth=1.5) 
    

    x=[]
    y=[]
    i = 0
    s= 0
    with open(file_name3, 'r') as csvfile:
        plots= csv.reader(csvfile, delimiter=',')
        for row in plots:
            i+=1
            s+= float (row[1])
            x.append(float(row[0]))
            y.append(float(float(s)/float(i)))

    x_new = np.linspace(min(x), max(x), 2000) 
    spl = make_interp_spline(x, y, k=3)
    y_smooth = spl(x_new)

    plt.plot(x_new,y_smooth, color="red", label="HOH_Lock", linewidth=1.5)

    
    plt.title("Workload performance comparision wrt Concurrent Threads: Averaged Out")

    plt.xlabel('Number of Threads')
    plt.ylabel('Time (s)')
    plt.legend(loc='best')

    plt.savefig("./plots/nthreads_comp_avg",dpi = 600)
    plt.close()

plot_graph()

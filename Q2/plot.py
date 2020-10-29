# Python script to generate plots from the CSV files
import matplotlib.pyplot as plt
import csv
from scipy.interpolate import make_interp_spline,BSpline
import numpy as np

def plot_graph(flag):

    x=[]
    y=[]
    file_name1 = "./data/"+flag+"_"+ "FIFO.csv"
    file_name2 = "./data/"+flag+"_"+ "LRU.csv"
    file_name3 = "./data/"+flag+"_"+ "LRUAppr.csv"
    file_name4 = "./data/"+flag+"_"+ "RANDOM.csv"
    
    with open(file_name1, 'r') as csvfile:
        plots= csv.reader(csvfile, delimiter=',')
        for row in plots:
            x.append(float(row[0]))
            y.append(float(row[1]))

    x_new = np.linspace(min(x), max(x), 2000) 
    spl = make_interp_spline(x, y, k=3)
    y_smooth = spl(x_new)

    plt.plot(x_new,y_smooth, color="orange", label="FIFO", linewidth=1.5)

    x=[]
    y=[]
    with open(file_name2, 'r') as csvfile:
        plots= csv.reader(csvfile, delimiter=',')
        for row in plots:
            x.append(float(row[0]))
            y.append(float(row[1]))

    x_new = np.linspace(min(x), max(x), 2000) 
    spl = make_interp_spline(x, y, k=3)
    y_smooth = spl(x_new)

    plt.plot(x_new,y_smooth, color="green", label="LRU", linewidth=1.5) 
    

    x=[]
    y=[]
    with open(file_name3, 'r') as csvfile:
        plots= csv.reader(csvfile, delimiter=',')
        for row in plots:
            x.append(float(row[0]))
            y.append(float(row[1]))



    x_new = np.linspace(min(x), max(x), 2000) 
    spl = make_interp_spline(x, y, k=3)
    y_smooth = spl(x_new)

    plt.plot(x_new,y_smooth, color="red", label="LRU APPROX", linewidth=1.5)

    x=[]
    y=[]
    with open(file_name4, 'r') as csvfile:
        plots= csv.reader(csvfile, delimiter=',')
        for row in plots:
            x.append(float(row[0]))
            y.append(float(row[1]))

    x_new = np.linspace(min(x), max(x), 2000) 
    spl = make_interp_spline(x, y, k=3)
    y_smooth = spl(x_new)

    plt.plot(x_new,y_smooth, color="blue", label="RANDOM", linewidth=1.5)
    
    plt.title(flag+" Work Load comparision")

    plt.xlabel('Cache Size')
    plt.ylabel('Hit Rate (%)')
    plt.legend(loc='best')

    plt.savefig("./plots/"+flag,dpi = 600)
    plt.close()


plot_graph("LOCAL")
plot_graph("LOOP")
plot_graph("RANDOM")
import csv
import numpy as np
import matplotlib.pyplot as plt



def open_csv(fileName):
    f = open(fileName, 'rb')
    reader = csv.reader(f, delimiter=",", quotechar='|')
    ncol=len(next(reader)) # Read first line and count columns
    f.seek(0)
    return reader,f,ncol



def csv_cols(reader, f, ncol):
    f.seek(0)
    cols = [[] for _ in range(ncol)]
    
    x = 0
    for row in reader:
        print x 
        x += 1
        for i in range(ncol):
            cols[i].append(float(row[i]))
            
    return cols

def avg_col(fileName, col):
    reader,f,_ = open_csv(fileName)
    f.seek(0)
    
    total = 0
    count = 0
    for row in reader:
        total += float(row[col])
        count += 1
    
    f.close()
    return total/count 
        

def max_col(fileName, col):
    reader,f,_ = open_csv(fileName)
    f.seek(0)
    
    maximum = float("-inf")
    for row in reader:
        x = float(row[col])
        if x > maximum:
            maximum = x
    
    f.close()
    return maximum 
        
def scatter_plot(fileName, x_col, y_col):
    reader,f,_ = open_csv(fileName)
    for row in reader:
        plt.plot([row[x_co]], [row[y_col]])
    plt.show()
    
def avg_plot_data(fileName, category_col, value_col):
    reader,f,_ = open_csv(fileName)
    total_buckets = {}
    count_buckets = {}
    for row in reader:
        category = float(row[category_col])
        value = float(row[value_col])
        if category in total_buckets:
            total_buckets[category] += value
            count_buckets[category] += 1
        else:
            total_buckets[category] = value
            count_buckets[category] = 1
    keys = sorted(total_buckets.keys())
    x = keys
    y = [float(total_buckets[k]) / count_buckets[k] for k in keys]
    return (x,y)
    
    
def max_plot_data(fileName, category_col, value_col):
    reader,f,_ = open_csv(fileName)
    buckets = {}
    for row in reader:
        category = float(row[category_col])
        value = float(row[value_col])
        if category in buckets:
            if value > buckets[category]:
                buckets[category] = value
        else:
            buckets[category] = value
    keys = sorted(buckets.keys())
    x = keys
    y = [buckets[k] for k in keys]
    return (x,y)
    


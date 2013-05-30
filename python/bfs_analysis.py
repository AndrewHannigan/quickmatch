import matplotlib.pyplot as plt


def computeLevelSizes(N,d,setSize):
    N = float(N)
    d = float(d)
    f = [setSize]
    level = 0
    while True:
        level += 1
        if level % 2 == 1:
            pastLevelsSum = sum([f[i] for i in range(level) if i % 2 == 1])
            if pastLevelsSum>=N:
                break
            f.append((1-pastLevelsSum/N)*d*f[level-1])
        else:
            f.append(f[level-1])

    print "Ended at level: ", level
    return f
        
y = computeLevelSizes(100000,3,1)
y_cum = [sum(y[0:(i+1)]) for i in range(len(y))]
print y
print y_cum
plt.plot(y)
plt.xlabel("Level")
plt.ylabel("Number of Vertices")
plt.title("Modeling the Growth of BFS Levels")
plt.show()



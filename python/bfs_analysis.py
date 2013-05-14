
import matplotlib.pyplot as plt

def computeRevisitProb(N,L,k):
    # Number of visited nodes
    visited = sum([L[i] for i in range(k) if i % 2 == k % 2])
    return (1.0 - min(1.0, (float(visited) / float(N))))

def expectedLayerSizes(N,d,startingSize,targetSize):
    L = [1]
    terminationProb=[0]
    steps = [0]
    for k in range(1,N):
        L.append(computeRevisitProb(N,L,k) * d * L[k-1])
        if L[k] == 0: break
        #unvisited = N-sum([L[i] for i in range(k) if i % 2 == k % 2])
        #terminationProb.append(L[k]/(N-visited))
        
    return L
        
y = expectedLayerSizes(100000,2,1,1)
x = [sum(y[0:(i+1)]) for i in range(len(y))]
print y
print x
plt.plot(y)
plt.show()



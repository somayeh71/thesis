from CLA_NET import *
import os


#reading each csv files and use them as input for the CLA_NET algorithm
s=0
path = 'size/'
files = []
filesNoAdd = []
# r=root, d=directories, f = files
for r, d, f in os.walk(path):
    for file in f:
        if '.csv' in file:
            files.append(os.path.join(r, file))
            filesNoAdd.append(file)
            s=s+1
# for f in files:
#     print(f)
path1 = 'siz-membership/'

files1 = []
filesNoAdd1 = []
# r=root, d=directories, f = files
for r1, d1, f1 in os.walk(path1):
    for file1 in f1:
        if '.csv' in file1:
            files1.append(os.path.join(r1, file1))
            filesNoAdd1.append(file1)
# for f1 in files1:
#     print(f1)




memShip = 0
for f in files:
    a=f
    b=files1[memShip]
    memShip += 1
    print(a,b)

    A,result=getData(a,b)

Q=[]
t=[]
Q[s],t[s]=CLA_NET(a,b)

plt.subplot(2, 1, 1)
plt.plot(Q, 'r-')
plt.xlabel("noise")
plt.ylabel("Modularity")

plt.subplot(2, 1,2)
plt.plot(t, 'r-')
plt.xlabel("noise")
plt.ylabel("Iteration")
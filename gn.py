
import matplotlib.pyplot as plt


#plt.plot([10,500,1000,2000,4000,6000,8000,10000,15000],[25497,37337,34452,41464,41024,41861,41095,42275,42303])
# plt.plot([10,500,1000,2000,4000,6000,8000,10000,15000],[25497,35085,34452,41464,41024,41861,41095,42275,42303],'-o')
#
# plt.ylabel("Iteration")
# plt.ylim((20000, 50000))
# plt.xlabel("Number of Edges")

from mpl_toolkits import mplot3d
#matplotlib inline
import numpy as np
import matplotlib.pyplot as plt

fig = plt.figure()
ax = plt.axes(projection='3d')


plt.ylabel("Iteration")
plt.xlim((0, 1))
plt.ylim((0, 10000))

plt.xlabel("Alpha Rate")
x_data=[1,0.5,0.05,0.005,0.0005]#alpha
y_data=[103,106,206,1008,8008,] #iteration
#z_data=[0.7226,0.8326,0.7489,0.7489,0.7489] #NMI
#ax.set_zlabel('NMI');
#plt.plot(x_data,y_data,z_data,'-o')
#ax.scatter3D(x_data, y_data, z_data);

z_data=[0.1892,0.2962,0.4444,0.4444,0.4444]
ax.set_zlabel('Modularity');
ax.scatter3D(x_data, y_data, z_data);

plt.show()
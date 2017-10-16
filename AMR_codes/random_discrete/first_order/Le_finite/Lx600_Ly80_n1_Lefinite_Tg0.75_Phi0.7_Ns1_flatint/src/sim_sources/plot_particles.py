#! /usr/bin/env python
import numpy as np
import matplotlib.pyplot as plt

#read data from file
infile_par='par_list.dat'
x=[];y=[];rad=[];

with open(infile_par,'r') as fin:
	line_0=fin.readline()
 	x_initial = 0
    	x_final = int(line_0.split()[0]) * int(line_0.split()[2])
    	y_initial = 0
   	y_final = int(line_0.split()[1]) * int(line_0.split()[2])
      	read_line = lambda: fin.readline()
       	for line in iter(read_line,''):
             	if(len(line.split())>1):
                        x.append(line.split()[0])
                        y.append(line.split()[1])
                        rad.append(line.split()[2])

x=np.float64(x)
y=np.float64(y)
rad=np.float64(rad)

# output figures size (inches)
h=15.
w=h*(x_final-x_initial)/(y_final-y_initial)

fig=plt.figure(figsize=(w,h))
fig_ax=fig.add_axes([0,0,1,1],frame_on=False)
plt.axis('off')
plt.xlim(x_initial,x_final)
plt.ylim(y_initial,y_final)

for np in range(len(x)):
    circle=plt.Circle((x[np],y[np]),rad[np],color='black',fill=False)
    fig.gca().add_artist(circle)

fig.savefig('par_list.png')
plt.close()


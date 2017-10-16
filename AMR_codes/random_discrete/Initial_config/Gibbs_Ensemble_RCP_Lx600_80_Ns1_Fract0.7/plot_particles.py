#! /usr/bin/env python
import numpy as np
import matplotlib.pyplot as plt
import math
#read data from file
infile_par ='aaa_config.dat'
if(0):
	Fract =   raw_input('Fraction?\n')   
	density   =int(raw_input('Density?\n'))
	Lx = int(raw_input('Lx?\n'))
	Ly = int(raw_input('Ly?\n'))
	#calculate diameter
	fraction = float("{0:.3f}".format(float(Fract)))
	dim =np.sqrt(4 * fraction/math.pi/density)
	dim = float("{0:.3f}".format(dim))
	file_num  =int(raw_input('file number?\n'))
	infile_par='Partconf_real_Lx_Ly_'+str(int(Lx))+'_'+str(int(Ly))+'_'+str(int(file_num))+'_Fract'+str(Fract)+'_Ns'+str(int(density))+'_Dim'+str(float(dim))+'.dat'


x=[];y=[];rad=[];

with open(infile_par,'r') as fin:
	line_0=fin.readline()
 	x_initial = 0#int(line_0.split()[0]) * int(line_0.split()[4])
    	x_final = 2#int(line_0.split()[1]) * int(line_0.split()[4])
    	y_initial = 0#int(line_0.split()[2]) * int(line_0.split()[4])
   	y_final = 2#int(line_0.split()[3]) * int(line_0.split()[4])
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
h=20.
#w=h*(x_final-x_initial)/(y_final-y_initial)
w=h * (x_final-x_initial)/(y_final-y_initial)

fig=plt.figure(figsize=(w,h))
fig_ax=fig.add_axes([0,0,1,1],frame_on=False)
plt.axis('off')
plt.xlim(x_initial,x_final)
plt.ylim(y_initial,y_final)

for np in range(len(x)):
    circle=plt.Circle((x[np],y[np]),rad[np],color='black',fill=False,linewidth=2)
    fig.gca().add_artist(circle)


fig.savefig('aaapar_list.png')

#fig.savefig('par_list_real'+str(int(file_num))+'_Fract'+str(Fract)+'_Ns'+str(int(density))+'_Dim'+str(float(dim))+'.png')
plt.close()


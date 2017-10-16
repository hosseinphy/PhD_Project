#! /usr/bin/env python
import numpy as np
from scipy.interpolate import griddata
import matplotlib
matplotlib.use('Agg')
import matplotlib.pyplot as plt
from numpy import arange


########## Input parameters:

Tint = 0.75

# min-max for colormaps
Tmin = 0.
Tmax = 1.
Cmin = 0.
Cmax = 1.

filen = 'aaa' #raw_input('filename?\n')
first_num = 0 #raw_input('first?\n')
last_num  = 256000#raw_input('last?\n')
step_num=16000
#if(int(first_num)!=int(last_num)):
#	step_num = raw_input('step?\n')

#uniform plot grid parameters
x_initial = 0 #int(raw_input('x_initial?\n')) 	#x_initial=0
x_final = 80 #int(raw_input('x_final?\n')) 		#x_final=512
npoints_x = 1600 #int(raw_input('npoints_x?\n')) 	#npoints_x=1000 
y_initial = 0 #int(raw_input('y_initial?\n')) 	#y_initial=0
y_final = 100 #int(raw_input('y_final?\n')) 		#y_final=512
npoints_y = 2000 #int(raw_input('npoints_y?\n')) 	#npoints_y=1000

dx = ( float (x_final) - float (x_initial) ) / float (npoints_x)
dy = ( float (y_final) - float (y_initial) ) / float (npoints_y)

midnx = (npoints_x+1)/2
if (npoints_x%2 == 0):
	midnx = npoints_x/2

#flags 1=Yes, 0=No
flag_grid_plot=0 	#Plot grid?
flag_T_plot=1 		#Plot T?
flag_c_plot=0		#Plot c?
flag_par_plot=1         # show particles in T and c plots?

y0 = 5.+2. 		# Window/Interface initial position
window_amp = 5  	# Amplitude of moving window 

# output figures size (inches)
h=6.
w=h*(x_final-x_initial)/(y_final-y_initial)


########## End input parameters


#Uniform plot grid
xi = np.linspace(x_initial,x_final,npoints_x)
yi = np.linspace(y_initial,y_final,npoints_y)

if(int(step_num)==0):
    step_num=int(last_num)-int(first_num)+1 # step=0 does not work in range()

# Interface evolution figure
if(flag_T_plot==1):
	fig0=plt.figure(figsize=(w,h))
	fig0_ax=fig0.add_axes([0,0,1,1],frame_on=False)

# for interface plot/s
x_axis = np.linspace(1,npoints_x,npoints_x)
y_axis = np.linspace(1,npoints_y,npoints_y)

y_final0 = y_final
w0 = w


if(flag_par_plot==1):
    # read particles data
    infile_par='par_list.dat'
    parpx=[];parpy=[];parrad=[]
    with open(infile_par,'r') as fin:
        read_line = lambda: fin.readline()
        for line in iter(read_line,''):
                if(len(line.split())>1):
                        parpx.append(line.split()[0])
                        parpy.append(line.split()[1])
                        parrad.append(line.split()[2])
    parpx=np.float64(parpx)
    parpy=np.float64(parpy)
    parrad=np.float64(parrad)

for num in range(int(first_num),int(last_num)+1,int(step_num)):

	#read data from file
	in_num=str(num)
	infile=filen+in_num+'.dat'
	print in_num
	x=[]; y=[]; T=[]; c=[];
	with open(infile,'r') as fin:
		nnodes=fin.readline()
		read_line = lambda: fin.readline()
		for line in iter(read_line,''):
			if(len(line.split())>1):
				x.append(line.split()[0])
				y.append(line.split()[1])
				T.append(line.split()[2])
				c.append(line.split()[3])

	x=np.float64(x)
	y=np.float64(y)

	if(flag_grid_plot==1):
		# plot original grid
		fig=plt.figure(figsize=(w,h))
		fig_ax=fig.add_axes([0,0,1,1],frame_on=False)
  		plt.scatter(x,y,marker='o',edgecolors='none',c='black',s=1)
		plt.xlim(x_initial,x_final)
  		plt.ylim(y_initial,y_final)
  		plt.axis('off')
		fg_plot="grid"+in_num+".png"
		plt.savefig(fg_plot)
		plt.close()

	if(flag_c_plot==1):
		# grid and plot c
		fig=plt.figure(figsize=(w,h))
		fig_ax=fig.add_axes([0,0,1,1],frame_on=False)
		zi=griddata((x,y),c,(xi[None,:],yi[:,None]),method='linear')
		plt.contourf(xi,yi,zi,100,cmap=plt.cm.jet,vmin=Cmin,vmax=Cmax)
		plt.axis('off')
       		# plot particles:
		if(flag_par_plot==1):
			for n_p in range(len(parpx)):
				circ=plt.Circle((parpx[n_p],parpy[n_p]),parrad[n_p],color='gray',fill=False,linewidth=0.1)
				plt.gca().add_artist(circ)
        
		fg_plot="c"+in_num+".png"
		plt.savefig(fg_plot)
		plt.close(fig)

		# plot c through center
		fig_mids=plt.figure(figsize=(8,4))
		plt.ylim(0,1.5)
		#plt.ylabel('')
		plt.plot(yi,zi[:,midnx],'k',color='red',label='Conc')
		plt.xlabel('y')
		if(flag_T_plot!=1):
			plt.xlim(x_initial,x_final)
			plt.ylim(y_initial,y_final)
			plt.legend(loc='upper right', shadow=True)
			plt.savefig("TC"+in_num+".png")
			plt.close(fig_mids)

	if(flag_T_plot==1):
		# grid and plot T
		fig=plt.figure(figsize=(w,h))
		fig_ax=fig.add_axes([0,0,1,1],frame_on=False)
		zi=griddata((x,y),T,(xi[None,:],yi[:,None]),method='linear')
		plt.contourf(xi,yi,zi,100,cmap=plt.cm.jet,vmin=Tmin,vmax=Tmax)
		plt.axis('off')
        	# plot particles:
		if(flag_par_plot==1):
			for n_p in range(len(parpx)):
				circ=plt.Circle((parpx[n_p],parpy[n_p]),parrad[n_p],color='black',fill=False,linewidth=0.4)
				plt.gca().add_artist(circ)
        
		fg_plot="T"+in_num+".png"
		plt.savefig(fg_plot)
		plt.close()

		# plot T through center
		if(flag_c_plot!=1):
			fig_mids=plt.figure(figsize=(8,4))
			plt.ylim(0,1.5)
		#plt.ylabel('')
		plt.plot(yi,zi[:,midnx],'k',color='blue',label='Temp')
		plt.xlabel('y')
		plt.xlim(y_initial,y_final)
		plt.ylim(0,1.5)
		plt.legend(loc='upper right', shadow=True)
		plt.savefig("TC"+in_num+".png")
		plt.close(fig_mids)

		########## Plot interface (isoterm T_int)
		zi = np.array(zi).reshape(npoints_y,npoints_x)
		fig=plt.figure(figsize=(w,h))
		fig_ax=fig.add_axes([0,0,1,1],frame_on=False)
		fig_ax.contour(x_axis, y_axis, zi, [Tint], linewidths=0.5, colors='k')
		fig_ax.axis('off')
		#plt.gca().invert_yaxis()
		fig.savefig("T_int"+in_num+".png")
		plt.close(fig)

		##### plot interface evolution
		fig0_ax.contour(x_axis, y_axis, zi, [Tint], linewidths=0.5, colors='k')

if(flag_T_plot==1):
	# interface evolution
	fig0_ax.axis('off')
	#plt.gca().invert_yaxis()
	fig0.savefig("T_int_all.png")	
	plt.close(fig0)


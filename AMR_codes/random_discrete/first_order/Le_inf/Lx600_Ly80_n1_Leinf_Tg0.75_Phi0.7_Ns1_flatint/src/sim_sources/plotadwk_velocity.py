#! /home/hophy/ENV/bin python
import math
import numpy as np
from scipy.interpolate import griddata
import matplotlib.pyplot as plt
from numpy import arange
from numpy import diff

def savitzky_golay(y, window_size, order, deriv=0, rate=1):
    r"""Smooth (and optionally differentiate) data with a Savitzky-Golay filter.
    The Savitzky-Golay filter removes high frequency noise from data.
    It has the advantage of preserving the original shape and
    features of the signal better than other types of filtering
    approaches, such as moving averages techniques.
    Parameters
    ----------
    y : array_like, shape (N,)
        the values of the time history of the signal.
    window_size : int
        the length of the window. Must be an odd integer number.
    order : int
        the order of the polynomial used in the filtering.
        Must be less then `window_size` - 1.
    deriv: int
        the order of the derivative to compute (default = 0 means only smoothing)
    Returns
    -------
    ys : ndarray, shape (N)
        the smoothed signal (or it's n-th derivative).
    Notes
    -----
    The Savitzky-Golay is a type of low-pass filter, particularly
    suited for smoothing noisy data. The main idea behind this
    approach is to make for each point a least-square fit with a
    polynomial of high order over a odd-sized window centered at
    the point.
    Examples
    --------
    t = np.linspace(-4, 4, 500)
    y = np.exp( -t**2 ) + np.random.normal(0, 0.05, t.shape)
    ysg = savitzky_golay(y, window_size=31, order=4)
  ysg = savitzky_golay(y, window_size=31, order=4)
    import matplotlib.pyplot as plt
    plt.plot(t, y, label='Noisy signal')
    plt.plot(t, np.exp(-t**2), 'k', lw=1.5, label='Original signal')
    plt.plot(t, ysg, 'r', label='Filtered signal')
    plt.legend()
    plt.show()
    References
    ----------
    .. [1] A. Savitzky, M. J. E. Golay, Smoothing and Differentiation of
       Data by Simplified Least Squares Procedures. Analytical
       Chemistry, 1964, 36 (8), pp 1627-1639.
    .. [2] Numerical Recipes 3rd Edition: The Art of Scientific Computing
       W.H. Press, S.A. Teukolsky, W.T. Vetterling, B.P. Flannery
       Cambridge University Press ISBN-13: 9780521880688
    """
    import numpy as np
    from math import factorial

    try:
        window_size = np.abs(np.int(window_size))
        order = np.abs(np.int(order))
    except ValueError as e:
        print(e)
    #except ValueError, msg:
    #    raise ValueError("window_size and order have to be of type int")
    if window_size % 2 != 1 or window_size < 1:
        raise TypeError("window_size size must be a positive odd number")
    if window_size < order + 2:
        raise TypeError("window_size is too small for the polynomials order")
    order_range = range(order+1)
    half_window = (window_size -1) // 2
    # precompute coefficients
    b = np.mat([[k**i for i in order_range] for k in range(-half_window, half_window+1)])
    m = np.linalg.pinv(b).A[deriv] * rate**deriv * factorial(deriv)
    # pad the signal at the extremes with
    # values taken from the signal itself
    firstvals = y[0] - np.abs( y[1:half_window+1][::-1] - y[0] )
    lastvals = y[-1] + np.abs(y[-half_window-1:-1][::-1] - y[-1])
    y = np.concatenate((firstvals, y, lastvals))
    return np.convolve( m[::-1], y, mode='valid')


def find_nearest_value(array,value):
    idx = (np.abs(array-value)).argmin()
    return float(array[idx])

def find_nearest(array,value):
    idx = (np.abs(array-value)).argmin()

    if(float(idx)<len(array)-1):
    	if (float(array[idx+1])-float(array[idx])==0):
       		idx_interpolate=float(idx)
        else :
        	idx_interpolate=float(idx)+(float(value)-float(array[idx]))/(float(array[idx+1])-float(array[idx]))
        # idx,idx_interpolate
        idx = idx_interpolate
    return idx


##############################################################

########## Input Parameters:

Tint = 0.75
lam = 80  	# wavelength of perturbation
Lambda_half = 80/2/0.05


#uniform plot grid parameters
x_initial = 0 #int(raw_input('x_initial?\n')) 	#x_initial=0
x_final = 400 #int(raw_input('x_final?\n')) 		#x_final=512
npoints_x = 8000 #int(raw_input('npoints_x?\n')) 	#npoints_x=1000 
y_initial = 0 #int(raw_input('y_initial?\n')) 	#y_initial=0
y_final = 400 #int(raw_input('y_final?\n')) 		#y_final=512
npoints_y = 8000 #int(raw_input('npoints_y?\n')) 	#npoints_y=1000

#flags 1=Yes, 0=No
flag_grid_plot=0 	#Plot grid?
flag_T_plot=1 		#Plot T?
flag_c_plot=0		#Plot c?
flag_amp=1 		#Plot Perturbation Amplitude?
flag_window=1           # height of the window         

#moving window height around the interface
amp_Init =10.
window_amp = 30 	# Amplitude of plotting window (in y)

# min-max for colormaps
Tmin = 0.
Tmax = 1.
Cmin = 0.
Cmax = 1.

#files
filen = 'aaa' #raw_input('filename?\n')
first_num = 0#raw_input('first?\n')
last_num  = 5120000#raw_input('last?\n')
step_num =  25600#raw_input('step?\n')

#time step -> simulation time step * step_num
dt = 0.000025 * float(step_num)
y_final0 = y_final

# find the number of iterations
if(step_num ==0):
	num_itr = 0
else:
	num_itr = int(last_num)/int(step_num) + 1	

##############################################################

xlam = 2 * ( float (x_final) - float (x_initial) )

dx = ( float (x_final) - float (x_initial) ) / float (npoints_x)
dy = ( float (y_final) - float (y_initial) ) / float (npoints_y)

midnx = (npoints_x+1)/2
if (npoints_x%2 == 0):
	midnx = npoints_x/2

# output figures size (inches)
h=6.
#w=h*(x_final-x_initial)/(y_final-y_initial)

#Uniform plot grid
xi = np.linspace(x_initial,x_final,npoints_x)
yi = np.linspace(y_initial,y_final,npoints_y)

if(int(step_num)==0):
    step_num=int(last_num)-int(first_num)+1 # step=0 does not work in range()

amp_list = []; i0_list = []; i1_list = []; i2_list = []; v0_list = []; v1_list = [];

imean=np.zeros((num_itr,1));
Instant_Vel=np.zeros((num_itr,1));

T=[];x=[];y=[];c=[]; # field values and coordinated

if(step_num==0):
	iter_list = range(1)
else:

	iter_list = [num*dt for num in range(int(first_num),(num_itr)*(int(step_num)),int(step_num))]

print len(iter_list)

# Interface evolution figure
#if(flag_T_plot==1):
#	fig0=plt.figure(figsize=(w,h))
#	fig0_ax=fig0.add_axes([0,0,1,1],frame_on=False)

# for interface plot/s

x_axis = np.linspace(1,npoints_x,npoints_x)
y_axis = np.linspace(1,npoints_y,npoints_y)

#intiate fields and coordinates
for num in range(int(first_num),int(last_num)+1,int(step_num)):
	x.append([]); y.append([]); T.append([]); c.append([]);

for num in range(int(first_num),int(last_num)+1,int(step_num)):

	#read data from file
        itr_idx = num / int(step_num)
	in_num=str(num)
	infile=filen+in_num+'.dat'

	with open(infile,'r') as fin:
		nnodes=fin.readline()
		read_line = lambda: fin.readline()
		for line in iter(read_line,''):
			if(len(line.split())>1):
				x[itr_idx].append(line.split()[0])
				y[itr_idx].append(line.split()[1])
				T[itr_idx].append(line.split()[2])
				c[itr_idx].append(line.split()[3])

	x[itr_idx]=np.float64(x[itr_idx])
	y[itr_idx]=np.float64(y[itr_idx])

	if(flag_T_plot==1):
		zi=griddata((x[itr_idx],y[itr_idx]),T[itr_idx],(xi[None,:],yi[:,None]),method='linear')
		i0 = find_nearest(zi[:,0],Tint)* dy                   
  		imean[itr_idx] = i0
                print imean[itr_idx]

if(flag_T_plot==1):
        y0 = 10
	for i in range(0,len(imean),1):
                if (i==0):                	
			y_initial = y0 - window_amp                       
			y_final = y_initial + 2.*window_amp
			Velocity_0 = 0  #instanteneous velocity
                else :
			Velocity_0 = (imean[i]-imean[i-1])/dt  #instanteneous velocity 
			y_initial = y0 - window_amp + Velocity_0*dt*(i)
			y_final = y_initial + 2.*window_amp			

                Instant_Vel[i] = Velocity_0
	
'''
	
                y_axis0 = np.linspace(1,npoints_y,npoints_y)
        	yi0 = np.linspace(y_initial,y_final,npoints_y)
        	npoints_y = (2*window_amp) * npoints_y / (y_final-y_initial)
        	w=h*(x_final-x_initial)/(2*window_amp)
        	y_axis = np.linspace(1,npoints_y,npoints_y)

		yi = np.linspace(y_initial,y_final,npoints_y)
        	#print y_initial
      	  	#print y_final
        	int_num =  ((i) * int(step_num))
       	 	inum = str(int_num)
                print inum
		########## Plot T field 
		fig=plt.figure(figsize=(w,h))
 		fig_ax=fig.add_axes([0,0,1,1],frame_on=False)
		zi=griddata((x[i],y[i]),T[i],(xi[None,:],yi[:,None]),method='linear')
		plt.contourf(xi,yi,zi,100,cmap=plt.cm.jet,vmin=Tmin,vmax=Tmax)
		plt.axis('off')
		fg_plot="Twin"+inum+".png"
		plt.savefig(fg_plot)
		plt.clf()
                plt.close()

		########## Plot interface (isoterm T_int)
		zi = np.array(zi).reshape(npoints_y,npoints_x)
		fig=plt.figure(figsize=(w,h))
		fig_ax=fig.add_axes([0,0,1,1],frame_on=False)
		fig_ax.contour(x_axis, y_axis, zi, [Tint], linewidths=0.5, colors='k')
		fig_ax.axis('off')
		#plt.gca().invert_yaxis()
		fig.savefig("T_int"+inum+".png")
		plt.close(fig)
'''

if(flag_T_plot==1):

                #print velocity_ave               

                imean = np.array(imean)
                Instant_Vel = np.array(Instant_Vel)
                #print iter_list
                #velocity_list = (np.concatenate(Instant_Vel, axis=0))
                veloc = savitzky_golay(Instant_Vel[:,0], 51, 3) # window size 51, polynomial order 3
                pos_list = savitzky_golay(imean[:,0], 51, 3) # window size 51, polynomial order 3

                #print (len(veloc))
                #print (len(pos_list))
                #print (len(iter_list))
	        t_axis = [i*int(step_num)*dt for i in range(len(pos_list))]
 
	        for i in range(0,len(imean),1):
	                y_initial = imean[i,0]-window_amp#y0 - window_amp + veloc[i]*dt*i
                        y_final = y_initial + 2.*window_amp			

		        # avoid window moving beyond system limits:
		       # if y_initial < 0:
		       #         y_initial = 0
		       # if y_initial > y_final0 - 2*window_amp :
                       #         y_initial = y_final0 - 2*window_amp

		        y_axis0 = np.linspace(1,npoints_y,npoints_y)
        	        yi0 = np.linspace(y_initial,y_final,npoints_y)
        	        npoints_y = (2*window_amp) * npoints_y / (y_final-y_initial)
        	        w=h*(x_final-x_initial)/(2*window_amp)
        	        y_axis = np.linspace(1,npoints_y,npoints_y)

	  	        yi = np.linspace(y_initial,y_final,npoints_y)

        	        int_num =  ((i) * int(step_num))
       	 	        inum = str(int_num)
                        print inum
		        ########## Plot T field 
		        fig=plt.figure(figsize=(w,h))
 		        fig_ax=fig.add_axes([0,0,1,1],frame_on=False)
		        zi=griddata((x[i],y[i]),T[i],(xi[None,:],yi[:,None]),method='linear')
		        plt.contourf(xi,yi,zi,100,cmap=plt.cm.jet,vmin=Tmin,vmax=Tmax)
	     	        plt.axis('off')
		        fg_plot="Twin"+inum+".png"
		        plt.savefig(fg_plot)
		        plt.clf()
                        plt.close()

 		        ########## Plot velocity
		        fig=plt.figure(figsize=(8,4))
		        #plt.plot(iter_list,imean,'ko-')
		        plt.plot(t_axis,pos_list,'r-')
		        plt.plot(t_axis,veloc,'g-')
		        #plt.ylim(0,1.5)
		        plt.xlabel('time ')
		        plt.ylabel('Front position')
		        #plt.legend(loc='upper left', shadow=True)
		        fig.savefig("Front_pos"+".png")
		        plt.close(fig)




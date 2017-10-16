#! /usr/bin/env python
import numpy as np
import matplotlib.pyplot as plt
import math
from scipy.signal import argrelextrema

def input_number(msg):
    while True:
        try:
            res = round(float(input(msg)), 3)
            break
        except:
            print("Invalid")
    return res

#read data from file

Fract =   raw_input('Fraction?\n')   
density = int(raw_input('density?\n'))            
Lx = int(raw_input('Lx?\n'))
Ly = int(raw_input('Ly?\n'))
#calculate diameter
fraction = float("{0:.3f}".format(float(Fract)))
dim =np.sqrt(4 * fraction/math.pi/density)
dim = float("{0:.3f}".format(dim))
print dim
tot_real_num = int(raw_input('total number of realizations?\n'))            
infile_par='RDF_'+'Lx'+str(int(Lx))+'_Ly'+str(int(Ly))+'_tot_real'+str(int(tot_real_num))+'_Fract'+str(Fract)+"_Ns"+str(int(density))+"_Dim"+str(float(dim))+".dat"

x=[];y=[];
with open(infile_par,'r') as fin:
	line_0=fin.readline()
      	read_line = lambda: fin.readline()
       	for line in iter(read_line,''):
                x.append(line.split()[0])
                y.append(line.split()[1])

x=np.float64(x)
y=np.float64(y)

# Find the limit of x and y (For plotting ...)
mx=np.max(y) #this is to find the max RDF for plottign ...                   
min_idx=next((i for i, j in enumerate(y) if(j==np.min(y))))
#
# finding the correlation length
#mx_corr=(argrelextrema(y,np.greater)) # to find the correlation length
#mn_corr=np.max(argrelextrema(y,np.less)) # to find the correlation length
#print mx_corr
#Corr_len_idx = next(i for i, j in enumerate(x) if( (j>x[mx_corr] or j<x[mn_corr]) and (math.fabs(y[mx_corr]-1.0)<=0.1) ))

#print "Correlation length is :"
#print x[Corr_len_idx]

# find the nearest neighbour distance -> use to find the effective paramters
char_len_idx=next((i for i, j in enumerate(y) if(j<1)))

print  "Characterstic length is r/D :"
print x[char_len_idx]

fig=plt.figure(figsize=(8,5)) 
plt.plot(x,y,'k-o')
plt.axhline(y=1, color='k', linestyle='--')
plt.xlabel('r/D_{p} ') 
plt.ylabel('g(r)') 
plt.xlim(0,4)
plt.ylim(y[min_idx],y[0])
fig.savefig("Rdist_"+'Lx'+str(int(Lx))+'_Ly'+str(int(Ly))+'_tot_real'+str(int(tot_real_num))+'_Fract'+str(float(Fract))+"_Ns"+str(int(density))+"_Dim"+str(float(dim))+".png") 
plt.close(fig) 



#with open('amp'+str(int(xlam))+'.out','w') as amp_t:
#                        for i in range(len(amp_list)):
	#                                amp_t.write(str(iter_list[i])+" "+str(amp_list[i])+"\n")

#ifndef READ_INPUT_H
#define READ_INPUT_H

#define NAME_LEN 256

class read_input {

public:

int 	confignum; 	  		//# number of configuratiosn to calculate g2(r) 
int 	p_num;       			// number of particles in 1 dim
int 	save_freq;			//number of cycles between data saves
int 	ratio_freq;			//number of cycles between calculating g2(r)
int 	print_freq;			//number of cycles between print information
int 	boxX;				//box size in x direction
int 	boxY;				//box size in y direction
double  fraction;        	       	// initial volume (area) fraction
double  density;        	       	// Number density of particles per (region.x * region.y)
double  deltar;				//bin-width for calculations of g2(r)
double  deltar_max;			//max bin-width for calculations of g2(r)
char 	readfile[NAME_LEN];		//radial dist func. (RDF) g2(r) VS. r/D 
char 	writefile[NAME_LEN];		//radial dist func. (RDF) g2(r) VS. r/D 
char 	configuration[NAME_LEN];

int read(int argc, char* argv[]);

};


#endif





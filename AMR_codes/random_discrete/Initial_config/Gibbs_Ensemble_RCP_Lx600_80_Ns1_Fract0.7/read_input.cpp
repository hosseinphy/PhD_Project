#include <iomanip>
#include <iostream>
#include <stdio.h>
#include <fstream>

#include "read_input.h"

//================================================================
//
// Source File for input
//
//================================================================
input.read(int argc ,char* argv[])
{

int error =0 ;
  if (argc != 2) 
    {
    std::cout << "Syntax: spheres input" << std::endl;
    error = 1;
    } 
  else 
    {
    std::ifstream infile;
    infile.open(argv[1]);
    if(!infile)
      {
	std::cout << "Can't open " << argv[1] << " for input." << std::endl;
	error = 2;
	return error;
      } 
    else 
      {
	std::cout << "Reading input from file " << argv[1] << std::endl;
      }
    char buf[100],c;
    infile.get(buf,100,'='); infile.get(c); infile >> input.confignum;
    infile.get(buf,100,'='); infile.get(c); infile >> input.p_num;
    infile.get(buf,100,'='); infile.get(c); infile >> input.save_freq;
    infile.get(buf,100,'='); infile.get(c); infile >> input.ratio_freq;
    infile.get(buf,100,'='); infile.get(c); infile >> input.print_freq;
    infile.get(buf,100,'='); infile.get(c); infile >> input.fraction;
    infile.get(buf,100,'='); infile.get(c); infile >> input.deltar;
    infile.get(buf,100,'='); infile.get(c); infile >> input.deltar_max ;
    infile.width(NAME_LEN-1); infile >> input.readfile;
    infile.get(buf,100,'='); infile.get(c); 
    infile.width(NAME_LEN-1); infile >> input.writefile;
    infile.get(buf,100,'='); infile.get(c); 
    infile.width(NAME_LEN-1); infile >> input.configuration;


    if(infile.eof()) 
      {
	std::cout << "Error reading input file " << argv[1] << std::endl;
	error = 3;
      }
    }
return error ;

}

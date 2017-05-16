function[M] = temp_time_space_OXD_2d(mode,times,N,filename)

clc
close all

%Which Function do you want to run ???
  
 switch mode
     
 %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%   
  
  case 1
         
   %% Multi Purposes function%%%%
   %% 1) read from all profile.txt files
   %% 2) make a plot for Y VS X (posiyion of max temp)      
   
   %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
   %% N is the number of mesh points along y axis   %
   %% times : number of files equ number of times   % 
   %% filename is comon string for each file's name %
   %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
      
    disp('This function plot X-Y , T(X,Y)-X-Y and store all temp info in temp2D,dat in destination ...')              
    %Ask for the input from the user
    prompt   = 'Please enter the following parameters value? '     
    %%Num=input('How many files do you want to plot ?');        
    filenumber =  input('Please enter the number of the file : ');
    deltat   =  input('Please enter the combustion time : ');
    Tsub     =  input('Please enter the ignition temperature : ');    
      
    A3 = cell(times,1);
    A2=num2str(filenumber);
    A4=num2str(deltat,'%.2f')
    A5=num2str(Tsub,'%.3f')
    %%A5 =single(Tsub);
    A1=filename;        
    file=cell(times,1);  
    dx=0.25
    %start_time=3000;
    %end_time=3500;
    %duration=end_time-start_time;
    %% destination (if you would like to plot files in ather folder) : 
    folder='/Users/hosseinazizi/Documents/programms/simulation_results/parallel/Siva_cellular/2400_300_0.001_1_tau_0.05_tg_0.75_Le_10.66_tbak_0.01/';
    
    %% Parameter Definition
    Row=zeros(N,times);
    Col=zeros(N,times);
    Temp=zeros(N,times);   
    M=[];
    
    %Load files  
      counter=0;
      m=0;
       while(counter<times)          
        A3{counter+1} = int2str(counter);%%[counter num2str(counter,0)];
        formatSpec = ' %1$s%2$s_%3$s_%4$s_%5$s';;
        str = sprintf(formatSpec,A1,A2,A3{counter+1},A4,A5);
        str; 
        File=[str,'.txt'];
        File=strtrim(File);
        file_path=strcat(folder,File);      
        file{counter+1}= file_path;         
        disp(file{counter+1})
        
          %%%%%%%%%%%%%%%%%%%%%
          fid = fopen(file{counter+1},'r');
          if(fid<0);error('unable to open file');end
          %% (remove header):
          linesToSkip = 7;
           for ii = 1:linesToSkip-1
            fgetl(fid);
           end
            cntr = 1;
           while (true)
            line = fgetl(fid);
              if(line==-1);break; end; %-1 indicates end of file...
               if(line(1)=='$')
                continue;
               end            
              %% line = regexprep(line,',','');            
              M = [M; str2num(line)];             
            cntr = cntr +1;
           end 
          fclose(fid);                   
          %% assign X , Y and temp          
          Row (:,counter+1)= M(1+m:N+m,1);           
          Col (:,counter+1)= M(1+m:N+m,2);
          Temp(:,counter+1)= M(1+m:N+m,3);    
          m=m+N;
          counter=counter+1;
       end

%% Comment out following lines       
  if(0)               
   %% Setup a plot for Y VS T_max positions (X's)        
    figure(1)    
     %%clr = lines(2000);    %# LINES colormap    
       for i=1:times        
          for j=1:1:N
             plot(Col(j,i),Row(j,i),'b',...%'Color','b','LineWidth',2,...        
                         'MarkerEdgeColor','k',...
                         'MarkerFaceColor','k',...
                         'MarkerSize',3);    
             hold on;
          end
       end
       xlabel('$$ Position  of  max  temperature $$','interpreter','latex','FontSize',16)
       ylabel('$$Y axis $$','interpreter','latex','FontSize',18)
       % axis([300 N 1 times 1 2]
       hold off;     
  end
%% end of comments

   %% End of the diagram
   
       %% finding the max and min of temp , X and Y
       max_M=zeros(1,3);
       min_M=zeros(1,3);
       max_M(:)=max(M,[],1);
       min_M(:)=min(M,[],1);
       x_max=floor(max_M(2));
       x_min=floor(min_M(2));       
       y_max=max_M(1);
       y_min=min_M(1);       
       T_max=max_M(3);
       T_min=min_M(3);
       
       %% seting up the params for Surf plot
       xdim=x_min:x_max;
       ydim=y_min:y_max;
       [X,Y]=meshgrid(xdim,ydim);
       
       %% 2d array containg temperature of each positions(x,y)
       combustion=zeros(y_max,x_max);
       
       for a=1:y_max*times                    
         combustion(M(a,1),(M(a,2)))=M(a,3);                                     
       end
       
       %% fill in the gaps for combustion file
       %% if there is no corresponding x , equate T to zero
        for i=1:1:y_max
          for j=1:1:x_max
             if(combustion(i,j)==0)
               combustion(i,j)=1;
             end
          end
        end 
        
       %% write files
       
       file_w=['YX.dat'];           
       file_w=strtrim(file_w);
       file_w_path_1=strcat(folder,file_w);      
       file_temp_1= file_w_path_1;                       
       disp(file_temp_1); 
       fileID_1 = fopen(file_temp_1,'w');
       for i=1:times
          for j=1:N
            fprintf(fileID_1,'%d %d %f \n',Row(j,i),Col(j,i),Temp(j,i));
          end
       end           
       fclose(fileID_1);
       
       %% *******************
       file_w=['temp2D.dat'];           
       file_w=strtrim(file_w);
       file_w_path=strcat(folder,file_w);      
       file_temp= file_w_path;                       
       disp(file_temp); 
       fileID = fopen(file_temp,'w');
       for i=1:y_max
          for j=1:x_max
            fprintf(fileID,'%d %d %f \n',i,j,combustion(i,j));
          end
       end           
       fclose(fileID);
    
     
      
      %% -------------------------------------------- %%
             
   %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% END OF CASE 1
   
    otherwise
        warning('You entered the wrong choice.');

end
end
%%%%%%% END OF MAIN FUNCTION %%%%%%%%%      
          
%% ---------------------------------------------------------
%% function to find the calculate and plot the power spectrum

function raPsd2d(img,res)
% function raPsd2d(img,res)
%
% Computes and plots radially averaged power spectral density (power
% spectrum) of image IMG with spatial resolution RES.
%
% (C) E. Ruzanski, RCG, 2009

%% Process image size information
[N M] = size(img);

%% Compute power spectrum
imgf = fftshift(fft2(img));
imgfp = (abs(imgf)/(N*M)).^2;                                               % Normalize

%% Adjust PSD size
dimDiff = abs(N-M);
dimMax = max(N,M);
% Make square
if N > M                                                                    % More rows than columns
    if ~mod(dimDiff,2)                                                      % Even difference
        imgfp = [NaN(N,dimDiff/2) imgfp NaN(N,dimDiff/2)];                  % Pad columns to match dimensions
    else                                                                    % Odd difference
        imgfp = [NaN(N,floor(dimDiff/2)) imgfp NaN(N,floor(dimDiff/2)+1)];
    end
elseif N < M                                                                % More columns than rows
    if ~mod(dimDiff,2)                                                      % Even difference
        imgfp = [NaN(dimDiff/2,M); imgfp; NaN(dimDiff/2,M)];                % Pad rows to match dimensions
    else
        imgfp = [NaN(floor(dimDiff/2),M); imgfp; NaN(floor(dimDiff/2)+1,M)];% Pad rows to match dimensions
    end
end

halfDim = floor(dimMax/2) + 1;                                              % Only consider one half of spectrum (due to symmetry)

%% Compute radially average power spectrum
[X Y] = meshgrid(-dimMax/2:dimMax/2-1, -dimMax/2:dimMax/2-1);               % Make Cartesian grid
[theta rho] = cart2pol(X, Y);                                               % Convert to polar coordinate axes
rho = round(rho);
i = cell(floor(dimMax/2) + 1, 1);
for r = 0:floor(dimMax/2)
    i{r + 1} = find(rho == r);
end
Pf = zeros(1, floor(dimMax/2)+1);
for r = 0:floor(dimMax/2)
    Pf(1, r + 1) = nanmean( imgfp( i{r+1} ) );
end

%% Setup plot
fontSize = 14;
maxX = 10^(ceil(log10(halfDim)));
f1 = linspace(1,maxX,length(Pf));                                           % Set abscissa

% Find axes boundaries
xMin = 0;                                                                   % No negative image dimension
xMax = ceil(log10(halfDim));
xRange = (xMin:xMax);
yMin = floor(log10(min(Pf)));
yMax = ceil(log10(max(Pf)));
yRange = (yMin:yMax);

% Create plot axis labels
xCell = cell(1:length(xRange));
for i = 1:length(xRange)
    xRangeS = num2str(10^(xRange(i))*res);
    xCell(i) = cellstr(xRangeS);
end
yCell = cell(1:length(yRange));
for i = 1:length(yRange)
    yRangeS = num2str(yRange(i));
    yCell(i) = strcat('10e',cellstr(yRangeS));
end

%% Generate plot
figure
loglog(1./f1,Pf,'k-','LineWidth',2.0)
set(gcf,'color','white')
set(gca,'FontSize',fontSize,'FontWeight','bold','YTickLabel',yCell,'YMinorTick','off',...
    'XTickLabel',xCell,'XGrid','on','YAxisLocation','right','XDir','reverse');
xlabel('Wavelength (km)','FontSize',fontSize,'FontWeight','Bold');
ylabel('Power','FontSize',fontSize,'FontWeight','Bold');
title('Radially averaged power spectrum','FontSize',fontSize,'FontWeight','Bold')

end

%%%%%%% END OF power spectrum FUNCTION %%%%%%%%% 

%% ---------------------------------------------------------

%% Function to find the Extrima
    function [maxtab, mintab]=peakdet(v, delta, x)
%PEAKDET Detect peaks in a vector
%        [MAXTAB, MINTAB] = PEAKDET(V, DELTA) finds the local
%        maxima and minima ("peaks") in the vector V.
%        MAXTAB and MINTAB consists of two columns. Column 1
%        contains indices in V, and column 2 the found values.
%      
%        With [MAXTAB, MINTAB] = PEAKDET(V, DELTA, X) the indices
%        in MAXTAB and MINTAB are replaced with the corresponding
%        X-values.
%
%        A point is considered a maximum peak if it has the maximal
%        value, and was preceded (to the left) by a value lower by
%        DELTA.

% Eli Billauer, 3.4.05 (Explicitly not copyrighted).
% This function is released to the public domain; Any use is allowed.

maxtab = [];
mintab = [];

v = v(:); % Just in case this wasn't a proper vector

if nargin < 3
  x = (1:length(v))';
else 
  x = x(:);
  if length(v)~= length(x)
    error('Input vectors v and x must have same length');
  end
end
  
if (length(delta(:)))>1
  error('Input argument DELTA must be a scalar');
end

if delta <= 0
  error('Input argument DELTA must be positive');
end

mn = Inf; mx = -Inf;
mnpos = NaN; mxpos = NaN;

lookformax = 1;

for i=1:length(v)
  this = v(i);
  if this > mx, mx = this; mxpos = x(i); end
  if this < mn, mn = this; mnpos = x(i); end
  
  if lookformax
    if this < mx-delta
      maxtab = [maxtab ; mxpos mx];
      mn = this; mnpos = x(i);
      lookformax = 0;
    end  
  else
    if this > mn+delta
      mintab = [mintab ; mnpos mn];
      mx = this; mxpos = x(i);
      lookformax = 1;
    end
  end
end
    end

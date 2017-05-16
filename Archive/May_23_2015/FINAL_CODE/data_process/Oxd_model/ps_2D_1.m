
    %% Read temp2D.datfile for file       
      times =2000;
      filename ='YX';
      Figure ='xy_osc_oxd_4800_512';
      deltat = 0.001;%%input('Please enter the time_sep : ');
      Tsub = 0.2034;%%input('Please enter the background tempreture : ');    

     %% filenumber=counter_run;      
      A2=num2str(deltat,'%.5f')
      A3=num2str(Tsub,'%.6f')
      %%A5 =single(Tsub);
      A1=filename;              
      formatSpec = ' %1$s_%2$s_%3$s';       
      str = sprintf(formatSpec,A1,A2,A3);
      str; 
      File=[str,'.dat'];      
     %% destination (if you would like to plot files in ather folder) : 
      %folder='/Users/hosseinazizi/Documents/programms/simulation_results/serial/provatas/2d_code/non_dimensional/results/perpend_corr_length/8000x80_0.3317_0.001/';

      folder='/gs/scratch/hophy/DATA_ANALYSIS/Oxd_model/4800_512_0.001_1_1_0.8/';
      file_r=[File];           
      file_r=strtrim(file_r);
      file_r_path=strcat(folder,file_r);      
      file_temp= file_r_path;                       
      disp(file_temp);              
      fileID_r = fopen(file_temp,'r');  
      array = fscanf(fileID_r,'%d %d %f',[3 inf]); % It has two rows now.
      array = array';
      fclose(fileID_r);
      
      %% finding the max and min of temp , X and Y
       max_array=zeros(1,3);
       min_array=zeros(1,3);
       max_array(:)=max(array,[],1);
       min_array(:)=min(array,[],1);
       x_max=max_array(2);
       x_min=min_array(2);       
       y_max=max_array(1);
       y_min=min_array(1);       
       T_max=max_array(3);
       T_min=min_array(3);
       
      %% seting up the params for Surf plot
       xdim=x_min:x_max;
       ydim=y_min:y_max;
       x_min
       x_max
       y_min
       y_max
       [X,Y]=meshgrid(xdim,ydim);
      
   %% Define functions    
   
       Yaxis=zeros(y_max*times,1);
       Xaxis=zeros(y_max*times,1);
       combustion=zeros(y_max,x_max);
       
     %% stor in 2D temperature array
     for a=1:y_max*times                    
         Yaxis(a,1)=array(a,1);
         Xaxis(a,1)=array(a,2);
         combustion(array(a,1),array(a,2))=array(a,3);
     end
            
   %% Generate plot
      
       h=figure(1);                
       for i=1:y_max*times
        plot(Xaxis(i,1),Yaxis(i,1),'b');
        hold on;
       end       
       hold off;
    
   %% Set parameters
    
       Title  = title ('2D spatiotemporal oscillations');
       XLabel = xlabel('Length (m)');
       YLabel = ylabel('Width (m)'); 
       %axis([x_min x_max y_min y_max])
       ylim([y_min y_max])
   %% Creat filename to save figure     

      %%F1='xy_osc_14400_512_0.001';  
      F1=Figure; 
      F2=num2str(deltat,'%.5f')
      F3=num2str(Tsub,'%.6f')
      formatfig = ' %1$s_%2$s_%3$s';       
      strfig = sprintf(formatfig,F1,F2,F3);
      strfig; 
      fig_name=[strfig,'.png'];  
       fig_w=[fig_name];           
       fig_w=strtrim(fig_w);
       fig_w_path=strcat(folder,fig_w);                    
       saveas(h,fig_w_path,'png')
       %%hgexport(gcf, fig_w_path, hgexport('factorystyle'), 'Format', 'png');
       %% set camera angle

       az=0;
       e1=90;  
             
       figure (2)               
       set(gcf,'PaperPositionMode','auto')
       set(gcf, 'Color', 'w');           
       colormap jet
       W=surf(1:x_max,ydim,combustion(:,:));       
       set(W, 'edgecolor','none')
       view(az,e1);
       Title  = title ('Tempreture  spatial  oscillations');
       XLabel = xlabel('Length (m)');
       YLabel = ylabel('Width (m)'); 


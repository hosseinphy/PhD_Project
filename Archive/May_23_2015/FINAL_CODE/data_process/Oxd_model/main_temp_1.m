   %% code input
    prompt   = 'Please enter the following parameters value? '     
    N=512;
    filename='profile';
    filenumber = 1;%% input('Please enter the number of the file : ');
    deltat =0.001;%%  input('Please enter the time_step : ');
    Tsub = 0.2034;%% input('Please enter the background tempreture : ');  
    file_start_time =56000;%%  input('# of file start calucltion from : ');
    file_end_time = 60000;%% input('# of file end the calulation at : ');

    duration=file_end_time -file_start_time;
   
  %% filenumber=counter_run; 
    A3 = cell(duration,1);
    A2=num2str(filenumber);
    A4=num2str(deltat,'%.5f')
    A5=num2str(Tsub,'%.6f')
    %%A5 =single(Ts:ub);
    A1=filename;        
    file=cell(duration,1);  

  %% destination (if you would like to plot files in ather folder) : 
    folder='/gs/scratch/hophy/DATA_ANALYSIS/Oxd_model/4800_512_0.001_1_1_0.8/';
    
  %% Parameter Definition
    Row   =zeros(N,duration);
    Col    =zeros(N,duration);
    Temp=zeros(N,duration);   
    M=[];
    
   %% Load files  
      counter=file_start_time;
      m=0;
       while(counter<file_end_time)          
        A3{counter+1} = int2str(counter);%%[counter num2str(counter,0)];
        formatSpec = ' %1$s%2$s_%3$s_%4$s_%5$s';
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
          Row  (:,(counter-file_start_time)+1)= M(1+m:N+m,1);           
          Col    (:,(counter-file_start_time)+1)= M(1+m:N+m,2);
          Temp(:,(counter-file_start_time)+1)= M(1+m:N+m,3);    
          m=m+N;
         counter=counter+1;
       end

%% Comment out following lines       
 if(0)                 
   %% Setup a plot for Y VS T_max positions (X's)        
    figure(1)    
     %%clr = lines(2000);    %# LINES colormap    
       for i=1:duration        
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
       x_max=max_M(2);
       x_min=min_M(2);       
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
       
       for a=1:y_max*duration                    
         combustion(M(a,1),M(a,2))=M(a,3);                                     
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
       B1='YX';   
       B2=num2str(deltat,'%.5f');
       B3=num2str(Tsub,'%.6f')
       formatSpec_2 = ' %1$s_%2$s_%3$s';
       str_2 = sprintf(formatSpec_2,B1,B2,B3);
       str_2; 
       file_w=[str_2,'.dat'];              
       %%file_w=['YX_', int2str(counter_run), '.dat'];
       %%HHHHH%% file_w=['YX.dat'];           
       file_w=strtrim(file_w);
       file_w_path_1=strcat(folder,file_w);      
       file_temp_1= file_w_path_1;                       
       disp(file_temp_1); 
       fileID_1 = fopen(file_temp_1,'w');
       for i=1:duration
          for j=1:N
            fprintf(fileID_1,'%d %d %f \n',Row(j,i),Col(j,i),Temp(j,i));
          end
       end           
       fclose(fileID_1);
       
       %% *******************
       C1='temp2D';  
       C2=num2str(deltat,'%.5f')
       C3=num2str(Tsub,'%.6f')
       formatSpec_3 = ' %1$s_%2$s_%3$s';
       str_3 = sprintf(formatSpec_3,C1,C2,C3);
       str_3; 
       file_w=[str_3,'.dat'];  
       %%file_w=['temp2D_', int2str(counter_run), '.dat'];        
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


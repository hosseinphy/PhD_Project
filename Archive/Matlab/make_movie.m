function [M] = make_movie(x,y,n,filename)

x=4800;
y=300;
n=700;
filename='test8.avi';
%filename='front_temperature_2d_Siva_continuum_with_OXG_1.avi';

[X,Y]=meshgrid(1:x,1:y);

   % set(gca, 'nextplot', 'replacechildren');  
   % caxis manual;          % allow subsequent plots to use the same color limits
   % caxis([0.01 1.9]);         % set the color axis scaling to your min and max color limits

counter = 0;
combustion=zeros(x,y);
avicombustion=avifile(filename,'Compression','None','fps',1,'quality',100);

while (counter < n)
    
    file=['temperature_regular_2D_Siva_0_0.05_0.750_', int2str(counter), '.dat'];
    disp(file)
    
    try
        array=load(file);
    catch exception
        disp('wait')
        continue;
    end
    
    for a=1:x
        for b=1:y            
                combustion(a,b)=array((a-1)*y+b);           
        end
    end

    
    xdim=1:x;
    ydim=1:y;
       
    az=114;
    e1=20;
  
    hfig=figure('Position',[200 200 1280 720]);
    %%figure('Renderer','zbuffer');
     s=surf(ydim,xdim,combustion(:,:));
    view (az,e1);
    set(s,'EdgeColor','None');
    set(gcf,'Visible','off');
      
    %axis([0 40 0 500 ])
    colorbar
    %caxis([0.01 1.5])
    %axis([0 300 0 4800]);
    colormap default
    set(gca, 'clim', [0.01 1.5]);
    %F=getframe(hfig);
    avicombustion=addframe(avicombustion,hfig);
    close;      
    counter=counter+10;
    clear combustion;
    
end
%close(F)
avicombustion=close(avicombustion);
end
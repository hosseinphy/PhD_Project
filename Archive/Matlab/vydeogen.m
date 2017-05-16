function [M] = vydeogen(x,y,n,filename)

x=4800;
y=300;
n=10;
filename='test8.avi';
%filename='front_temperature_2d_Siva_continuum_with_OXG_1.avi';

[X,Y]=meshgrid(1:x,1:y);

   % set(gca, 'nextplot', 'replacechildren');  
   % caxis manual;          % allow subsequent plots to use the same color limits
   % caxis([0.01 1.9]);         % set the color axis scaling to your min and max color limits

counter = 0;
combustion=zeros(x,y,n);
    az=114;
    e1=20;
     xdim=1:x;
    ydim=1:y; 

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
                combustion(a,b,counter+1)=array((a-1)*y+b);           
        end
    end

     counter=counter+1;
    %%clear combustion;
end
    
     
   
%% new code

%f=figure;
f=figure('Position',[200 200 1280 720]);
set(f,'Visible', 'off');%'animating in background
a=gca;
%set(a,'DataAspectRatio',[300 10 1])
daspect('manual')
colorbar
colormap default
axis manual
hold on
ylabel('y');
title('title');
writerObj = VideoWriter(filename,'Motion JPEG AVI');
writerObj.Quality = 100;
writerObj.FrameRate = 1;%frames per sec
open(writerObj);    
for j = 1:n
    cla(a);
    axis([0 300 0 4800  0 1.9])%useful to specify sometimes    
    %%PLOT YOUR STUFF NOW
    s=surf(ydim,xdim,combustion(:,:,j));
    view (az,e1);
    %DYNAMIC_LABEL = sprintf('%s\n%s',LABEL,FOOTNOTE)
    %xlabel(DYNAMIC_LABEL)%I like to use this as a changing footnote
    colorbar    
    set(a, 'clim', [0.01 1.5]);
    drawnow;
    frame = getframe(f);
    writeVideo(writerObj,frame);   
end
close(f)
daspect('auto')
close(writerObj);
end

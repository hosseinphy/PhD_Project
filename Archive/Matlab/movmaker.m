function [M] = movmaker(x,y,n,filename)

% Takes input files of format filename1, filename2, etc, with x x-values, y
% y-values, and z z-values for each of the n files that the combustion
% model spits out

x=4800;
y=300;
n=704;
%%filename='test.avi';
filename='front_temperature_2d_Siva_continuum_with_OXG_1.avi';

[X,Y]=meshgrid(1:x,1:y);


set(gca, 'nextplot', 'replacechildren');  
caxis manual;          % allow subsequent plots to use the same color limits
caxis([0.34 1.3]);         % set the color axis scaling to your min and max color limits

    
counter =0;

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
    
    %G=combustion(5,5);
    %disp(G);
    xdim=1:x;
    ydim=1:y;    
     %az=120;   
     %e1=40; 
    az=0;
    e1=90;
   
   % figure('Renderer','zbuffer');
    figure('Position',[200 200 1280 720]);
    set(gcf,'PaperPositionMode','auto')
    set(gcf, 'Color', 'w');
    set(gcf,'Visible','off');
    s=surf(ydim,xdim,combustion(:,:));
    set( gca, 'TickDir', 'out');
	set(gca, 'Linewidth', 1.0);
	xlabel('Y ', 'LineWidth', 2, 'FontName', 'Helvetica',...
        'FontSize', 24);
    ylabel('X  - Propagation Direction', 'LineWidth', 2, 'FontName', 'Helvetica',...
        'FontSize', 24);
	%text( 1, 1.4, 1, sprintf('t = %.0f sec',counter*1.3), 'LineWidth', 2,...
    %    'FontName', 'Helvetica', 'FontSize', 24, 'BackgroundColor', 'white')
    view(az,e1);
    %set(s,'EdgeColor','None');
    
    %%NEW code
      axis off;
      
      set(gcf,'Visible','off');
      %pcolor(xdim,ydim,combustion(:,:));
      shading interp;
      colorbar;  % add the colorbar
      F(counter+1) = getframe(gcf); % capture the complete figure which includes the colorbar
      lighting phong;
       avicombustion=addframe(avicombustion,F(counter+1));
    
    %colormap default
    %colorbar 
    %%%%caxis([cmin cmax])
   axis([1 y 1 x])
   
   % avicombustion=addframe(avicombustion,gcf);
    close;     
    
    counter=counter+10;
    clear combustion;
    
end
avicombustion=close(avicombustion);

end



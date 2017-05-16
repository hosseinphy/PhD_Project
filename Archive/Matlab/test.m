function [M] = make_movie(x,y,n,filename)
% Takes input files of format filename1, filename2, etc, with x x-values, y
% y-values, and z z-values for each of the n files that the combustion
% model spits out
x=4800;
y=300;
n=40;
filename='test.avi';

[X,Y]=meshgrid(1:x,1:y);

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
       
    az=90;
    e1=90;
   figure('Position',[200 200 1280 720]);
    %%figure('Renderer','zbuffer');
     s=surf(ydim,xdim,combustion(:,:));
       view (az,e1);
    set(s,'EdgeColor','None');
    set(gcf,'Visible','off');
     %axis([0 40 0 500 ])
    colorbar
    axis([0 300 0 4800 0 3]);
    colormap default
    avicombustion=addframe(avicombustion,gcf);
    close;     
    
    counter=counter+10;
    clear combustion;
end
avicombustion=close(avicombustion);

end



media = zeros(1201,12);
media_t = zeros(1201,12);
for i=1:12

    suma=zeros(1200,1);
    suma_t=zeros(1200,1);
    #tiempo=[];
    for j=1:10
        name=sprintf('%dV %d',i,j);
        disp(name);
        v = load(name);
        suma=suma+v(:,2);
        suma_t= suma_t+v(:,1);
        
    end
    media(:,i) = [0;suma./10];
    media_t(:,i) = [0;suma_t./10];
    #tiempo = 0:1200;
    name_output=sprintf('PololuModeladoDat/trap%dV_0ms600ms600ms_T1ms_ST.mean',i);
    dlmwrite(name_output, [media_t(:,i) media(:,i)], 'delimiter',' ', 'precision', '%.6f')
    #dlmwrite(name_output, [tiempo' media(:,i)], 'delimiter',' ', 'precision', '%.6f')
end
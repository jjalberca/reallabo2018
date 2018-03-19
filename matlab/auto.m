
for i=1:12

    suma=zeros(1200,1);
    tiempo = [];
    for j=1:10
        name=sprintf('%dV %d',i,j);
        disp(name);
        v = load(name);
        suma=suma+v(:,2);
        tiempo=v(:,1);
    end

    media = suma./10;
    name_output=sprintf('PololuModeladoDat/trap%dV_0ms600ms600ms_T1ms_ST.mean',i);
    dlmwrite(name_output, [tiempo media], 'delimiter',' ', 'precision', '%.6f')
end
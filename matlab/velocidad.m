pasos=50;
v=zeros(1200,1);
velocidad1=[];
for i=1:10
    for j=pasos+1:1200
        v(j)=(resultado1(j,i)-resultado1(j-pasos,i))./(0.001*pasos);
    end
    velocidad1=[velocidad1 v];
end

v=zeros(1200,1);
velocidad2=[];
for i=1:10
    for j=pasos+1:1200
        v(j)=(resultado2(j,i)-resultado2(j-pasos,i))./(0.001*pasos);
    end
    velocidad2=[velocidad2 v];
end

v=zeros(1200,1);
velocidad3=[];
for i=1:10
    for j=pasos+1:1200
        v(j)=(resultado3(j,i)-resultado3(j-pasos,i))./(0.001*pasos);
    end
    velocidad3=[velocidad3 v];
end

v=zeros(1200,1);
velocidad4=[];
for i=1:10
    for j=pasos+1:1200
        v(j)=(resultado4(j,i)-resultado4(j-pasos,i))./(0.001*pasos);
    end
    velocidad4=[velocidad4 v];
end

v=zeros(1200,1);
velocidad5=[];
for i=1:10
    for j=pasos+1:1200
        v(j)=(resultado5(j,i)-resultado5(j-pasos,i))./(0.001*pasos);
    end
    velocidad5=[velocidad5 v];
end

v=zeros(1200,1);
velocidad6=[];
for i=1:10
    for j=pasos+1:1200
        v(j)=(resultado6(j,i)-resultado6(j-pasos,i))./(0.001*pasos);
    end
    velocidad6=[velocidad6 v];
end

v=zeros(1200,1);
velocidad7=[];
for i=1:10
    for j=pasos+1:1200
        v(j)=(resultado7(j,i)-resultado7(j-pasos,i))./(0.001*pasos);
    end
    velocidad7=[velocidad7 v];
end

v=zeros(1200,1);
velocidad8=[];
for i=1:10
    for j=pasos+1:1200
        v(j)=(resultado8(j,i)-resultado8(j-pasos,i))./(0.001*pasos);
    end
    velocidad8=[velocidad8 v];
end

v=zeros(1200,1);
velocidad9=[];
for i=1:10
    for j=pasos+1:1200
        v(j)=(resultado9(j,i)-resultado9(j-pasos,i))./(0.001*pasos);
    end
    velocidad9=[velocidad9 v];
end
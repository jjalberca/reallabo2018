suma=zeros(1200,1);

for i=1:10
    name=sprintf('1V %d',i);
    disp(name);
    v = load(name);
    suma=suma+v(:,2);
end

media1 = suma./10;

suma=zeros(1200,1);

for i=1:10
    name=sprintf('2V %d',i);
    disp(name);
    v = load(name);
    suma=suma+v(:,2);
end

media2 = suma./10;

for i=1:10
    name=sprintf('3V %d',i);
    disp(name);
    v = load(name);
    suma=suma+v(:,2);
end

media3 = suma./10;

for i=1:10
    name=sprintf('4V %d',i);
    disp(name);
    v = load(name);
    suma=suma+v(:,2);
end

media4 = suma./10;

for i=1:10
    name=sprintf('5V %d',i);
    disp(name);
    v = load(name);
    suma=suma+v(:,2);
end

media5 = suma./10;

for i=1:10
    name=sprintf('6V %d',i);
    disp(name);
    v = load(name);
    suma=suma+v(:,2);
end

media6 = suma./10;

for i=1:10
    name=sprintf('7V %d',i);
    disp(name);
    v = load(name);
    suma=suma+v(:,2);
end

media7 = suma./10;

for i=1:10
    name=sprintf('8V %d',i);
    disp(name);
    v = load(name);
    suma=suma+v(:,2);
end

media8 = suma./10;

for i=1:10
    name=sprintf('9V %d',i);
    disp(name);
    v = load(name);
    suma=suma+v(:,2);
end

media9 = suma./10;
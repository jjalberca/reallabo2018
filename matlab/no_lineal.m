Veq1=zeros(1,1001);
for i=1:680
    x1=(i-1)*12/1000;
    [distancia, index] = min(abs(x-x1));
    Veq1(i) = Veq(index);
end

for i=681:1001
    x1=(i-1)*12/1000;
    Veq1(i) = (x1);
end

Veq1=Veq1*1000/12;
Veq1=round(Veq1);

for i=1:10:1000
   disp(sprintf('%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,', Veq1(i), Veq1(i+1), Veq1(i+2), Veq1(i+3), Veq1(i+4), Veq1(i+5), Veq1(i+6), Veq1(i+7), Veq1(i+8), Veq1(i+9)));
end
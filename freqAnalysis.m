%%
A=load('measurements.txt');
R=load('commands.txt')
n=[];
for q=R(6:10,1)'
    
z=abs(A(:,1)-q);
n=[n find(min(z)==z)];
end
n
C=unwrap(2*pi*A(:,9)/255);%A(:,1);%
figure(1)
hold on
%plot(gradient(C),'g');
%plot(R(:,1)/1000,0,'o')
%plot(A(:,1)/1000,360/2/pi*C);
%find(gradient(C)>0.05)
%m=[1313,1469,1656,1777];
%n=[449,2002, 2128, 2261,2430];
%%
M=[];
before=20;
after=50;
size(C)
for i=n
M=[ C((i-before):(i+after))-C(i-before) M ];
end
q=((1:(before+after+1))'-1>before)*315;

M=[q 360/2/pi*M(:,1:end) ];
M(25:end,5)=M(25:end,5)+365
%M(:,2:end)

plot((A(100:(99+before+after+1),1)-A(100+before,1))/1000,M(:,1:end))%
%(A(100:(99+before+after+1),1)-A(100+before,1))/1000,
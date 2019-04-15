clear
fileID = fopen('result.txt','r');
formatSpec = '%f';
A = fscanf(fileID,formatSpec);

m=A;

for i=2:size(A)
    m(i)=((m(i-1)*(i-1))+A(i) )/i;
end 
 figure
% 
%  hist(A,30);
% figure (2)
plot(A);
hold on
plot(m,'r','LineWidth',3);
max(A)
function [ modularity ] = Modularity(n,r,C,graph )
%Modularity Summary of this function goes here
%   
%   
%  
Qtemp=0;
for i=1:n
    for j=1:n
        delta = 0;
        if (C(i) == C(j))
            delta = 1;
        end
        Qtemp = Qtemp + (graph(i,j) - (r(i)*r(j)/sum(r,2))) * delta;
    end
end

modularity = (1/sum(r,2))*(Qtemp);

end


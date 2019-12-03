function [ result ] = Check_Quality( Q, Qbest, n, A, C ) %,r,C,graph
%Check_Quality Summary of this function goes here
%   
%   
%   
    if ( Q < Qbest )
        beta(1:n) = 1;
    else
        for  i = 1 : n  
            neighborscomm{i} = C(A{i});
            icomm = C(i);
           % InternalEdges = sum(neighborscomm{i} == icomm);
        
            %check_Raghavan_def()
            if( icomm == mode(neighborscomm{i},2) )
                 beta(i) = 0; % 0 is for Reward
            else
                 beta(i) = 1; % 1 is for ponishment
            end
             
        end
    end
    result = beta;
end


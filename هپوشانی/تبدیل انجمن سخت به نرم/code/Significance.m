function [ sig ] = Significance(n,C,graph )
%Significance Summary of this function goes here
%   
%   
%  


template = graph;
e = sum(sum(graph))/2;
p = e/n*(n-1)
[Csorted is] = sort(C,'ascend');
co(:,1) = is;
co(:,2) = Csorted;


for i = 1 : n
    for j = 1 : n
        if (C(i)~=C(j))
            template(i,j) = 0;
            template(j,i) = 0;
        end
    end
end

commNum = max(C); 
for c = 1 : commNum
    com{c} = co(co(:,2) == c);
end
sig = 0;
% first = com{1}
% com{2}
% sec = sum(template(first,:),2)
% ec = sum(sec)
%ec = sum(template(com{1}))
    pc(1:n,1:n) = 0;
for i = 1 : commNum
    nc = length(com{i});
    ec = sum(sum(template(com{i},:),2))/2;
    pc = ec/nc*(nc-1)
    %template(com{i},:)
    %ec/nc;
    size(p);
    size(pc);
    D = KLD(pc,p)
    sig = sig+ nc*(nc-1)/2*sum(D);
end

end

function [D] = KLD(q,p)

D = sum(q.*log2(q./p))+((1-q).*log2((1-q)./(1-p)));
end
% 
% function [dist]=KLD(P,Q)
% %  dist = KLDiv(P,Q) Kullback-Leibler divergence of two discrete probability
% %  distributions
% %  P and Q  are automatically normalised to have the sum of one on rows
% % have the length of one at each 
% % P =  n x nbins
% % Q =  1 x nbins or n x nbins(one to one)
% % dist = n x 1
% 
% 
% 
% if size(P,2)~=size(Q,2)
%     error('the number of columns in P and Q should be the same');
% end
% 
% if sum(~isfinite(P(:))) + sum(~isfinite(Q(:)))
%    error('the inputs contain non-finite values!') 
% end
% 
% % normalizing the P and Q
% if size(Q,1)==1
%     Q = Q ./sum(Q);
%     P = P ./repmat(sum(P,2),[1 size(P,2)]);
%     temp =  P.*log(P./repmat(Q,[size(P,1) 1]));
%     temp(isnan(temp))=0;% resolving the case when P(i)==0
%     dist = sum(temp,2);
%     
%     
% elseif size(Q,1)==size(P,1)
%     
%     Q = Q ./repmat(sum(Q,2),[1 size(Q,2)]);
%     P = P ./repmat(sum(P,2),[1 size(P,2)]);
%     temp =  P.*log(P./Q);
%     temp(isnan(temp))=0; % resolving the case when P(i)==0
%     dist = sum(temp,2);
% end
% end
% 

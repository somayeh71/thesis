function [ componentList state] = runDFS(node,state,n,S,compno,comp )
%runDFS Summary of this function goes here
%   
%   
%  

state(node) = 1 ; % 1 is Gray
for j = 1 : n
    if ( S(node,j) == 1 && state(j) == 0 )
        [comp state]= runDFS(j,state,n,S,compno,comp);
    end
    state(node) = 2; % 2 is Black
    comp(node) = compno;
end
componentList = comp;
state = state;
end
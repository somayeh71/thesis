%=========================================================================%
                %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
                  % Data Preprocessing Phase %  
                %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%=========================================================================%
clear all
Q_Av = 0;
t_av = 0;


global n
global r;
global C ;
bestQuality = 0;
nmiBest = 0;
% Adjacency Matrix
fileName = 'karate.gml';
 adjmatrix = read_gml(fileName); % get adjacency matrix from input


% adjmatrix = importdata('network.txt'); % input
% realcommunity = importdata('community.txt'); % input

graph = generate_graph(adjmatrix);
size(graph)

% 
% % proving the limitation free
% graph(1:54,1:54) = 0;
% graph(1:50,1:50) = 1;
% graph(51:54,51:54) = 1;
% for i=1:54
%     graph(i,i)=0;
% end
% 
%     

n = size(graph,1); % n is the number of nodes in graph

r = sum(graph,2)';  % r is the num of neighbors for each node (deg of each node)

% N(i) >> Neighberhood List for each node [list of actions for each cell]
A=cell(n,1);
for i=1:n
    temp_vec = [];
    for j=1:n
        if( graph(i,j)==1 )
             temp_vec = [temp_vec j];
        end
    end
    A{i} = temp_vec;
end

%=========================================================================%
                %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
                    % Initialization Phase %     
                %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%=========================================================================%
%for q = 1:10
h=0;
a = 0.01;
Qbest = 0;


p=cell(n,1); %p(i) is the probability of selecting each neighbers of node i
for i =1:n
    p{i}(1:r(i))=1./r(i);
end

C = [];
% First each node is a community itself
for i = 1:n
    C  = [C [i;i]];
end

%initialz Z,W,D
W=cell(n,1);
Z=cell(n,1);
D=cell(n,1);
for i=1:n
    W{i}(1:r(i)) = 0;
    Z{i}(1:r(i)) = 0;
    D{i}(1:r(i)) = 0;
end

%=========================================================================%
                %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
                        % CLA-net Phase %            
                %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%=========================================================================%
% 

t = 1;
while (true)
    
    % Choose action based on probability
    S = []; % S -> Solution Vector
    for i=1:n
        if ( t > 5 )
            alpha = datasample(A{i},1,'Weights',p{i}); % select an action 
        else
            alpha=A{i}(randi(numel(A{i}))); %Repeat randomly a small number of times to initialz Z,W,D
        end
        S = [S alpha];
    end
   
    % Decoding solution vector(S) to membership vector(C)
    C_old = C;
        
    G(1:n,1:n)=0;
    for i = 1 : n
        G(i,S(i)) = 1;
        G(S(i),i) = 1;
    end
    
    state(1:n) = 0; %White
    compno = 0; % number of components in the graph
    comp(1:n) = 0;
    for i = 1 : n
        if (state(i) == 0)
            compno = compno + 1;
            [comp, state] = runDFS(i,state,n,G,compno,comp);
        end
    end
    C = comp;
    sig = Significance(n,C,graph);

    % Calculate quality of this community stracture
    Q = Modularity(n,r,C,graph);
    
    % Check the local and global quality
    beta = Check_Quality( Q , Qbest, n, A, C );

    % Update modularity
   
    if (Qbest < Q)
         Qbest = Q;
         obtaincommunity(:,1) = (1:n);
         obtaincommunity(:,2) = C;
%          nmi_value = nmi(realcommunity, obtaincommunity);
    end
    
     % update W and Z
    for i = 1 : n
        aq = S(i);
        indx = find(A{i}==aq);
        W{i}(indx) = W{i}(indx)+ (1-beta(i));
        Z{i}(indx) = Z{i}(indx) + 1;
    end

   
    %Estimate the optimal action
    for i = 1 : n
        D{i} = W{i}./Z{i};
        optimalAct(i) = max(D{i}); 
        temp = find( D{i} == optimalAct(i) );
        optimalActInx(i) = temp(randi(numel(temp)));   
    end
    
   
    %Update the action probability vector P of each LA
    for i = 1 : n
        for j = 1 : r(i)
            if (j == optimalActInx(i))
                p{i}(j) = p{i}(j) + a * (1 - p{i}(j));
            else
                p{i}(j) = (1 - a) * p{i}(j);
            end
        end
    end
 
    
    % Check if the community structure remains fixed
     if ( isequal(C,C_old) )
         h=h+1;          
     else
         h=0;
     end
     
     if ( h>2 || t>1000 )
         break;
     end    
   
t = t + 1;
 
end

 Qbest
%  nmi_value
 C
% 
%  Q_Av = Qbest + Q_Av;
%  t_av = t + t_av;
%  
%  if (nmiBest < nmi_value)
%      nmiBest = nmi_value;
%      Q_bestnmi = Qbest;
%  end
%  
%  if (bestQuality < Qbest)
%      bestQuality = Qbest; 
%      Cbest = C;
%      NmiForBestQ = nmi_value;
%  end
% t

% end
%  Q_Av = Q_Av/10
%  t_av = t_av/10
%  bestQuality
%  Cbest
%  NmiForBestQ
%  
%  
%  nmiBest
%  Q_bestnmi
 

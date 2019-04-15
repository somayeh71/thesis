clc 
close all
clear all

l1  =   0.1 ;                   % (a)first  learning  parameter[reward]
l2  =   0.001 ;                 % second learning  parameter[penalty]
beta=   [0 1];                  % reinforcement parameter

adj_matrix = importdata('ml_karate_adjmat.txt'); % input
node_count = size(adj_matrix,2); % count oflearning automaton
degrees = sum(adj_matrix, 2); % (ri)the number of the actions for learning automaton Li
ri = degrees';
membershipVec =[];              % membership vector
tic
for i = 1:node_count
    membershipVec  = [membershipVec [i;i]];
end
% membershipVec  = [(1:node_count);(1:node_count)];

temp = 0;
for i=1:node_count
    for j=1:node_count
        delta = 0;
        if (membershipVec(2,i) == membershipVec(2,j))
            delta = 1;
        end
        temp = temp + (adj_matrix(i,j) - (degrees(i)*degrees(j)/sum(ri,2))) * delta;
    end
end
 % The largest modularity obtained in the past cycles.;
Qbest = (1/sum(ri,2))*(temp/2);

% initialization probability vectors in a matrix___________________________
 PrMatrix = {};
for i = 1:node_count
    ac  =   ri(i) ;                 % actions count
    act =  (1:ac) ;                 % actions
    % the number of times the jth actions of learning automaton Li has been rewarded
    Wij = floor((20-1).*rand(ac,1) + 1)';
    % the number of times the jth actions of learning automaton Li has been choosen
    Zij = floor((40-20).*rand(ac,1) + 20)';
    Di = Wij./Zij;                  % estimate vector
    % connected nodes
    temp_vec = [];
    for k=1:node_count
        if (adj_matrix(i,k) == 1)
            temp_vec = [temp_vec k];
        end
    end
    % actions probabilities + Dis for CPrp
    actp = [(zeros(1,ac)+(1/ac));Wij;Zij;Di;Di;temp_vec];  
    PrMatrix = [PrMatrix actp];
end

% Repeat until convergence_________________________________________________
s = 1000;
while true
        if (s<=0) 
            break
        end    
        % Each learning automaton Li chooses an action according to its 
        % action probability vector pi
        sv = []; % solution vector
        for i = 1:node_count % one iteration for each automaton
            pk1 = PrMatrix{i};
            pk2 = pk1(1,:);
            temp_act = pk1(6,:);
            sa = datasample(temp_act,1,'Weights',pk2) ;% select an action 
            sv = [sv [i;sa]];
        end

        % import solution vector to membership vector 
        membershipVec_old = membershipVec;
        for i = 1:node_count
            temp1 = sv(2,i);
            
            temp2 = membershipVec(2,temp1);
            membershipVec(2,i) = temp2;
           
        end
        % calculate quality of community structure
        temp = 0;
        for i=1:node_count
            for j=1:node_count
                delta = 0;
                if (membershipVec(2,i) == membershipVec(2,j))
                    delta = 1;
                end
                temp = temp + (adj_matrix(i,j) - (degrees(i)*degrees(j)/sum(ri,2))) * delta;
            end
        end
         % The largest modularity obtained in the past cycles.;
        Q = (1/sum(ri,2))*(temp/2);
        % calculate success rate
        beta = []; % vector of success or fail of action selection
        for i=1:node_count
            check_dens = 0;
            temp1 = membershipVec(2,i);
            c = 1;
            cp = 0;
            temp_vec = [0;0];
            for k=1:node_count
                if (adj_matrix(i,k) == 1)
                    temp2 = membershipVec(2,k);
                    if (temp1 == temp2)
                        c = c + 1;
                    else
                        cp = cp + 1;
%                         temp_vec1 = temp_vec(1,:);
%                         if 0 ~= (numel(find(temp_vec1 == temp2)))
%                             temp = (find(temp_vec1 == temp2));
%                             temp_vec(2,temp) = temp_vec(2,temp) + 1;
%                         else
%                             temp_vec = [temp_vec [temp2;1]];
%                         end
                    end
                end
            end
%             temp_vec2 = temp_vec(2,:);
%             cp = max(temp_vec2);
            if c >= cp
                check_dens = 1;
            end
            if (Q >= Qbest) && (check_dens == 1)
                beta = [beta 1];
            else
                beta = [beta 0];
            end
        end
        % update maximum quality
        temp = sum(beta,2);
        if (Q >= Qbest) && (temp > 0)
            Qbest = Q;
        else
            membershipVec = membershipVec_old;
        end
        % update parameters
        for i=1:node_count
            pk1 = PrMatrix{i};
            pk2 = pk1(1,:);
            pk6 = pk1(6,:); % all connected nodes
            ac = size(pk1,2);
            connected_node = sv(2,i);
            % find index of connected node in this action
            action_idx = find(pk6 == connected_node);
            Wij_temp = pk1(2,action_idx)+beta(i);
            Zij_temp = pk1(3,action_idx)+1;
            pk1(4,action_idx) = pk1(5,action_idx);
            pk1(5,action_idx) = Wij_temp/Zij_temp;
            pk1(2,action_idx) = Wij_temp;
            pk1(3,action_idx) = Zij_temp;
            if pk1(4,action_idx) <= pk1(5,action_idx)
                pk2 = LReP(action_idx,beta(i),ac,pk2,l1,l2,1) ;
%                 pk2 = LReP_v2(action_idx,ac,pk2,l1) ;
                pk1 = [pk2;pk1(2:end,:)];
            end
            PrMatrix{i} = pk1 ;
        end
        s=s-1;
end
membershipVec
toc
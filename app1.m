clc; clear; close all;

% ------ Value Iteration for Gambler Problem - Example 4.3
% ------ Refer to Figure 4.3 - Page 68 to validate solutions

nState = 101; % s = 0 || s = 100 || s = 1:99
V = zeros(1, nState);  % state values
V(1) = 0; % losing the game, terminal state.
V(end) = 1; % winning the game, terminal state.
pi = zeros(1,99); % s = 0 & s= 100 are terminal
% pHead = 0.4; % probability of head (unfair game)
pH = [0.4,0.5,0.7];

figure;
hold on;
for phIndex = 1:numel(pH)
    pHead = pH(phIndex);
    subplot(2,numel(pH),phIndex);
    hold on;
    nIter = 50; 
    legendInfo = {};
    for iter = 1:nIter
        for sIndex = 2:100
            s = sIndex - 1; % converting index to the actual state (capital)
            action = 1:min(s, 100-s); % the actions are stakes available (from 1 to the largest allowed bet)
            nAct = numel(action);
            q = zeros(1, nAct);
            for a = 1:nAct
                % Bellman equation, averaging over possibilites
                q(a) =  pHead*V(sIndex+action(a)) + (1-pHead)*V(sIndex-action(a));
            end
            V(sIndex) = max(q);
            % selecting action with highest value for policy (greedy approach)
            aStar = find(q==max(q),1);
            pi(s) = action(aStar);
        end
        if(iter == 2 || iter == 3 || iter == 4 || iter == 5 || iter == 1)
            plot(0:100,V); xlabel('capital'); ylabel('value-function'); title("p-head = " + pHead);
            legendInfo{end+1} = ["iteration " + num2str(iter)];
        end
    end
    
    legend(legendInfo,'Location','southeast');
    subplot(2,numel(pH),phIndex+numel(pH));
    plot(1:99,pi); xlabel('capital'); ylabel('policy'); ylim([0,50]); title("p-head = " + pHead);
    % reset V and pi
    V = zeros(1, nState);  % state values
    V(1) = 0; % losing the game, terminal state.
    V(end) = 1; % winning the game, terminal state.
    pi = zeros(1,99); % s = 0 & s= 100 are terminal
end
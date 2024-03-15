A = load("network_A.mat").A;
%disp(A);

% initialize parameters
visited = zeros(1, 100);
queue = [];
tree = zeros(100, 100);

% root intial
visited(1) = 1;
queue(end+1) = 1;

% spanning tree algorithm
while(1)
    if(isempty(queue))
        break; % if all nodes are visited, done
    end
    i = queue(1); % pop queue
    queue(1) = [];
    % expanding this node
    for j = 1 : 100
        if(visited(j) == 0 && A(i, j) == 1)
            % spanning this edge
            queue(end+1) = j; % push next hop to queue
            tree(i, j) = 1;
            tree(j, i) = 1;
            visited(j) = 1; % set visited
        end
    end
end

% check the number of edges, it should be E = V - 1
count = 0;
for i = 1: 100
    for j = 1: 100
        if(tree(i,j) == 1)
            count = count + 1;
        end
    end
end
disp("Tree edges: ")
disp(count/2); % the result should be 99

% save result
save result.mat tree;


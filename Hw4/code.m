A = load("network_A.mat").A;
%A = load("input_test.mat").A;
%disp(A)

n = size(A, 1); % get the number of nodes
d = Inf(n, n); % set all edges by infite value to represent cost MAX
%disp(n)

% initialize d matrix
for i = 1: n
    for j = 1: n
        if(i == j) % pass node itself
            d(i, j) = 0;
            continue;
        end
        if(A(i, j) == 1) % set adjacency edges
            d(i, j) = 1;
        end
    end
end
%disp(d)

% shortest path routing algorithm
for k = 1: n
    for i = 1: n
        for j = 1: n
            % check whether passing through node k can get a smaller value
            d(i, j) = min(d(i, j), d(i, k) + d(k, j));
        end
    end
end

% check result
%disp(d)
%D = load("output_test.mat").d;
%disp(D)
%disp(isequal(D, d))

% save result
save result.mat d;

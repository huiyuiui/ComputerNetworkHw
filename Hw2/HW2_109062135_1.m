n= 12000;
k = 32;

Mx = load("inputdata.mat").packet;
Cx = [1 0 0 0 0 0 1 0 0 1 1 0 0 0 0 0 1 0 0 0 1 1 1 0 1 1 0 1 1 0 1 1 1];
Tx = cat(2, Mx, zeros(1,k));

MxLen = length(Mx);
CxLen = length(Cx);
TxLen = length(Tx);
diff = TxLen - CxLen;

Cx = cat(2, Cx, zeros(1, diff));
disp(length(Cx))

i=1;
while i<=MxLen
    if(Tx(i) == 0)
        continue
    end
    %fprintf("i = %i\n", i)
    %fprintf("old Tx:\n")
    %disp(Tx)
    %fprintf("new ")
    Tx = xor(Tx, Cx);
    if(all(Tx(:) == 0))
        break
    end
    if(i==MxLen)
        break
    end
    for j=i+1:1:i+k
        %fprintf("j = %i\n", j)
        if(Tx(j) == 1)
            shift = j-i;
            Cx = circshift(Cx, shift);
            i=j;
            break
        end
    end
end

Ex = Tx(n+1:n+k)
Px = cat(2, Mx, Ex);

codepacket = Px;
save 109062135.mat codepacket

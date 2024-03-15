n= 8
k = 3
Mx = [1 0 0 1 1 0 1 0]
Cx = [1 1 0 1]
Tx = cat(2, Mx, zeros(1,k))
Px = [1 0 0 1 1 0 1 1 0 0 0]
tx = [0 0 0 0 1 0 1 1 0 0 1]

MxLen = length(Mx)
CxLen = length(Cx)
TxLen = length(Tx)
PxLen = length(Px)
diff = PxLen - CxLen

Cx = cat(2, Cx, zeros(1, diff))
disp(length(Cx))
cx = [0 0 0 0 0 0 0 1 0 1 1]

i=1
while i<=MxLen
    if(Tx(i) == 0)
        continue
    end
    fprintf("i = %i\n", i)
    fprintf("old Tx:\n")
    disp(Tx)
    fprintf("new ")
    Tx = xor(Tx, Cx)
    if(all(Tx(:) == 0))
        break
    end
    if(i==MxLen)
        break
    end
    for j=i+1:1:i+k
        fprintf("j = %i\n", j)
        if(Tx(j) == 1)
            shift = j-i
            Cx = circshift(Cx, shift)
            i=j;
            break
        end
    end
end

Ex = Tx(n+1:n+k)
Px = cat(2, Mx, Ex)

i=1
while i<=MxLen
    if(tx(i) == 0)
        continue
    end
    tx = xor(tx, cx);
    if(all(tx(:) == 0))
        break
    end
    if(i==MxLen)
        break
    end
    for j=i+1:1:i+k
        if(tx(j) == 1)
            shift = j-i;
            cx = circshift(cx, shift);
            i=j;
            break
        end
    end
end

ex = tx(n+1:n+k)
EX = flip(ex)
EEX = cat(2, zeros(n, 0), EX)
PX = xor(Px, EEx)

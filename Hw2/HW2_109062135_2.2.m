message = load('109062135.mat');
codepacket = message.codepacket;

Cx = [1 0 0 0 0 0 1 0 0 1 1 0 0 0 0 0 1 0 0 0 1 1 1 0 1 1 0 1 1 0 1 1 1];

multiply = 1;
change_bits = sum(Cx);
while change_bits > 10
    multiply = multiply + 1;
    disp(multiply);
    mult_binary = de2bi(multiply, 'left-msb');
    Ex = gfconv(Cx, mult_binary);
    change_bits = sum(Ex);
end
disp(change_bits);
error = Ex
save 109062135.mat codepacket error;

codepacket = load('109062135.mat').codepacket;

Cx = [1 0 0 0 0 0 1 0 0 1 1 0 0 0 0 0 1 0 0 0 1 1 1 0 1 1 0 1 1 0 1 1 1];
error = Cx;
min = sum(Cx);

for i = 1 : 15
    %disp(i)
    for j = 1 : 50

        % initial 
        position = 1;
        Ex = Cx;

        % second 1
        for k = 1 : i
            shift = 1;
            idx = position + 1;
            while 1
                if(Ex(idx) == 1)
                    position = idx;
                    %disp(position)
                    break;
                end
                shift = shift + 1;
                idx = idx + 1;
            end
            Ex = cat(2, Ex, zeros(1, shift));
            intervalEnd = position + length(Cx);
            %disp(intervalEnd)
            Ex(position: intervalEnd-1) = xor(Ex(position: intervalEnd-1), Cx);
            if sum(Ex) < min
                min = sum(Ex);
                %disp(min)
            end
        end

        % third 1
        shift = 1;
        idx = position + 1;
        while 1
            if(Ex(idx) == 1)
                position = idx;
                %disp(position)
                break;
            end
            shift = shift + 1;
            idx = idx + 1;
        end
        Ex = cat(2, Ex, zeros(1, shift));
        for k = 1 : j
            shift = 1;
            idx = position + 1;
            while 1
                if(Ex(idx) == 1)
                    position = idx;
                    %disp(position)
                    break;
                end
                shift = shift + 1;
                idx = idx + 1;
            end
            Ex = cat(2, Ex, zeros(1, shift));
            intervalEnd = position + length(Cx);
            Ex(position: intervalEnd-1) = xor(Ex(position: intervalEnd-1), Cx);
            if sum(Ex) < min
                min = sum(Ex);
                %disp(min)
            end
        end

        % fourth 1
        shift = 1;
        idx = position + 1;
        while 1
            if(Ex(idx) == 1)
                position = idx;
                %disp(position)
                break;
            end
            shift = shift + 1;
            idx = idx + 1;
        end
        Ex = cat(2, Ex, zeros(1, shift));
        while length(Ex) < 12032
            shift = 1;
            idx = position + 1;
            while 1
                if(Ex(idx) == 1)
                    position = idx;
                    %disp(position)
                    break;
                end
                shift = shift + 1;
                idx = idx + 1;
            end
            Ex = cat(2, Ex, zeros(1, shift));
            intervalEnd = position + length(Cx);
            Ex(position: intervalEnd-1) = xor(Ex(position: intervalEnd-1), Cx);
            if sum(Ex) < min
                min = sum(Ex);
                %disp(min)
            end
        end     

        % finish
        if min == 4
            disp(min);
            error = cat(2, Ex, zeros(1, 12032-length(Ex)));
            disp(length(error))

            save 109062135.mat codepacket error;
            return;
        end
    end
end


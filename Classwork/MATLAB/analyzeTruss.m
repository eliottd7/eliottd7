% Import data
clear;
str = input('What is the name of the file?\n','s');
importRaw = importdata(str);
C = importRaw.C;
L = importRaw.L;
Sx = importRaw.Sx;
Sy = importRaw.Sy;
X = importRaw.X;
Y = importRaw.Y;

% Local variables
cost = 0;
[J, M] = size(C); % Calculate J and M from size of C

% Create distance matrix R
R = zeros(1, J); % Populate R
for at = 1:J % Each member
    for bt = 1:(at) % compared to each member
        if at == bt
            R(at,bt) = 0;
        else
            R(at,bt) = sqrt((X(bt)-X(at))^2 + (Y(bt)-Y(at))^2);
            R(bt,at) = R(at,bt); % Mirror along diagonal
        end
    end
end

% Create component matrix A
A = zeros(2*J, M+3);

% Populate last 3 columns with Sx and Sy data
A(1:J, M+1:end) = Sx;
A(J+1:2*J, M+1:end) = Sy;

% Populate first M columns with data
for mt = 1:M
    % X components ~~~~~~~~~~~~~~~~~~~~~~~~~~
    pair = [0 0]; flag = 0;
    for jt = 1:J
        if (C(jt,mt) > 0)
            if flag == 0
                flag = 1;
                pair(1) = jt;
            else % flag != 0 --> true
                pair(2) = jt;
                A(pair(1),mt) = (X(pair(2)) - X(pair(1)))/R(pair(1),pair(2));
                A(pair(2),mt) = (X(pair(1)) - X(pair(2)))/R(pair(1),pair(2));
                % Calculate cost by net sum of R distances, without duplicates
                cost = cost + R(pair(1),pair(2));
            end
        else
            C(jt,mt) = 0;
        end
    end
    % Y components ~~~~~~~~~~~~~~~~~~~~~~~~~~
    pair = [0 0]; flag = 0;
    for jt = 1:J
        if (C(jt,mt) > 0)
            if flag == 0
                flag = 1;
                pair(1) = jt;
            else % flag != 0 --> true
                pair(2) = jt;
                A(pair(1)+J,mt) = (Y(pair(2)) - Y(pair(1)))/R(pair(1),pair(2));
                A(pair(2)+J,mt) = (Y(pair(1)) - Y(pair(2)))/R(pair(1),pair(2));
            end
        else
            C(jt,mt) = 0;
        end
    end
end

T = A\L;

% Print to console nice and pretty
disp("EK301 Section A1. Eliott D. Emily M. Justin S., 6 April 2023.");
str = sprintf('Load: %0.3f oz', max(L)); disp(str);
disp("Member forces, in oz");

compressed = zeros(1,M); % for later compression calculation
lengths = zeros(1,M);
for at = 1:M
    if T(at,1) <= 0 % compression
        str = sprintf('M%d:\t%02.3f\t(C)', at, -1*T(at,1)); disp(str);

        % Calculate maximum compression members and values
        compressed(1,at) = at;
        pair = [0 0]; flag = 0; % Search for joints around max comp. member
        for jt = 1:J
            if (C(jt,at) > 0)
                if flag == 0
                    flag = 1;
                    pair(1) = jt;
                else % flag != 0 --> true
                    pair(2) = jt;
                    break;
                end
            end
        end
        lengths(1,at) = R(pair(1),pair(2));
    else % tension
        str = sprintf('M%d:\t%02.3f\t(T)', at, T(at,1)); disp(str);
    end
end

% calculate maximum load per member
maxCompression = lengths.^(-2.125); 
maxCompression = maxCompression.*(4338);

% Find critical member
diffs = maxCompression - abs(T(1:M,1)');
mindiff = find(diffs == min(diffs));

disp("Reaction forces, in oz");
fprintf('Sx1:\t%02.3f\n', T(M+1,1));
fprintf('Sy1:\t%02.3f\n', T(M+2,1));
fprintf('Sy2:\t%02.3f\n', T(M+3,1));

cost = cost + 10*J;
fprintf('Cost of truss: $%6.2f\n', cost);
fprintf('Theoretical max load/cost ratio in oz/$: %0.4f\n\n', (max(L)/cost));

disp("Compression member analysis.");
disp("* means load will cause buckle. # means critical member.");
for mt = 1:M
    if lengths(1,mt) > 0
        if maxCompression(1,mt) < -1*T(mt,1) % Too much load
            fprintf('*');
        else
            fprintf(' ');
        end
        if mt == mindiff
            fprintf('#');
        else
            fprintf(' ');
        end
        fprintf('Member %.f\tlength %02.3f\tmax %03.3f oz\tfeels %03.3f oz\n', mt, lengths(1,mt), maxCompression(1,mt), -1*T(mt,1));
        
    end
end


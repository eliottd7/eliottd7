%Nearest neighbor training error rate is 0%.
%Nearest neighbor test error rate is 16%.

%This function takes in a training data matrix Xtrain and training
%label vector ytrain. It also takes in a data matrix Xrun and 
%produces a vector of label guesses yguess. Each guess is found
%by searching through Xtrain to find the closest row, and then 
%outputting its label.
function yguess = nearest_neighbor(Xtrain,ytrain,Xrun)

    %pre-defining vectors and sizes
    [rowsTrain, ~] = size(Xtrain);
    [rowsTest, ~] = size(Xrun);
    yguess = zeros(rowsTest,1);
    
    for i=1:rowsTest %cycle through testing matrix
        distance = zeros(rowsTrain, 1); %create/clear distance vector
        
        for j = 1:rowsTrain %cycle through training matrix
            distance(j) = sqrt(sum(((Xrun(i,:) - Xtrain(j,:)).^2))); %find distance between testing and training
        end
        [~, mindex] = min(distance);
        yguess(i) = ytrain(mindex); %sets flag to closest neighbor
    end
    
%Your code should go above this line.
if (~iscolumn(yguess))
    error("yguess is not a column vector.")
elseif (length(yguess)~=size(Xrun,1))
    error("Length of yguess is not equal to the number of rows in Xrun.")
elseif (sum(unique(abs(ytrain))~=1))
    warning("Some elements in ytrain are not +1 or -1.")
elseif (sum(unique(abs(yguess))~=1))
    warning("Some elements in yguess are not +1 or -1.")
end
end

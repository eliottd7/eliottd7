%Closest average training error rate is 20%.
%Closest average test error rate is 18%.

%This function takes in a training data matrix Xtrain, training
%label vector ytrain and uses them to compute the average cat
%and dog vectors. It also takes in a data matrix Xrun and 
%produces a vector of label guesses yguess, corresponding to whether
%each row of Xtest is closer to the average cat or average dog.
function yguess = closest_average(Xtrain,ytrain,Xrun)

    [meanCat, meanDog] = average_pet(Xtrain, ytrain); %getting the averages
    [rowsTest, ~] = size(Xrun);
    
    yguess = zeros(rowsTest,1); %pre-defining vector
    
    for i = 1:rowsTest
        distanceFromCat = sqrt(sum((Xrun(i,:) - meanCat).^2)); %find distance to nearest average cat
        distanceFromDog = sqrt(sum((Xrun(i,:) - meanDog).^2)); %find distance to nearest average dog
        if (distanceFromCat <= distanceFromDog) %compare distances
            yguess(i) = -1; %set appropriate flag in guess vector
        elseif (distanceFromCat > distanceFromDog)
            yguess(i) = 1;
        end
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
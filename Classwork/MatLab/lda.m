%Linear discriminant analysis training error rate is 0.94%.
%Linear discriminant analysis test error rate is 22%.

%This function takes in a training data matrix Xtrain, training
%label vector ytrain and uses them to compute the cat
%and dog sample mean vectors as well as the sample covariance matrix 
%(which is assumed to be equal for cats and dogs). 
%It also takes in a data matrix Xrun and produces a vector of
%label guesses yguess, corresponding to the ML rule for
%jointly Gaussian vectors with different means and the same 
%covariance matrix.
function yguess = lda(Xtrain,ytrain,Xrun)

    trainCats = Xtrain(find((ytrain == -1)),:); %#ok<*FNDSB> % using find for indices because logical indicing
    trainDogs = Xtrain(find((ytrain == 1)),:);  % would require an excessive loop
    
    [rowTrain, ~] = size(Xtrain); %setting up dimensions
    [rowCats, ~] = size(trainCats);
    [rowDogs, ~] = size(trainDogs);
    
    meanCats = mean(trainCats); %finding averages and covariances of training data
    meanDogs = mean(trainDogs);
    covarianceCats = cov(trainCats);
    covarianceDogs = cov(trainDogs);
    covarianceALL = 1/(rowTrain - 2) * ((rowCats - 1) * covarianceCats + (rowDogs - 1) * covarianceDogs); %covariance of everything
    invCovariance = pinv(covarianceALL); %inverse covariance
    
    Acompare = 2 * invCovariance * transpose(meanDogs - meanCats); % part of the formula for the decision rule
    Bcompare = (meanDogs * invCovariance * transpose(meanDogs)) - (meanCats * invCovariance * transpose(meanCats)); % other part of the formula
    
    yguess = 2 * (Xrun * Acompare >= Bcompare) - 1; % works because logical operator is 0 or 1; the 2* is needed to make sure the flag is set correctly
    
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
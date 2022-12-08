% For PCA-reduced data with dimension k = 50, QDA training error rate is 5.1%.
% For PCA-reduced data with dimension k = 50, QDA test error rate is 6.2%.
% For PCA-reduced data with dimension k = 100, QDA training error rate is 2.2%.
% For PCA-reduced data with dimension k = 100, QDA test error rate is 5%.
% For PCA-reduced data with dimension k = 200, QDA training error rate is 0.62%.
% For PCA-reduced data with dimension k = 200, QDA test error rate is 5.8%.
% For PCA-reduced data with dimension k = 300, QDA training error rate is 0.25%.
% For PCA-reduced data with dimension k = 300, QDA test error rate is 5.2%.
% For PCA-reduced data with dimension k = 400, QDA training error rate is 0.19%.
% For PCA-reduced data with dimension k = 400, QDA test error rate is 8.2%.
% For PCA-reduced data with dimension k = 500, QDA training error rate is 0.062%.
% For PCA-reduced data with dimension k = 500, QDA test error rate is 11%.

function yguess = qda(Xtrain,ytrain,Xrun)
    
    knownCats = Xtrain((ytrain == -1),:); % split training matrix with logical
    knownDogs = Xtrain((ytrain == 1),:); % indicing into cat and dog matrices

    [rowsTest, ~ ] = size(Xrun); %fetching dimensions info
    yguess = zeros(rowsTest,1); % pre-defining vectors
    
    [meanCat, meanDog] = average_pet(Xtrain,ytrain); % gets average of each
    
    covCat = cov(knownCats); % covariance matrices
    covDog = cov(knownDogs);
    
    invcat = pinv(covCat); % inverse covariance matrices
    invdog = pinv(covDog);
    
    deterCat = sum(log(eig(covCat))); % equivalent expression for determinant
    deterDog = sum(log(eig(covDog))); % of covariance matrices
    
    for i = 1:rowsTest
        
        compsCat = (Xrun(i,:) - meanCat) * invcat * (Xrun(i,:) - meanCat)' + deterCat; %calculate comparison values
        compsDog = (Xrun(i,:) - meanDog) * invdog * (Xrun(i,:) - meanDog)' + deterDog;
        
        if (compsCat >= compsDog)
            yguess(i) = 1;
        else
            yguess(i) = -1;
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
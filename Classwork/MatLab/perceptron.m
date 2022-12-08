%Perceptron training error rate is 0%
%Perceptron test error rate is 21%

%This function takes in a training data matrix Xtrain and training
%label vector ytrain and uses them to train the weights for a very
%simple one-layer neural network: the perceptron. It also takes in 
%a data matrix Xrun and produces a vector of label guesses yguess.
function yguess = perceptron(Xtrain,ytrain,Xrun)

    compare = pinv( transpose(Xtrain) * Xtrain) * (transpose(Xtrain) * ytrain);
    yguess = sign(Xrun * compare);

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
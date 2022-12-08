%This function takes in a data matrix X and a label
%vector y and outputs the average cat image and average dog image.
function [avgcat, avgdog] = average_pet(X,y)

[rows, columns] = size(X);
cats = 0; %sum total number of cats
dogs = 0; %sum total number of dogs
totalCats = zeros(1,columns); %sum of the image vectors
totalDogs = zeros(1,columns);

for i = 1:rows
    if ( y(i) == -1) %check flag and sum to appropriate totaling vector
        totalCats = totalCats + X(i,:);
        cats = cats + 1;
    elseif ( y(i) == 1)
        totalDogs = totalDogs + X(i,:);
        dogs = dogs + 1;
    end
end

avgcat = totalCats/cats; %take average value for vectors
avgdog = totalDogs/dogs;

%Your code should go above this line.
if (size(avgcat,1)~=1) 
    error("The variable avgcat is not a row vector.")
end
if (size(avgcat,2)~=size(X,2))
    error("The variable avgcat does not have the same number of columns as the data matrix input.")
end
if (size(avgdog,1)~=1) 
    error("The variable avgdog is not a row vector.")
end
if (size(avgdog,2)~=size(X,2)) 
    error("The variable avgdog does not have the same number of columns as the data matrix input.")
end
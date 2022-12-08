/*
    These algorithms are designed to open a file, TinyData.txt, that contains pseudorandom ASCII text.
    FindWords determines the number of words that only contain letters in TinyData.
    MatchDict determines how many words appear in the dictionary that do not contain a specific letter (here, 'E').
*/

#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <algorithm>
#include "TextMatching.h"

using namespace std;

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
Find the number of “words” that contain only letters. Here, we define a word as a
sequence of characters delimited by a white space (that is, any number of spaces,
new lines, tabs, etc).
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
/*  Algorithm concept:
    Input Buffer inherently delimits by space characters.
    So, just check if any symbols exist within the buffer
*/
int FindWords(){
    
    int counter = 0;
    
    string buffer; //takes on string value of input
    ifstream input;
    input.open("TinyData.txt");
    
    if(!input.fail()){ //must successfully open file
        bool word = true;
        int diff = 0;
        while(input>>buffer){
            word = true;
            for(int i = 0; i < buffer.length(); i++){
                diff = (buffer[i] - 'A'); //character algebra
                if(diff < 0){
                    word = false;
                    break;
                } else if(diff > 25 && diff < 32){
                    word = false;
                    break;
                } else if(diff > 57){
                    word = false;
                    break;
                } //else if instead of OR -> fewer boolean operators, is faster
            }
            if(word) counter++;
        }
    }
    
    return counter;
};

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
The number of English words (from the dictionary.txt file) that appear in the file
that do not contain the first letter of your first name. The words need to be
delimited by spaces, or other punctuation. Duplicates count. For example, the
word “a”, which is a dictionary word, appears in the data more than once, and
should be counted accordingly
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
/*  Algorithm concept:
    1. Use a while loop and fstream buffer to read each line of the dictionary
    2. Use a for loop to check the given string for the letter 'E'
    3. If the letter 'E' is not found, increment the counter
    4. Else do NOTHING and continue to next buffer input
*/

int MatchDict(){
    
    int counter = 0;
    string buffer; //takes on string value of input
    ifstream input;
    input.open("dictionary.txt");
    
    if(!input.fail()){
        bool valid = true;
        int uppercase = -1;
        int lowercase = -1;
        while(input>>buffer){
            valid = true;
            for(int i = 0; i < buffer.length(); i++){
                uppercase = buffer[i] - 'E'; //character algebra
                lowercase = buffer[i] - 'e';
                if(uppercase == 0){
                    valid = false;
                    break;
                } else if(lowercase == 0){
                    valid = false;
                    break;
                } //else if instead of OR -> fewer boolean operators, is faster
            }
            if(valid) counter++;
        }
    }
    return counter;
};

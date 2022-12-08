/*
    This algorithm accepts a list of strings and returns (via console) the strings that are
    rotations of each other. For example, "abba" and "bccb" are rotations by a factor of 1.
*/

#include <iostream>
#include <string>
#include <cstring>
#include "StringRotation.h"

using namespace std;

void findRotations(string list[], int length){
    
    /* 
     * The algorithm solution applied here compares the differences
     * between the indices of the strings. If the differences are
     * the same, the string is a rotation.
     *
     * This implementation will index items from the original list
     * to prevent double checking a word and reprinting it. With
     * larger inputs, this also helps make the algorithm more efficient.
    */

    unsigned short checked[length]= {0};

    //calculate the differences
    for(int i = 0; i < length-1; i++){
        
        if(~(checked[i])){ //check if word was already checked
        
            string str1 = list[i]; //pull for faster reference and clarity
            if (str1.length() < 1){ //no empty strings
                checked[i] = 1;
            } else {
                
                short * differenceA = new short[str1.length()];
                for(int x = 0; x < str1.length()-1; x++){ //difference between characters in the string
                    differenceA[x] = ((str1.at(x) - str1.at(x+1))%26 + 26); //normalize the difference
                    differenceA[x] = differenceA[x] >= 26 ? differenceA[x] % 26 : differenceA[x];
                }
                
                for(int j = i+1; j < length; j++){ //second string
                    
                    if(~(checked[j])){
                        
                        string str2 = list[j];
                        if (str2.length() < 1){
                            checked[j] = 1;
                        } else {
                            short * differenceB = new short[str2.length()];
                            for(int x = 0; x < str2.length()-1; x++){ 
                                differenceB[x] = ((str2.at(x) - str2.at(x+1))%26 + 26);
                                differenceB[x] = differenceB[x] >= 26 ? differenceB[x] % 26 : differenceB[x];
                            }
                            
                            //begin comparison of strings
                            
                            bool flag = (str2.length() == str1.length());
                            
                            if (flag){ //strings must be equal length
                                for(int x = 0; x < str1.length(); x++){
                                    if (differenceA[x] != differenceB[x]){
                                        flag = false; //if any character difference doesn't match, set flag 0
                                    }
                                }
                                if(flag) { // if the string was never flagged
                                    checked[i] = 2; //output words and index them
                                    checked[j] = 2;
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    
    for (int i = 0; i < length; i++){ //push output to console
        if (checked[i] == 2){
            cout << list[i] << endl;
        }
    }
}

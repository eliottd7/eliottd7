#include <iostream>
#include <string>

using namespace std;

// Given two sequences A and B, finds the longest sequence C such that
// C is a subsequence (not necessarily contiguous) of both A and B
string findMaxSubSeq(string str1, string str2)
{
	// the maximum common subsequence
	string subseq = "";
	string curr_seq = "";
    
    /*
        Linear Parallel Comparison Search:
        - implements std::string.find()
        - iterate through A while searching B
        -- if a match is found, check next character
        -- take the biggest max sequence
    */
	int itr = 0;
	for(auto i : str1){
	    curr_seq = "";
	    int first = str2.find(i);
	    if(first != string::npos){
	        int subl = 0;
	        int subitr = itr;
	        
	        while(str1[subitr] == str2[first]){
	            curr_seq = curr_seq+str1[subitr];
	            auto tmp = str1[subitr];
	            while(str1[subitr] == tmp) subitr++;
	            while(str2[first] == tmp) first++;
	        }
	        
	        if(curr_seq.length() > subseq.length()){
	            subseq = curr_seq;
	            curr_seq = "";
	        }
	    }
	    itr++;
	}

	return subseq;
}

// main to test findMaxSubSeq
int main(int argc, char const* argv[])
{
	// example sequences A, B
	string a = "";
	string b = "";

    if(argc < 3){
        cout<<"Error: not enough arguments"<<endl;
        return 1;
    } else if (argc > 3){
        cout<<"Error: too many arguments"<<endl;
        return 1;
    } else {
        a = argv[1];
        b = argv[2];
        // maximum subsequence in a and b
    	string maxSubSeq = findMaxSubSeq(a, b);
    	cout << maxSubSeq << endl;
    	return 0;   
    }
}

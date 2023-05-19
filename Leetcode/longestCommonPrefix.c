char * longestCommonPrefix(char ** strs, int strsSize){

    char tmp = 0;
    unsigned int letter = 0;
    unsigned int word = 0;
    while(true){
        word = 0;
        tmp = strs[word][letter];
        if(tmp == 0) goto loopexit;
        for(word = 1; word < strsSize; word++){
            if(tmp != strs[word][letter]) goto loopexit;
        }
        letter++;
    }

loopexit:
    strs[0][letter] = 0;
    return strs[0];
}

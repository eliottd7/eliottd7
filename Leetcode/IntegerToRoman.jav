class Solution {
    public String intToRoman(int num) {
             
        String output = "";
        
        int digit = 0;
        while(num > 0){
            
            if(num >= 1000){
                output = output + "M";
                num -= 1000;
            }
            
            if((num < 1000) && (num >= 900)){
                output = output + "CM";
                num -= 900;
            }
            
            if((num < 900) && (num >= 500)){
                output = output + "D";
                num -= 500;
            }
            
            if((num < 500) && (num >= 400)){
                output = output + "CD";
                num -= 400;
            }
            
            if((num < 400) && (num >= 100)){
                output = output + "C";
                num -= 100;
            }
            
            if((num < 100) && (num >= 90)){
                output = output + "XC";
                num -= 90;
            }
            
            if((num < 90) && (num >= 50)){
                output = output + "L";
                num -= 50;
            }
            
            if((num < 50) && (num >= 40)){
                output = output + "XL";
                num -= 40;
            }
            
            if((num < 40) && (num >= 10)){
                output = output + "X";
                num -= 10;
            }
            
            if((num < 10) && (num >= 9)){
                output = output + "IX";
                num -= 9;
            }
            
            if((num < 9) && (num >= 5)){
                output = output + "V";
                num -= 5;
            }
            
            if((num < 5) && (num >= 4)){
                output = output + "IV";
                num -= 4;
            }
            
            if((num < 5) && (num >= 1)){
                output = output + "I";
                num -= 1;
            }
        }
        
        return output;
        
    }
}

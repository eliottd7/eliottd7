/**
 * Definition for singly-linked list.
 * struct ListNode {
 *     int val;
 *     ListNode *next;
 *     ListNode() : val(0), next(nullptr) {}
 *     ListNode(int x) : val(x), next(nullptr) {}
 *     ListNode(int x, ListNode *next) : val(x), next(next) {}
 * };
 */
class Solution {
public:
    ListNode* addTwoNumbers(ListNode* l1, ListNode* l2) {
        ListNode * iter1 = l1; //current is the iterator through the linked lists
        ListNode * iter2 = l2;
        vector<uint8_t> sum1;
        vector<uint8_t> sum2;

        ListNode * current = nullptr;

        //step 1: fetch data
        while(iter1 != nullptr){
            sum1.push_back(iter1->val);
            iter1 = iter1->next;
        }
        while(iter2 != nullptr){
            sum2.push_back(iter2->val);
            iter2 = iter2->next;
        }

        //step 2: sum data
        if(sum1.size() >= sum2.size()){
            for(int i = 0; i < sum2.size(); i++){
                sum1.at(i) += sum2.at(i);
            }
            if(sum1.at(0) == 0 && sum1.size() == 1) return (new ListNode(0)); //sum = 0
            for(int i = 0; i < sum1.size(); i++){
                if (sum1.at(i) > 9 && i != sum1.size()-1){
                    sum1.at(i) %= 10;
                    sum1.at(i+1)++;
                } else if (sum1.at(i) > 9 && i == sum1.size()-1){
                    sum1.at(i) %= 10;
                    sum1.push_back(1);
                }
            }
            while(!sum1.empty()){
                if(current == nullptr){
                    current = new ListNode(sum1.back());
                    sum1.erase(sum1.end()-1, sum1.end());
                } else{
                    current = new ListNode(sum1.back(), current);
                    sum1.erase(sum1.end()-1, sum1.end());
                }
            }
        } else {
            for(int i = 0; i < sum1.size(); i++){
                sum2.at(i) += sum1.at(i);
            }
            for(int i = 0; i < sum2.size(); i++){
                if (sum2.at(i) > 9 && i != sum2.size()-1){
                    sum2.at(i) %= 10;
                    sum2.at(i+1)++;
                } else if (sum2.at(i) > 9 && i == sum2.size()-1){
                    sum2.at(i) %= 10;
                    sum2.push_back(1);
                }
            }
            while(!sum2.empty()){
                if(current == nullptr){
                    current = new ListNode(sum2.back());
                    sum2.erase(sum2.end()-1, sum2.end());
                } else{
                    current = new ListNode(sum2.back(), current);
                    sum2.erase(sum2.end()-1, sum2.end());
                }
            }
        }
 
        return current;
    }
};

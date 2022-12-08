#include "Person.h"
#include <string>
#include <iostream>
using namespace std;

Person::Person(int income, double tax_rate, string name){
    (*this).income = income;
    (*this).tax_rate = tax_rate;
    (*this).name = name;
    enemy = NULL;
    buddy = NULL;
};

int Person::calc_takehome_pay(){
    return (income - (tax_rate * income));
};

void Person::enemy_or_buddy(Person* P1){
    if((*this).income >= (*P1).income) {
        buddy = P1;
    }
    else {
        enemy = P1;
    }
};

void Person::print_status(){
    if(enemy == NULL) cout<<"No enemy."<<endl;
    else cout<<"Enemy: "<<((*enemy).name)<<endl;
    if(buddy == NULL) cout<<"No buddy."<<endl;
    else cout<<"Buddy: "<<((*buddy).name)<<endl;
};

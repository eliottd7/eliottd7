/*
    A barebones linked list implementation fully contained within this .h file.
*/

#ifndef LIST_H
#define LIST_H
#include "Node.h"
#include <iostream>
#include <string>

template <typename T>
class AbstractList{
    protected:
        unsigned int numElements; //protected members are protected in derived class
        Node<T> * head;
    public:
        AbstractList(){ //default constructor
            numElements = 0;
            head = nullptr;
        };
        
        ~AbstractList(){}; //destructor
        
        bool isEmpty(){
                return (head == nullptr); 
        };
        
        virtual void append(T x) = 0; //DECLARE here, DEFINE under subclass
        virtual T remove() = 0; //DECLARE here, DEFINE under subclass
};

template <typename T>
class List : public AbstractList<T> {
    public:
        void append(T x){
            Node<T> * list = new Node<T>(x, AbstractList<T>::head);
            AbstractList<T>::head = list;
            AbstractList<T>::numElements++;
        };
        
        T remove(){
            T value = AbstractList<T>::head->getValue();
            AbstractList<T>::head = AbstractList<T>::head -> getNext(); //bumps head to next            AbstractList<T>::numElements += -1;
            return value; //return value
        };
        
        void append_tail(T x){
            Node<T> * list = new Node<T>(x, nullptr);
            
            Node<T> * temp = AbstractList<T>::head;
            
            if (AbstractList<T>::numElements == 0){
                AbstractList<T>::head = list;
                list -> setNext(nullptr);
                AbstractList<T>::numElements++;
                return;
            }
            else{
                while ((temp -> getNext()) != nullptr){ //find end of list
                    temp = temp -> getNext();
                }   
                
                temp -> setNext (list);
                AbstractList<T>::numElements++;
            }
        };
        
        void insert(T x, int pos){
            pos--;
            Node<T> * list = new Node<T>(x, nullptr);
            
            Node<T> * temp = AbstractList<T>::head;
            
            int iter = 0;
            while (iter < pos){
                temp = temp -> getNext();
                iter++;
            }
            
            list -> setNext(temp -> getNext());
            temp -> setNext(list);
            
            AbstractList<T>::numElements ++;
        };
            
        T remove_at(int pos){
            Node<T> * temp = AbstractList<T>::head;
            T value = AbstractList<T>::head -> getValue();
            
            int iter = 0;
            while (iter < pos){
                temp = temp -> getNext();
                iter++;
                
                if (iter == (pos - 1)){
                    temp -> setNext( (temp -> getNext()) -> getNext());
                }
            }
            value = AbstractList<T>::head -> getValue();
            
            AbstractList<T>::numElements--;
            return value;
        };
        
        T operator[](int i) {
            Node<T> * temp = AbstractList<T>::head;
            
            int iter = 0;
            while(iter < i){
                temp = temp -> getNext();
                iter++;
            }
            
            return temp -> getValue();
        };
        
        friend std::ostream& operator <<(std::ostream& out, List<T> &l){
            Node<T> * temp = l.AbstractList<T>::head;
            while ((temp -> getNext()) != nullptr){ //find end of list
                out << (temp -> getValue()); out << " ";
                temp = temp -> getNext();
            }
            out << (temp -> getValue()); out << " ";
            return out;
        };
};

#endif

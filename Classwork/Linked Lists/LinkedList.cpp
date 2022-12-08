/*
  A more robust doubly linked list class specifically designed for integers
*/

#include "LinkedList.h"
#include <iostream>

 /*****************
 * class NODE setup
 *****************/

Node::Node(int data){
    this->data = data;
    this->next = nullptr; //initialize to null
    this->prev = nullptr;
}

void Node::print(){
    cout << this->data << " ";
}

 /*****************
 * class LIST setup
 *****************/

LinkedList::LinkedList(){
    this->head = nullptr; // initialize all pointers to null
    this->tail = nullptr;
    this->median = nullptr;
    this->size = 0; //initial list size is 0 because it's empty
};

LinkedList::~LinkedList() { }; //destructor

void LinkedList::insertToTail(int data){
    Node * insert = new Node(data); //pointer to new node object
    if(this->size == 0){ //special case, when first element is added
        this->head = insert; //point all pointers to single element
        this->median = insert;
        this->tail = insert;
        this->tail->next = nullptr; //next and prev should be null
        this->head->prev = nullptr;
    } else if(this->size == 1){ //otherwise
        this->tail = insert;
        //head still points to first element
        //median still points to first element due to floor
        this->head->next = insert;
        this->tail->prev = this->head;
    } else {
        Node* temp = this->tail; //pointer to old tail
        this->tail = insert; //redirect tail to new node
        this->tail->prev = temp; //connect new tail to old tail
        temp->next = insert; //connect old tail to new tail
        if(this->size %2 == 0){
            this->median = this->median->next; //odd # elements means shift median
        }
    }
    this->size += 1; //increment list size
};

int LinkedList::removeFromTail(){
    int out = 0;
    if(this->size != 0){ //checking there is a node to remove
        Node* temp = this->tail->prev; //create pointer to prevent losing tail node
        out = this->tail->data; //store the data to be returned before destruction
        temp->next = nullptr; //disconnect tail from list
        delete this->tail; //delete old tail node
        this->tail = temp; //point tail to "previous" node
        if(this->size % 2 == 1){
            this->median = this->median->prev; //adjust median if needed
        }
        this->size -= 1; //decrement list size
    }
    return out;
};

void LinkedList::insertToHead(int data){
    Node * insert = new Node(data);
    if(this->size == 0){
        this->head = insert; //special case: first new element
        this->median = insert;
        this->tail = insert;
        this->tail->next = nullptr;
        this->head->prev = nullptr;
    } else {
        Node* temp = this->head; //pointer to current head
        this->head = insert; //redirect head to new node
        this->head->next = temp; //connect new head to old head
        temp->prev = insert; //connect old head to new head
        if(this->size == 1){
            this->median = this->head; //adjust median if needed
        } else if(this->size % 2 == 1){
            this->median = this->median->prev;
        }
    }
    this->size += 1; //increment list size
};

int LinkedList::removeFromHead(){
    int out = 0;
    if(this->size != 0){
        Node* temp = this->head->next; //see documentation removeFromTail()
        out = this->head->data; 
        delete this->head; 
        temp->prev = nullptr; //point new head to null since it's first
        this->head = temp; //point head to second-in-list node
        this->size -= 1; //decrement list size
        //median automatically shifts into correct place
    }
    return out;
};

void LinkedList::insertToMedian(int data){
    Node * insert = new Node(data); //create node to be inserted
    if(this->size == 0){
        this->head = insert; //special case, single item list
        this->median = insert;
        this->tail = insert;
        this->tail->next = nullptr;
        this->head->prev = nullptr;
    } else if(this->size == 1){ //list will be 2 elements, assign pointers properly
        this->tail = insert;
        this->head->next = insert;
        this->tail->prev = this->head;
        this->median = this->head;
    } else {
        if(this->size % 2 == 0){ //insert median into center
            Node * aft = this->median->next;
            this->median->next = insert;
            aft->prev = insert;
            this->median->next = insert;
            insert->prev = this->median;
            insert->next = aft;
            this->median = insert;
        } else {                //insert median just "left" of center
            Node * bef = this->median->prev;
            bef->next = insert;
            this->median->prev = insert;
            insert->prev = bef;
            insert->next = this->median;
            this->median = insert;
        }
    }
    this->size += 1;
};

int LinkedList::removeFromMedian(){
    int out = this->median->data; //capture value of old median
    if(size != 0){
        Node* temp = this->median;
        this->median->prev->next = this->median->next; //point previous node to next node
        this->median->next->prev = this->median->prev; //point next node to previous node
        if(this->size % 2 == 0){ //adjust median position as necessary
            this->median = this->median->next;
        } else {
            this->median = this->median->prev;
        }
        delete temp; //destroy old node
        this->size -= 1; //decrement size
    }
    return out;
};

int LinkedList::returnMedian(){
    return this->median->data; //return data at median
};

void LinkedList::print(){
    Node* itr = this->head; //create iterator for cycle through list
    //wanted to use Node.print() but it was buggy for some reason
    for(int i = 0; i < this->size; i++){
        cout << (itr->data) << " "; //same functionality as Node.print()
        itr = itr->next; //bump iterator
    }
    cout<<endl;
};

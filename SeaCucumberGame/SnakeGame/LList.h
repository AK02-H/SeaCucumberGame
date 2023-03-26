/************************************************/
/*	Sea Cucumber Game	    	                */
/*	Programmer : Niblet, Amin Khan		        */
/*	Date : 11th May, 2022	                    */
/*	Email : akhan2019@elam.co.uk                */
/* 	Webpages : https://niblet.itch.io/		    */
/*             https://github.com/Niblet-0		*/
/* 	Discord : Niblet#7617		                */
/* 	                    		                */
/* 	Made for Teesside university		        */
/* 	Uhh, don't redistribute source code         */
/* 	and other copyright stuff                   */
/* 	                                            */
/************************************************/

#pragma once
#include <iostream>

using namespace std;

template <class T>
class LListNode
{
public:
	LListNode(LListNode<T>* _next = nullptr) {
		next = _next;
	}

	LListNode(T newElement, LListNode<T>* _next = nullptr) {
		next = _next;
		elem = newElement;
	}

	~LListNode() {
		cout << "LLIST DESTRUCTOR CALLED" << endl;
		//delete(&elem);
	}

	LListNode<T>* getNext() const
	{
		//cout << "GET NEXT" << endl;
		return next;
	}

	void setElement(T newElement) {
		elem = newElement;
	}

	T getElement() {
		return elem;
	}

	T* getElementPtr() {
		return &elem;
	}

private:
	T elem;	//The element held in this linked list node
	LListNode<T>* next = nullptr;	//The pointer to the next linked list node

	template <class T>
	friend class LList;	//Gives access to private members of linked list
};

template <class T>
class LList
{
public:

	LList() {

	}

	LList(LListNode<T>* start) {//constructor
		head = start;
		tail = start;
	}
	~LList() {
	};	//destructor

	bool empty() const {
		if (head.getNext()) {
			return false;
		}
		else {
			return true;
		}
	}

	LListNode<T>* front() const {		//get front element
		return head;
	}

	LListNode<T>* back() const {		//get front element
		return tail;
	}

	void addFront(const T& e) {


		if (head == nullptr) {	//if list is empty
			LListNode<T>* newNode = (new LListNode<T>(nullptr));	//New keyword??
			newNode->setElement(e);
			head = newNode;
			tail = newNode;
			//add to front of list

		}
		else {
			LListNode<T>* newNode = (new LListNode<T>(head));	//creates new node and sets it's next to the current head
			newNode->setElement(e);
			head = newNode;
			//add to front of list

		}

		
	}

	void addFront(LListNode<T>* node) {


		node->next = head;
		head = node;

	}

	void addBack(const T& e) {

		LListNode<T>* newNode = (new LListNode<T>(nullptr)); //New keyword??
		newNode->setElement(e);

		if (tail == nullptr) { //if list is empty
			tail = newNode;
			head = newNode;
		}
		else {
			
			tail->next = newNode;
			tail = newNode;
		}




	}

	void removeFront() {

		LListNode<T>* newHead = head->getNext();
		delete(head);	//am I deleting correctly?
		head = newHead;

	}

	void removeBack() {

		cout << "REMOVE BACK CALLED" << endl;

		if (head->next == tail) {
			cout << "CHEAT USED" << endl;

			head->next = nullptr;
			delete(tail);
			tail = head;


			return;
		}

		LListNode<T>* loopingNode = head;

		while (loopingNode->next != nullptr && loopingNode->next->next != nullptr){
			loopingNode = loopingNode->next;
		} 

		loopingNode->next = nullptr;
		delete(tail);
		tail = loopingNode;
	}

	void detatchBack() {
		LListNode<T>* loopingNode = head;

		LListNode<T>* prevNode = head;
		while (loopingNode->next != nullptr) {
			prevNode = loopingNode;
			loopingNode = loopingNode->next;
		}
		prevNode->next = nullptr;

		tail = prevNode;
	}

	void moveBackToFront() {
		

		LListNode<T>* loopingNode = head;
		LListNode<T>* nodeToBecomeTail = nullptr;

		LListNode<T>* prevNode = head;
		while (loopingNode->next != nullptr) {
			prevNode = loopingNode;
			loopingNode = loopingNode->next;
		}
		prevNode->next = nullptr;

		nodeToBecomeTail = prevNode;


		tail->next = head;
		head = tail;

		tail = nodeToBecomeTail;

	}


	T getFront() {
		return head->getElement();
	}

	T* getFrontPtr() {
		return head->getElementPtr();
	}

	T getBack() {
		return tail->getElement();
	}



	bool ContainsValue(T value) {

		/*LListNode<T>* loopingNode = head;

		do {
			if (loopingNode->getElement == value) {
				return true;
			}

			loopingNode = loopingNode->next;
		} while (loopingNode->next != nullptr);

		return false;
		*/
	}

	int count() {

		if (front() == nullptr) return 0;

		int counter = 0;

		LListNode<T>* currentNode = front();
		do {
			counter++;
			currentNode = currentNode->next;
		}
		while (currentNode->next != nullptr);

		return counter;
	}

	void DeleteAll() {

		cout << "DELETING ALL IN LLIST" << endl;

		LListNode<T>* loopingNode = head;

		while (loopingNode->next != nullptr) {
			LListNode<T>* temp = loopingNode;		//use smart pointer?
			loopingNode = loopingNode->next;
			delete temp;
		}
		
	}

private:
	LListNode<T>* head = nullptr;
	LListNode<T>* tail = nullptr;
};




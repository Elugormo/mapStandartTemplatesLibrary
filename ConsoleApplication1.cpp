// ConsoleApplication1.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//


#ifndef MAP_H_
#define MAP_H_
#include<iostream>
#include<utility>
#include<string>
#include<iterator>
#include<cassert>
#include<initializer_list>
#include<stdexcept>
#include<sstream>
#include <random>
#include <chrono>


namespace cs540 {
	template <typename T, typename U>
	class Map {
		using Key = T;
		using constKey = const T;
		using Value = U;
		using constValue = const U;
		using valueType = std::pair<const Key, Value>;
		using constValueType = const std::pair<const Key, Value>;
	public:
		Map();
		~Map();
		Map(std::initializer_list<std::pair<Key, Value>>);

		class Node {
		public:
			Node* left = nullptr;
			Node* right = nullptr;
			Node* parent = nullptr;
			Node* nextIter = nullptr;
			Node* prevIter = nullptr;
			char type = 'e';
			valueType* pairObj = nullptr;
			Node();
			Node(const valueType& obj);
			Node(valueType&& obj);
			~Node();
		};

		class Iterator {
		public:
			Node* iterNode;
			Iterator(Node* node); // default contructor
			Iterator(const Iterator& copy); // copy constructor
			Iterator(Iterator&& copy); // move constructor
			Iterator& operator = (const Iterator&); // coppy assignment operator
			Iterator& operator = (Iterator&&); // move assignment operator
			~Iterator(); // destructor

			Iterator& operator++(); // incrementing
			Iterator operator++(int); // std::atomic Atomic incrementing C++11
			Iterator& operator--(); // decrementing
			Iterator operator--(int); // std::atomic Atomic decrementing C++11
			valueType& operator*() const; // multiplication 
			bool operator ==(const Iterator& entry) const; // equality 
			bool operator !=(const Iterator& entry) const; // not equal

		};

		class ConstIterator {
		public:
			const Node* iterNode;
			ConstIterator(const Node* node); // default constructor
			ConstIterator(const Iterator&);//non const to constant Iter

			ConstIterator& operator++(); // incrementing
			ConstIterator operator++(int); // std::atomic Atomic incrementing C++11 
			ConstIterator& operator--(); // decrementing
			ConstIterator operator--(int); // std::atomic Atomic decrementing C++11
			valueType& operator*() const; // multiplication 
			bool operator ==(const ConstIterator& entry) const; // equality 
			bool operator !=(const ConstIterator& entry) const; // not equal

		};

		class ReverseIterator {
		public:
			const Node* iterNode;
			ReverseIterator(Node* node); // default constructor


			ReverseIterator& operator++(); // incrementing
			ReverseIterator operator++(int); // std::atomic Atomic incrementing C++11 
			ReverseIterator& operator--(); // decrementing
			ReverseIterator operator--(int); // std::atomic Atomic decrementing C++11
			valueType& operator*() const; // multiplication 
			bool operator ==(const ReverseIterator& entry) const; // equality 
			bool operator !=(const ReverseIterator& entry) const; // not equal

		};

		Iterator insert(constValueType& obj);
		Iterator insert(valueType& obj);

		void erase(Iterator it); // clear out all data in Map
		void setPointerAfterDelete(Iterator it); // set Iterator position
		void remove(constKey&); // remove node from Map

		Node* findPos(Node*, Key k); // find the position of the key
		void deleteAllNodes(Node* pNode); // clear data
		void deleteNode(Node* pNode); // delete single node
		Map(const Map&); // copy constructor
		void copyMap(Node* pNode); // copyMap to user's one
		Map(Map&&); // move constructor

		Map& operator=(const Map&); // assignment
		Map& operator=(Map&&); // assignment to r-value Map

		Iterator begin();
		Iterator end();

		ConstIterator begin() const;
		ConstIterator end() const;

		ReverseIterator rbegin() const;
		ReverseIterator rend() const;

		friend bool operator ==(const ConstIterator& a, const Iterator& b) {
			return (a == b);
		}

		friend bool operator !=(const ConstIterator& a, const Iterator& b) {
			return (a != b);
		}

		friend bool operator == (const Iterator& a, const ConstIterator& b) {
			return (a == b);
		}

		friend bool operator != (const Iterator& a, const ConstIterator& b) {
			return (a != b);
		}

		std::size_t size() const;
		bool empty() const;
		void clear();
		Iterator find(constKey&);
		ConstIterator find(constKey&) const;
		Value& at(constKey&);
		constValue& at(constKey&) const;
		Value& operator[](constKey&);
		bool operator ==(const Map&) const;
		bool operator !=(const Map&) const;

		void printTree();
		void printTreeH(Node* node);

	private:
		Node* root = nullptr;
		Node* head = new Node();
		Node* tail = new Node();
		std::size_t length = 0;
		Node* garbage = nullptr;
		Node* garbageEnd = nullptr;
		Iterator insertHelper(Node*);



	};


	template <typename T, typename U>
	Map<T, U>::Map() {
		this->head->nextIter = this->tail;
		this->tail->prevIter = this->head;
	}


	template <typename T, typename U>
	Map<T, U>::~Map() {
		this->printTree();

		if (this->root != nullptr) {
			this->deleteAllNodes(this->root);
			// this->deleteAllNodes(this->root->right);
		}

		for (Node* it = this->garbage; it != this->garbageEnd;) {
			if (it == nullptr) return;
			std::cout << "garbage ";
			Node* newNode = it->left;
			delete it;
			it = newNode;
		}
		//assert(this->head!=nullptr);
		//delete_node(this->head);
		//assert(this->tail!=nullptr);
		//delete_node(this->tail);
		//delete_node(this->root);

	}


	template<typename T, typename U>
	void Map<T, U>::deleteNode(Node* node) {
		// if (n == nullptr) return;
		 //delete_node(n->left);
		 //delete_node(n->right);
		 //delete n;
		 //n = nullptr;
#if 1
		//assert(n != nullptr);
		if (node == nullptr)return;
		//if(n->pair_obj!= nullptr)return;
		if (node->type == 'h')
		{
			this->head->nextIter = this->tail;
			this->tail->prevIter = this->head;
		}
		if (node != nullptr)
			delete node;
		node = nullptr;
		//why am i constantly dereferencing fucking this as null pointer... FUCK YOU ANYWAY, dude
#endif	
	}

	template<typename T, typename U>
	void Map<T, U>::deleteAllNodes(Node* node) {
		if (node == nullptr)
			return;


		deleteAllNodes(node->left);
		deleteAllNodes(node->right);
		deleteAllNodes(node);
	}

	template<typename T, typename U>
	void Map<T, U>::clear() {
		deleteAllNodes(this->root->left);
		deleteAllNodes(this->root->right);
		this->head->nextIter = this->tail;
		this->tail->prevIter = this->head;
		this->root = nullptr;
	}

	template<typename T, typename U>
	Map<T, U>::Node::Node() : pairObj(nullptr) {} // new valType 

	// node constructor
	template<typename T, typename U>
	Map<T, U>::Node::Node(constValueType& obj) : pairObj(new valueType(obj)) {}


	// move constructor
	template<typename T, typename U>
	Map<T, U>::Node::Node(valueType&& entry) : pairObj(&entry) {
		//pair_obj = &other;
		//other = nullptr;
	}


	// node destructor
	template<typename T, typename U>
	Map<T, U>::Node::~Node() {
		delete this->pairObj;
	}


	// const key initializer
	template <typename T, typename U>
	Map<T, U>::Map(std::initializer_list<std::pair<Key, Value>> initList) {
		for (auto i : initList) {
			this->insert(i);
		}
	}

	// copy constructor 
	template<typename T, typename U>
	Map<T, U>::Map(const Map& copy) {
		copyMap(copy.root);
	}

	// copy constructor helper function 
	template<typename T, typename U>
	void Map<T, U>::copyMap(Node* newNode) {
		if (newNode == nullptr)
			return;
		this->insert(*newNode->pairObj);
		copyMap(newNode->left);
		copyMap(newNode->right);
	}

	// move constructor 
	template<typename T, typename U>
	Map<T, U>::Map(Map&& copy) :root(copy.root), head(copy.head), tail(copy.tail) {
		copy.root = nullptr;
	}

	// Assignment operator 
	// very strange shit, i don't know how it works, but it works :D
	template <typename T, typename U>
	Map<T, U>& Map<T, U>::operator=(const Map<T, U>& entry) {
		std::cout << "Call" << std::endl;
		if (this != &entry) {
			this->deleteAllNodes(this->root);
			copyMap(entry.root);
		}
		return *this;
	}


	// move assignment iterator
	template <typename T, typename U>
	Map<T, U>& Map<T, U>::operator=(Map<T, U>&& entry) {
		if (this != &entry) {
			this->deleteAllNodes(this->root);
			this->root = entry.root;
			this->head->nextIter = entry.head->nextIter;
			this->tail->prevIter = entry.tail->prevIter;
			entry.root = nullptr;
			entry.head->nextIter = nullptr;
			entry.tail->prevIter = nullptr;
		}

		return *this;
	}


	//Iter ctor
	template<typename T, typename U>
	Map<T, U>::Iterator::Iterator(Node* node) :iterNode(node) {}


	//const Iter
	template<typename T, typename U>
	Map<T, U>::ConstIterator::ConstIterator(const Node* node) : iterNode(node) {}


	//non constant iterator to iterator
	template<typename T, typename U>
	Map<T, U>::ConstIterator::ConstIterator(const Iterator& entry) : iterNode(entry.iterNode) {}


	//reverse Iterator constructor
	template<typename T, typename U>
	Map<T, U>::ReverseIterator::ReverseIterator(Node* node) : iterNode(node) {}
	/*
	//copy ctor
	Map::Iterator::Iterator(const Iterator& copy) : iterNode(new Node(*copy.iterNode->pairObj)){}
	//move ctor
	Map::Iterator::Iterator(Iterator &&copy) : iterNode(copy.iterNode)
	{
	copy.iterNode = nullptr;
	}*/


	// iterator methods 


	template<typename T, typename U>
	typename Map<T, U>::Iterator& Map<T, U>::Iterator::operator++()
	{

		this->iterNode = this->iterNode->nextIter;
		return *this;
	}
	//const Iter
	template<typename T, typename U>
	typename Map<T, U>::ConstIterator& Map<T, U>::ConstIterator::operator++()
	{

		this->iterNode = this->iterNode->nextIter;
		return *this;
	}
	//reverse Iterator
	template<typename T, typename U>
	typename Map<T, U>::ReverseIterator& Map<T, U>::ReverseIterator::operator++()
	{

		this->iterNode = this->iterNode->prevIter;
		return *this;
	}


	template<typename T, typename U>
	typename Map<T, U>::Iterator Map<T, U>::Iterator::operator++(int) {
		Iterator temp(*this);
		++(*this);
		return temp;
	}

	// const
	template<typename T, typename U>
	typename Map<T, U>::ConstIterator Map<T, U>::ConstIterator::operator++(int) {

		ConstIterator temp(*this);
		++(*this);
		return temp;
	}


	//Reverse Iterator
	template<typename T, typename U>
	typename Map<T, U>::ReverseIterator Map<T, U>::ReverseIterator::operator++(int) {

		ReverseIterator temp(*this);
		++(*this);
		return temp;
	}

	template<typename T, typename U>
	typename Map<T, U>::Iterator& Map<T, U>::Iterator::operator--() {
		this->iterNode = this->iterNode->prevIter;
		return *this;
	}


	template<typename T, typename U>
	typename Map<T, U>::Iterator Map<T, U>::Iterator::operator--(int) {
		Iterator temp(*this);
		--(*this);
		return temp;
	}


	//const
	template<typename T, typename U>
	typename Map<T, U>::ConstIterator& Map<T, U>::ConstIterator::operator--() {
		this->iterNode = this->iterNode->prevIter;
		return *this;
	}


	template<typename T, typename U>
	typename Map<T, U>::ConstIterator Map<T, U>::ConstIterator::operator--(int) {
		ConstIterator temp(*this);
		--(*this);
		return temp;
	}


	//reverse Iterator
	template<typename T, typename U>
	typename Map<T, U>::ReverseIterator& Map<T, U>::ReverseIterator::operator--() {
		this->iterNode = this->iterNode->nextIter;
		return *this;
	}


	template<typename T, typename U>
	typename Map<T, U>::ReverseIterator Map<T, U>::ReverseIterator::operator--(int) {
		ReverseIterator temp(*this);
		--(*this);
		return temp;
	}

	template<typename T, typename U>
	bool Map<T, U>::Iterator::operator==(const Iterator& entry) const {
		return (this->iterNode == entry.iterNode);
	}


	template<typename T, typename U>
	bool Map<T, U>::Iterator::operator!=(const Iterator& entry) const {
		return !(this->iterNode == entry.iterNode);
	}

	// const

	template<typename T, typename U>
	bool Map<T, U>::ConstIterator::operator==(const ConstIterator& entry) const {
		return (this->iterNode == entry.iterNode);
	}

	template<typename T, typename U>
	bool Map<T, U>::ConstIterator::operator!=(const ConstIterator& entry) const {
		return !(this->iterNode == entry.iterNode);
	}

	// reverse

	template<typename T, typename U>
	bool Map<T, U>::ReverseIterator::operator==(const ReverseIterator& entry) const {
		return (this->iterNode == entry.iterNode);
	}


	template<typename T, typename U>
	bool Map<T, U>::ReverseIterator::operator!=(const ReverseIterator& entry) const {
		return !(this->iterNode == entry.iterNode);
	}



	//friend function.
	/*
	template<typename T,typename H>
	typename bool Map<T,H>operator==(const Map<T,H>::ConstIterator& c_iter,const Map<T,H>::Iterator &iter) {
		return (c_iter==c_iter);
	}
	*/

	//Map::Node* find_iterator(Map::Node)
	template<typename T, typename U>
	typename Map<T, U>::Iterator Map<T, U>::begin() {
		return Iterator(this->head->nextIter);
	}


	template<typename T, typename U>
	typename Map<T, U>::Iterator Map<T, U>::end() {
		return Iterator(this->tail);
	}

	template<typename T, typename U>
	typename Map<T, U>::ConstIterator Map<T, U>::begin() const {
		return ConstIterator(this->head->nextIter);
	}

	template<typename T, typename U>
	typename Map<T, U>::ConstIterator Map<T, U>::end() const {
		return ConstIterator(this->tail);
	}


	template<typename T, typename U>
	typename Map<T, U>::ReverseIterator Map<T, U>::rbegin() const {
		return ReverseIterator(this->tail->prevIter);
	}

	template<typename T, typename U>
	typename Map<T, U>::ReverseIterator Map<T, U>::rend() const {
		return ReverseIterator(this->head);
	}

	template<typename T, typename U>
	typename Map<T, U>::valueType& Map<T, U>::Iterator::operator*() const {
		return *(this->iterNode->pairObj);
	}

	// Error, can't return constValueType from operator*(), replaced with default valueType
	template<typename T, typename U>
	typename Map<T, U>::valueType& Map<T, U>::ConstIterator::operator*() const {
		return *(this->iterNode->pairObj);
	}


	template<typename T, typename U>
	typename Map<T, U>::valueType& Map<T, U>::ReverseIterator::operator*() const {
		return *(this->iterNode->pairObj);
	}

	template<typename T, typename U>
	std::size_t Map<T, U>::size() const {
		auto length = std::distance(this->begin(), this->end());
		return this->length;
	}

	template<typename T, typename U>
	bool Map<T, U>::empty() const {
		return(this->root == nullptr ? true : false);
	}

	template<typename T, typename U>
	bool Map<T, U>::operator==(const Map<T, U>& entry) const {
		std::cout << "Equality test" << std::endl;
		if (this->size() != entry.size()) {
			std::cout << "Size not equal" << std::endl;
			return false;
		}

		Map::ConstIterator it = entry.begin();
		Map::ConstIterator it2 = this->begin();


		size_t count = this->size();

		while (count != 0) {
			if ((it.iterNode->pairObj->first == it2.iterNode->pairObj->first) == false || (it.iterNode->pairObj->second == it2.iterNode->pairObj->second) == false) {
				std::cout << " False" << std::endl;
				return false;
			}

			++it;
			++it2;
			--count;
		}
		return true;
	}

	template<typename T, typename U>
	bool Map<T, U>::operator!=(const Map<T, U>& entry) const {
		if (*this == entry) {
			return false;
		}
		else
			return true;
	}

	template<typename T, typename U>
	typename Map<T, U>::Iterator Map<T, U>::find(constKey& other) {
		Node* newNode = this->root;
		while (newNode != nullptr) {
			if (other < newNode->pairObj->first) {
				newNode = newNode->left;
			}
			else if (newNode->pairObj->first < other) {
				newNode = newNode->right;
			}
			else if (newNode->pairObj->first == other) {
				return  (Iterator(newNode));
			}
		}

		return Iterator(this->root);
	}

	template<typename T, typename U>
	typename Map<T, U>::ConstIterator Map<T, U>::find(constKey& other) const {
		Node* newNode = this->root;
		while (newNode != nullptr) {
			if (other < newNode->pairObj->first) {
				newNode = newNode->left;
			}
			else if (newNode->pairObj->first < other) {
				newNode = newNode->right;
			}
			else if (newNode->pairObj->first == other) {
				return  (ConstIterator(newNode));
			}
		}

		return ConstIterator(this->root);
	}

	template<typename T, typename U>
	typename Map<T, U>::Value& Map<T, U>::at(constKey& entry) {
		std::cout << "Non constant at" << std::endl;

		Iterator it = find(entry);

		if (it == Iterator(this->tail)) {
			throw std::out_of_range("Value not present");
		}
		else {
			return (it.iterNode->pairObj->second);
		}
	}

	template<typename T, typename U>
	typename Map<T, U>::constValue& Map<T, U>::at(constKey& entry) const {

		std::cout << "Const at" << std::endl;
		ConstIterator it = find(entry);

		if (it == ConstIterator(this->tail)) {
			std::cout << "out of range" << std::endl;
			throw std::out_of_range("value not present in a range");
		}
		else {
			return (it.iterNode->pairObj->second);
		}
	}

	template <typename T, typename U>
	typename Map<T, U>::Value& Map<T, U>::operator[](constKey& entry) {
		Iterator it = find(entry);

		if (it == Iterator(this->tail)) {
			it = this->insert({ entry, Value {} });
		}

		return (it.iterNode->pairObj->second);
	}

	template <typename T, typename U> 
	typename Map<T, U>::Node* Map<T, U>::findPos(Map::Node* newNode, Key k) {
		assert(newNode != nullptr);

		if (newNode->pairObj->first == k) return newNode;
		else if (newNode->pairObj->first < k) {
			if (newNode->right == nullptr)
				return newNode;
			else
				return findPos(newNode->right, k);
		}
		else if (newNode->pairObj->first > k) {
			if (newNode->left == nullptr)
				return newNode;
			else
				return findPos(newNode->left, k);

		}

		return nullptr;
	}

	template <typename T, typename U>
	typename Map<T, U>::Iterator Map<T, U>::insertHelper(Node* newNode) {
		++this->length;
		if (this->root == nullptr) {   
			//root added.
			this->root = newNode;
			newNode->type = 'h';
			newNode->nextIter = this->tail;
			newNode->prevIter = this->head;
			this->tail->prevIter = this->root;//line added on monday moring
			this->head->nextIter = this->root;
			return Iterator(newNode);
		}
		else {
			Node* n = find_position(this->root, newNode->pairObj->first);
			if (n->pairObj->first == newNode->pairObj->first) {  
				//if duplicate 
				{
					//create a delete node function for this 
					--this->length;
					deleteNode(newNode);
					/*
					   delete new_node->pair_obj;
					   delete new_node;
					   */
				}
				return Iterator(n);
			}
			else if (n->pairObj->first < newNode->pairObj->first) { 
				//add to right
				n->right = newNode;
				newNode->parent = n;
				newNode->type = 'r';
				//code to set the prev_iter.
				newNode->prevIter = n;
				Node* temp = n->nextIter;
				temp->prevIter = newNode;

				//code to set the next_iter.
				newNode->nextIter = n->nextIter;
				n->nextIter = newNode;
				return Iterator(newNode);

			}
			else if (newNode->pairObj->first < n->pairObj->first) {   
				//add to left
				n->left = newNode;
				newNode->parent = n;
				newNode->type = 'l';
#if 1
				//code to set the next_iter.
				newNode->nextIter = n;
				Node* traverse = newNode;
				do {
					traverse = traverse->parent;
				} while (traverse->type == 'l');
				if (traverse->type == 'h') {
					Node* n = newNode->parent->prev_iter;
					//begin->next_iter = new_node;
					//not sure if correct..?   
					n->nextIter = newNode;
					//do notthing
				}
				else if (traverse->type == 'r') {
					traverse = traverse->parent;
					if (traverse->pairObj->first < newNode->pairObj->first) {
						traverse->nextIter = newNode;
					}
				}

				//code to set the prev_iter
				newNode->prevIter = n->prevIter;
				n->prevIter = newNode;
				return Iterator(newNode);
#endif      
			}
		}
		//traverse all the way left to set the begin node.
		//never reached here.
		throw std::runtime_error("I should never have gotten here");
		exit(0);
	}

	template<typename T, typename U>
	typename Map<T, U>::Iterator  Map<T, U>::insert(constValueType& obj) {
		std::cout << "++i" << std::endl;

		Node* newNode = new Node(obj);
		return insertHelper(newNode);
	}

#if 0
	++this->length;
	if (this->root == nullptr)
	{   //root added.
		this->root = new_node;
		new_node->type = 'h';
		new_node->next_iter = this->tail;
		new_node->prev_iter = this->head;
		this->tail->prev_iter = this->root;//line added on monday moring
		this->head->next_iter = this->root;
		return Iterator(new_node);
	}
	else
	{
		Node* n = find_position(this->root, new_node->pair_obj->first);
		if (n->pair_obj->first == new_node->pair_obj->first)
		{  //if duplicate 
			{//create a delete node function for this 
				--this->length;
				//delete_node(new_node);
				/*
				   delete new_node->pair_obj;
				   delete new_node;
				   */
			}
			return Iterator(n);
		}
		else if (n->pair_obj->first < new_node->pair_obj->first)
		{ //add to right
			n->right = new_node;
			new_node->parent = n;
			new_node->type = 'r';
			//code to set the prev_iter.
			new_node->prev_iter = n;
			Node* temp = n->next_iter;
			temp->prev_iter = new_node;

			//code to set the next_iter.
			new_node->next_iter = n->next_iter;
			n->next_iter = new_node;
			return Iterator(new_node);

		}
		else if (n->pair_obj->first > new_node->pair_obj->first)
		{   //add to left
			n->left = new_node;
			new_node->parent = n;
			new_node->type = 'l';
#if 1
			//code to set the next_iter.
			new_node->next_iter = n;
			Node* traverse = new_node;
			do
			{
				traverse = traverse->parent;
			} while (traverse->type == 'l');
			if (traverse->type == 'h')
			{
				Node* n = new_node->parent->prev_iter;//begin->next_iter = new_node;//not sure if correct..?   
				n->next_iter = new_node;
				//do notthing
			}
			else if (traverse->type == 'r')
			{
				traverse = traverse->parent;
				if (traverse->pair_obj->first < new_node->pair_obj->first)
				{
					traverse->next_iter = new_node;
				}
			}

			//code to set the prev_iter
			new_node->prev_iter = n->prev_iter;
			n->prev_iter = new_node;
			return Iterator(new_node);
#endif      
		}
	}
	//traverse all the way left to set the begin node.
	//never reached here.
	//throw std::runtime_error("I should never have gotten here");
}
#endif


template<typename T, typename U>
typename Map<T, U>::Iterator Map<T, U>::insert(valueType& obj) {
	//std::cout<<++i<<'\n';
	Node* new_node = new Node(obj);
	return insertHelper(new_node);
}
#if 0
++this->length;
if (this->root == nullptr)
{   //root added.
	this->root = new_node;
	new_node->type = 'h';
	new_node->next_iter = this->tail;
	new_node->prev_iter = this->head;
	this->tail->prev_iter = this->root;//line added on monday moring
	this->head->next_iter = this->root;
	return Iterator(new_node);
}
else
{
	Node* n = find_position(this->root, new_node->pair_obj->first);
	if (n->pair_obj->first == new_node->pair_obj->first)
	{  //if duplicate 
		{//create a delete node function for this 
			--this->length;
			//delete_node(new_node);
			/*
			   delete new_node->pair_obj;
			   delete new_node;
			   */
		}
		return Iterator(n);
	}
	else if (n->pair_obj->first < new_node->pair_obj->first)
	{ //add to right
		n->right = new_node;
		new_node->parent = n;
		new_node->type = 'r';
		//code to set the prev_iter.
		new_node->prev_iter = n;
		Node* temp = n->next_iter;
		temp->prev_iter = new_node;

		//code to set the next_iter.
		new_node->next_iter = n->next_iter;
		n->next_iter = new_node;
		return Iterator(new_node);

	}
	else if (n->pair_obj->first > new_node->pair_obj->first)
	{   //add to left
		n->left = new_node;
		new_node->parent = n;
		new_node->type = 'l';
#if 1
		//code to set the next_iter.
		new_node->next_iter = n;
		Node* traverse = new_node;
		do
		{
			traverse = traverse->parent;
		} while (traverse->type == 'l');
		if (traverse->type == 'h')
		{
			Node* n = new_node->parent->prev_iter;//begin->next_iter = new_node;//not sure if correct..?   
			n->next_iter = new_node;
			//do notthing
		}
		else if (traverse->type == 'r')
		{
			traverse = traverse->parent;
			if (traverse->pair_obj->first < new_node->pair_obj->first)
			{
				traverse->next_iter = new_node;
			}
		}

		//code to set the prev_iter
		new_node->prev_iter = n->prev_iter;
		n->prev_iter = new_node;
		return Iterator(new_node);
#endif      
	}
}
//traverse all the way left to set the begin node.
//never reached here.
//throw std::runtime_error("I should never have gotten here");
}
#endif


	template<typename T, typename U> 
	void Map<T, U>::remove(constKey& key) {
		std::cout << "Remove" << std::endl;
		Iterator it = find(key);
		if (it == Iterator(this->tail)) {
			throw std::out_of_range("value not present in a range");
		}

		if(it != this->end()) {
			erase(it);
		}
	}

	template<typename T, typename U>
	void Map<T, U>::printTree() {
		printTreeH(this->root);
		std::cout << std::endl;
	}

	template <typename T, typename U>
	void Map<T, U>::printTreeH(Node* node) {
		if (node == nullptr) return; 
		printTreeH(node->left);
		std::cout << node->pairObj->first << node->type << std::endl;

		printTreeH(node->right);
	}

	template<typename T, typename U>
	void Map<T, U>::erase(Iterator it) {
		assert(it.iterNode != this->tail);
		Node* parent = it.iterNode->parent; 
		if (it.iterNode->left == nullptr && it.iterNode->right == nullptr) {
			
			// 0 child
			std::cout << "Zero child" << std::endl;
			
			if (it.iterNode->type == 'l') {
				
				assert(parent != nullptr); 
				it.iterNode->parent = nullptr; 
				parent->left = nullptr; 
				std::cout << "end" << std::endl;
			
			}
			else if(it.iterNode->type == 'r') {
				assert(parent != nullptr);
				it.iterNode->parent = nullptr; 
				parent->right = nullptr; 
			}
			else if (it.iterNode->type == 'h') {
				Node* n = this->root; 
				this->root = nullptr; 
				deleteNode(n);
				--this->length; 
				return; 
			}
		}
		else if (it.iterNode->left == nullptr || it.iterNode->right == nullptr) {
			// 1 child
			std::cout << "1 child" << std::endl;
			if (it.iterNode->type == 'l' && it.iterNode->left == nullptr) {
				std::cout << "Case 1" << std::endl;
				assert(parent != nullptr);
				parent->left = it.iterNode->right; 
				it.iterNode->right->parent = parent; 
				it.iterNode->right->type = 'l';
			}
			else if (it.iterNode->type == 'l' && it.iterNode->right == nullptr) {
				std::cout << "Case2" << std::endl;
				assert(parent != nullptr);
				parent->left = it.iterNode->left; 
				it.iterNode->left->parent = parent;
				it.iterNode->left->type = 'l';
				std::cout << "endhere" << std::endl;
			}
			else if (it.iterNode->type == 'r' && it.iterNode->left == nullptr) {
				std::cout << "Case3" << std::endl;
				assert(parent != nullptr);
				parent->right = it.iterNode->right; 
				it.iterNode->right->parent = parent; 
				it.iterNode->right->type = 'r';
			}
			else if (it.iterNode->type == 'r' && it.iterNode->right == nullptr) {
				std::cout << "Case 4" << std::endl;
				assert(parent != nullptr);
				parent->right = it.iterNode->left; 
				it.iterNode->left->parent = parent; 
				it.iterNode->left->type = 'r';
			}
			else if (it.iterNode->type == 'h' && it.iterNode->left == nullptr) {
				std::cout << " Case 5" << std::endl;
				this->root->right->type = 'h';
				Node* tmp = it.iterNode->right; 
				this->root = tmp; 
			}
			else if (it.iterNode->type == 'h' && it.iterNode->right == nullptr) {
				std::cout << "Case 6" << std::endl;
				this->root->left->type = 'h';
				Node* tmp = it.iterNode->left; 
				this->root = tmp;
			}
		}
		else if (it.iterNode->left != nullptr && it.iterNode->right != nullptr) {
		// 2 child
			valueType* temp = it.iterNode->pairObj; 
			it.iterNode->pairObj = it.iterNode->nextIter->pairObj; 
			it.iterNode->nextIter->pairObj = temp; 
			erase(it.iterNode->nextIter);
			return;
		}

		setPointerAfterDelete(it);

	}

	template <typename T, typename U> 
	void Map<T, U>::setPointerAfterDelete(Iterator it) {
		std::cout << "set pointer" << std::endl; 
		std::cout << it.iterNode->pairObj->first;
		--this->length;
		assert(it.iterNode != this->head);
		assert(it.iterNode != this->tail);
		Node* prev = it.iterNode->prevIter; 
		Node* next = it.iterNode->nextIter; 
		Node* current = it.iterNode;
		assert(next != nullptr);
		assert(prev != nullptr);
		prev->nextIter = current->nextIter; 
		next->prevIter = current->prevIter; 
		if (this->garbage == nullptr) {
			std::cout << "First" << std::endl; 
			this->garbage = it.iterNode; 
			this->garbageEnd = it.iterNode;
		}
		else {
			std::cout << " Rest " << std::endl; 
			this->garbageEnd->left = it.iterNode; 
			this->garbageEnd = it.iterNode; 
		} // deleteNode(it.iterNode);
	}

}


#endif//MAP_H_
int main()
{

}

// Запуск программы: CTRL+F5 или меню "Отладка" > "Запуск без отладки"
// Отладка программы: F5 или меню "Отладка" > "Запустить отладку"

// Советы по началу работы 
//   1. В окне обозревателя решений можно добавлять файлы и управлять ими.
//   2. В окне Team Explorer можно подключиться к системе управления версиями.
//   3. В окне "Выходные данные" можно просматривать выходные данные сборки и другие сообщения.
//   4. В окне "Список ошибок" можно просматривать ошибки.
//   5. Последовательно выберите пункты меню "Проект" > "Добавить новый элемент", чтобы создать файлы кода, или "Проект" > "Добавить существующий элемент", чтобы добавить в проект существующие файлы кода.
//   6. Чтобы снова открыть этот проект позже, выберите пункты меню "Файл" > "Открыть" > "Проект" и выберите SLN-файл.

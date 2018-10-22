/*
 * Prefixtree.h
 *
 *  Created on: 2018. okt. 22.
 *      Author: Fjuzi
 */
#ifndef PREFIXTREE_H_
#define PREFIXTREE_H_
#include <iostream>
#include <set>
#include <vector>
#include <algorithm>

class Prefixtree {
public:
	class Node{
	public:
		char data;
		std::set<Node*> child;
		Node* parent;
		//Node();
		Node(const char _data): data(_data), parent(nullptr){};
		Node(const char _data, Node* parent): data(_data), parent(parent){};

	};
	Prefixtree();
	virtual ~Prefixtree();
	void insert(std::string x);
	void erase(std::string x);
	bool contains(std::string x);
	std::size_t size() const;
	std::size_t numNodes() const;
	std::string anagramFor(std::string anagramma) const;
	Node *find(const std::string& word, std::size_t & ind);



private:
	//Node *find(const std::string& word, std::size_t & ind);
	Node* root;
	Node* leaf;

	Node* keres(const char c, Node* n) const;
	std::size_t _size(Node* n) const;
	std::size_t _numNodes(Node* n) const;
};

#endif /* PREFIXTREE_H_ */

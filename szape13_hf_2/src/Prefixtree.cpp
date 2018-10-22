/*
 * Prefixtree.cpp
 *
 *  Created on: 2018. okt. 22.
 *      Author: Fjuzi
 */

#include "Prefixtree.h"

Prefixtree::Prefixtree() {
	// TODO Auto-generated constructor stub
	//std:cout << "ittvagyok";
	root = new Node(' ',nullptr);
	leaf = new Node('$');
}

Prefixtree::~Prefixtree() {
	// TODO Auto-generated destructor stub
}

Prefixtree::Node* Prefixtree::keres(char c, Prefixtree::Node* n) const{
	for(auto it = n->child.begin(); it != n->child.end(); it++){
		//std::cout << (*it)->data << " ";
		if( (*it)->data == c){
			//std::cout << "talalat" << std::endl;
			return *it;
		}
	}
	return nullptr;
}

void Prefixtree::insert(std::string x) {


	Node* p = root;
	for(int i = 0; i < x.length(); i++){
		//std::cout << x[i] << " ";
		Node* n = new Node(x[i],p);
		p->child.insert(n);
		p = n;
	}
	p->child.insert(leaf);
	std::size_t db = 0;
	//const std::string inp = x;
	const std::string inp = "szo";

	std::size_t szamlalo = 0;
	Node* res = find(inp,szamlalo);
	if(res != nullptr){
		std::cout << "az utolso adat: "<< res->data;
	}else{
		std::cout << "aha";
	}

}

void Prefixtree::erase(std::string x) {
	std::cout << x;
}

bool Prefixtree::contains(std::string x) {
	std::cout << x;
	return true;
}

std::size_t Prefixtree::_size(Node* n) const{
	Node* checker = keres('$', n); //ez egy értelmes szó
	std::size_t sum = 0;
	// itt lehet még okosítani
	if( n->child.size() == 1 && checker != nullptr ){
		return 1; // egy gyerek és vége az ágnak
	}
	if( n->child.size() >= 1 && checker == nullptr ){
		//nem ág vége, lehet több gyerek is, és nincs köztük végződés
		for(auto it: n->child){
			sum+= _size(it);
		}
	}else if(n->child.size() >= 1 && checker != nullptr){
		for(auto it: n->child){
			sum+= _size(n);
		}
		sum+=1;
	}
	return sum;
}

std::size_t Prefixtree::size() const {
	Node* x = root;
	std::size_t count = _size(x);
	return count;
}

std::size_t Prefixtree::_numNodes(Node* n) const{
	Node* checker = keres('$', n);
	std::size_t counter = 0;
	/*if(checker != nullptr){
		for(auto it: n->child){
			sum+= _size(n);
		}
	}*/
	for(auto it: n->child){
		if(it->data != '$'){
			counter = _numNodes(it)+1;
		}else{
			counter++;
		}
	}
	return counter;

}

std::size_t Prefixtree::numNodes() const {
	//VALAMI FELTÉTEL
	return _numNodes(root);
}

std::string Prefixtree::anagramFor(std::string anagramma) const {
	std::cout << anagramma;
	return anagramma;
}

Prefixtree::Node* Prefixtree::find(const std::string& word, std::size_t& ind) {
	Node* x = root;
	while(x != nullptr && ind != word.length()){
		Node* y = nullptr;
		for(unsigned int i = 0; i < word.length(); i++){
			y = keres(word[i],x);
			if(y != nullptr){
				x = y;
				ind++;
				break;
			}
		}
		if(y == nullptr){
			break;
		}
	}
	return x;

}


/*
 * Prefixtree.cpp
 *
 *  Created on: 2018. okt. 22.
 *      Author: Fjuzi
 */

#include "Prefixtree.h"

Prefixtree::Prefixtree() {
	// TODO Auto-generated constructor stub
	root = new Node(' ',nullptr);
	leaf = new Node('$');
}

Prefixtree::~Prefixtree() {
	// TODO Auto-generated destructor stub
}

Prefixtree::Node* Prefixtree::keres(char c, Prefixtree::Node* n) const{
	for(auto it = n->child.begin(); it != n->child.end(); it++){
		if( (*it)->data == c){
			return *it;
		}
	}
	return nullptr;
}

void Prefixtree::insert(std::string x) {
	std::size_t szamlalo = 0;
	const std::string inp = x;
	Node* res = _find(inp,szamlalo);
	//std::cout << "szamalo: " << szamlalo << std::endl;
	for(int i = szamlalo; i < x.length(); i++){
		Node* n = new Node(x[i],res);
		//std::cout << "add: " << x[i] << std::endl;
		res->child.insert(n);
		res = n;
	}
	res->child.insert(leaf);
}

void Prefixtree::erase(std::string x) {
	if(!contains(x)) return;
	std::cout << "eddig\n";
	std::size_t counter = 0;
	Node* vege = _find(x,counter);
	if(vege->child.size() == 1 && vege != root){
		std::set<Node*>::iterator it = vege->child.find(leaf);
		vege->child.erase(it);
		//std::cout << "törlöm: " << tmp->data << std::endl;
		//delete tmp; //eltörlöm a dollárt ez nem kell, elég csak dereferálni
		Node* gyerek = vege;
		Node* p = gyerek->parent;
		while(gyerek->child.size() == 0 && gyerek != root){
			//std::set<Node*>::iterator it = keres(gyerek->data,gyerek); // rosszat törlök ki!
			std::set<Node*>::iterator it = p->child.find(gyerek);
			p->child.erase(it);
			std::cout << "törlöm: " << gyerek->data << std::endl;
			delete gyerek;
			gyerek = p;
			p = p->parent;
			//nullpointert is törölni kell a parent gyerekei közül
		}
	}else{
		std::set<Node*>::iterator it = vege->child.find(leaf);
		vege->child.erase(it);
	}

}

bool Prefixtree::_seged(std::string& word, Node *x){
	if(word.length() == 0){
		for(auto it: x->child){
			if(it->data == '$'){
				return true; //ha elfogy a szó és dollár marad
			}
		}
		return false; // ha elfogy a szó, de nincs dolcsi
	}
	for(auto it: x->child){
		if(it->data == word[0]){
			word.erase(word.begin());
			return _seged(word,it);
		}
	}
	return false; //ha nem fogy el a szó és mégis csak $ maradt
}

bool Prefixtree::contains(std::string x){
	return _seged(x,root);

}

std::size_t Prefixtree::_size(Node* n) const{
	/*Node* checker = keres('$', n); //ez egy értelmes szó
	std::size_t sum = 0;
	// itt lehet még okosítani
	/*if( n->child.size() == 1 && checker != nullptr ){
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
	return sum;*/
	std::size_t sum = 0;
	if(n == nullptr) return 0;
	if(n->data == '$') return 1;
	for(auto it: n->child){
		if(it->data != '$'){
			sum += _size(it);
		}else{
			sum += 1;
		}
	}
	return sum;

}

std::size_t Prefixtree::size() const {
	Node* x = root;
	std::size_t count = _size(x);
	return count;
}

std::size_t Prefixtree::_numNodes(Node* n) const{
	std::size_t counter = 0;
	for(auto it: n->child){
		if(it->data != '$'){
			counter += _numNodes(it)+1;
		}else{
			counter++;
		}
	}
	return counter;

}

std::size_t Prefixtree::numNodes() const {
	//VALAMI FELTÉTEL
	return _numNodes(root)+1;
}

bool Prefixtree::_anagram(std::string& anagramma, Node *x, std::string& szo, std::string eredeti_szo) const{
	if(anagramma.length() == 0){
		auto it = x->child.find(leaf);
		if(it == x->child.end() || szo == eredeti_szo){
			return false;
		}else{
			return true;
		}
	}
	bool joe = false;
	for(auto it: x->child){
		for(int i = 0; i < anagramma.length(); i++){
			if(it->data == anagramma[i]){
				std::string temp = anagramma;
				std::string amibe_irok;
				temp.erase(temp.begin()+i);
				szo += anagramma[i];
				joe =  _anagram(temp, it, szo, eredeti_szo);
				if(!joe){
					szo.pop_back();
				}else{
					return joe;
				}
			}
		}
	}
	return false;
	if(!joe){
		return false;
	}
}

std::string Prefixtree::anagramFor(std::string anagramma) const {
	std::string szo = "";
	bool joe = false;
	for(auto it: root->child){
		for(int i = 0; i < anagramma.length(); i++){
			if(it->data == anagramma[i]){
				std::string temp = anagramma;
				std::string amibe_irok;
				temp.erase(temp.begin()+i);
				szo += anagramma[i];
				joe =  _anagram(temp, it, szo, anagramma);
				if(!joe){
					szo.pop_back();
				}else{
					return szo;
				}
			}
		}
	}
	return anagramma;

}

Prefixtree::Node* Prefixtree::find(const std::string& word, std::size_t& ind) {
	Node* x = root;
	/*for(unsigned int i = 0; i < word.length(); i++){
		std::cout << word[i];
	}
	std::cout << std::endl;*/
	while(x != nullptr && ind+1 != word.length()){
		Node* y = nullptr;
		//for(unsigned int i = 0; i < word.length(); i++){
		//std::cout << "ind: "<< ind << std::endl;
		for(unsigned int i = ind; i < word.length(); i++){
			//std::cout << "keresem: " << word[i] << std::endl;
			y = keres(word[i],x);
			if(y != nullptr){
				//std::cout << "talaltam " << y->data << " ";
				//std::cout << y->data << " ";
				x = y;
				ind++;
				break;
			}
			if(y == nullptr){
				return x;
			}
		}
		/*if(y == nullptr){
			break;
		}*/
		//std::cout << "vegtelen";
	}
	//std::cout << "visszater?";
	std::cout << std::endl;
	return x;

}

/*Prefixtree::Node* Prefixtree::_findHELP(const std::string& word, std::size_t& ind, Node *x) {
	//Node* next = keres(word[ind],x); //megkeresi a gyerekei között
	if(ind == word.length()) return x;
	Node* n = nullptr;
	for(auto it: x->child){
		if(it->data == word[ind]){
			ind++;
			Node* n = _findHELP(word,ind,it);
			if( n != nullptr){
				return n;
			}
		}else{

		}
	}
	ind--;
	return nullptr;
}*/

Prefixtree::Node* Prefixtree::_findHELP(const std::string& word, std::size_t& ind, Node *x){
	if(ind == word.length()){
		return x;
	}
	for(auto it: x->child){
		if(it->data == word[ind]){
			ind++;
			return _findHELP(word,ind,it);
		}
	}
	return x;

}

Prefixtree::Node* Prefixtree::_find(const std::string& word, std::size_t& ind) {
	return _findHELP(word, ind, root);
}




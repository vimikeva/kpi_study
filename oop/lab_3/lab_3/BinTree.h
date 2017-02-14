#pragma once
#include <iostream>
#include <fstream>

using namespace std;

template <class D>
class CBinTree{

	struct CTreeNode{
		D data;
		CTreeNode *left;
		CTreeNode *right;

		CTreeNode() : left(NULL), right(NULL) {};
		CTreeNode(D dat) {
			data = dat;
			left = NULL;
			right = NULL;
		};

		CTreeNode* operator = (CTreeNode* tn) {
			if (this == tn) 
				return this;
			data = tn->data;
			left = tn->left;
			right = tn->right;
			return this;
		}
		void print(){
			cout << data;
		}
	};

	CTreeNode* minimum(CTreeNode *temp) {
		if (!temp->left)
			return temp;
		return minimum(temp->left);
	}
	CTreeNode* insert(CTreeNode *temp, D data) {
		if (!temp)
			return new CTreeNode(data);
		else if (data < temp->data)
			temp->left = insert(temp->left, data);
		else
			temp->right = insert(temp->right, data);
		return temp;
	}
	CTreeNode* remove(CTreeNode *temp, D data) {
		CTreeNode *pointer;
		if (!temp) return temp;
		if (data < temp->data)
			temp->left = remove(temp->left, data);
		else if (data > temp->data)
			temp->right = remove(temp->right, data);
		else {
			if (!temp->left) {
				pointer = temp->right;
				delete temp;
				return pointer;
			}
			else if (!temp->right) {
				pointer = temp->left;
				delete temp;
				return pointer;
			}
			pointer = minimum(temp->right);
			temp->data = pointer->data;
			temp->right = remove(temp->right, temp->data);
		}
		return temp;
	}
	void print(CTreeNode *temp) {
		if (temp) {
			print(temp->left);
			temp->print();
			print(temp->right);
		}
	}
	void print(CTreeNode *temp, D data) {
		if (temp){
			print(temp->left, data);
			if (data == temp->data) {
				temp->print();
				root = remove(root, temp->data);
				return;
			}
			print(temp->right, data);
		}
	}

public:
	CBinTree() : root(NULL){};
	void load_from_file(char *file_name);
	void add_node(D data);
	void del_node(D data);
	void print_criteria(string flight, string flightdate);
	void print_all();

private:
	CTreeNode *root;
}; 

template<class D>
void CBinTree<D>::load_from_file(char *file_name) {
	D data;
	ifstream stream(file_name);
	if (!stream) {
		cout << "Couldn't open file\n";
		exit(1);
	}
	while (!stream.eof()) {
		stream >> data;
		add_node(data);
	}
	stream.close();
}

template <class D>
void CBinTree<D>::add_node(D data) {
	root = insert(root, data);
}

template <class D>
void CBinTree<D>::del_node(D data) {
	root = remove(root, data);
}

template <class D>
void CBinTree<D>::print_criteria(string flight, string flightdate) {
	D *data = new D("", flight, "", flightdate);
	print(root, *data);
}

template <class D>
void CBinTree<D>::print_all() {
	print(root);
}

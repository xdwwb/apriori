#pragma once
#include <map>
#include <set>
#include <vector>
#include <assert.h>

using namespace std;

class Node {
public:
	int level;//�����ڵĲ��
	int size;
	bool isLeaf;//�Ƿ�ΪҶ�ӽڵ�
	vector<int> values;
	vector<set<int>> keys;//����ֵ
	vector<Node*> children;//����ָ���ӽڵ��ָ��

	Node(int _level, int _size) :size(_size), level(_level), values(), keys(), children(), isLeaf(true) {

	}

	~Node() {
		//�������Ҷ�ӽڵ㣬��Ҫ�ݹ��������֧�ڵ�
		if (!isLeaf) {
			for (int i = 0;i < children.size();i++) {
				if (children[i])
					children[i]->~Node();
				delete children[i];
			}
		}
	}
	void insert(set<int> &key);
	void hashCount(set<int> &transaction);
	void collectFk(map<set<int>, int> &Fk, int min_sup_count);
private:
	void split();
};

class HashTree {
public:
	Node root;
	int size;

	HashTree(int _size) :size(_size), root(0, _size) {
	}
	void insert(set<int> &key)
	{
		root.insert(key);
	}
	void hashCount(set<int> &transaction) {
		//assert(transaction.size()>=this->size);
		//�������̫С����û�м����ı�Ҫ�ˣ�
		//if (transaction.size() < this->size)
			//return;
		root.hashCount(transaction);
	}

	void collectFk(map<set<int>, int> &Fk, int min_sup_count) {
		root.collectFk(Fk, min_sup_count);
	}

};


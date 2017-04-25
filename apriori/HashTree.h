#pragma once
#include <map>
#include <set>
#include <vector>
#include <assert.h>

using namespace std;

class Node {
public:
	int level;//树所在的层次
	int size;
	bool isLeaf;//是否为叶子节点
	vector<int> values;
	vector<set<int>> keys;//保存值
	vector<Node*> children;//保存指向孩子节点的指针

	Node(int _level, int _size) :size(_size), level(_level), values(), keys(), children(), isLeaf(true) {

	}

	~Node() {
		//如果不是叶子节点，需要递归地析构分支节点
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
		//如果事务太小，则没有计数的必要了！
		//if (transaction.size() < this->size)
			//return;
		root.hashCount(transaction);
	}

	void collectFk(map<set<int>, int> &Fk, int min_sup_count) {
		root.collectFk(Fk, min_sup_count);
	}

};


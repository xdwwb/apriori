#include "HashTree.h"
#include <algorithm>
#include <assert.h>
using namespace std;

void Node::insert(set<int>& key)
{
	//如果为叶子节点，则直接插入
	if (this->isLeaf) {
		this->keys.push_back(key);
		this->values.push_back(0);
		//如果大小超过限制并且有元素可以比较，则执行分裂操作
		if (this->keys.size() > size && this->level < this->size)
			this->split();
	}
	//如果为非叶子节点，则根据余数，插入到不同的子树
	else {
		//根据集合中第level层的值来确定需要插入的位置
		set<int>::iterator itr = key.begin();
		advance(itr, level);
		int branch = *itr%size;
		if (this->children[branch] == NULL) {
			Node *child = new Node(this->level + 1, size);
			this->children[branch] = child;
		}
		this->children[branch]->insert(key);
	}

}

void Node::hashCount(set<int>& transaction)
{
	if (isLeaf) {
		for (int i = 0;i < keys.size();i++) {
			set<int>::iterator itr_t = transaction.begin();
			advance(itr_t, level);
			set<int>::iterator itr_s = keys[i].begin();
			advance(itr_s,level);
			if (!equal(transaction.begin(), itr_t, keys[i].begin()))
				continue;
			if (includes(itr_t, transaction.end(), itr_s, keys[i].end()))
				values[i]++;
		}
	}
	else {
		//设置好起始指针！
		//begin 指向散射的开始位置
		set<int>::iterator begin = transaction.begin();
		advance(begin, level);
		//itr作为改变起始散射位置的指针变量
		set<int>::iterator itr = begin;
		//end作为散射结束位置
		set<int>::iterator end = transaction.begin();
		assert(transaction.size() - (size - level - 1) >= 1);
		advance(end, transaction.size() - (size - level - 1));
		for (;itr != end;++itr) {
			int branch = *itr%size;
			if (this->children[branch] == NULL)
				continue;
			set<int> sub;
			//插入已经散射过的部分
			sub.insert(transaction.begin(), begin);
			//插入新的部分
			sub.insert(itr, transaction.end());
			//子代继续这个过程
			children[branch]->hashCount(sub);
		}
	}
}

void Node::collectFk(map<set<int>, int>& Fk, int min_sup_count)
{
	if (this->isLeaf) {
		for (int i = 0;i < this->keys.size();i++) {
			if (this->values[i] >= min_sup_count) {
				Fk[this->keys[i]] = this->values[i];
			}
		}
	}
	else {
		for (int i = 0;i < this->children.size();i++) {
			if (this->children[i] == NULL)
				continue;
			else
				this->children[i]->collectFk(Fk, min_sup_count);
		}
	}
}

void Node::split()
{
	assert(this->level < this->size);
	Node &node = *this;
	node.isLeaf = false;
	node.children.resize(size, NULL);

	for (int i = 0;i < node.keys.size();i++) {
		set<int>::iterator itr = node.keys[i].begin();
		advance(itr, node.level);
		//越界了？
		int branch = *itr%size;
		if (node.children[branch] == NULL) {
			Node *child = new Node(node.level + 1, size);
			node.children[branch] = child;
		}
		node.children[branch]->insert(node.keys[i]);
	}
	node.keys.clear();
	node.values.clear();
}

#include "HashTree.h"
#include <algorithm>
#include <assert.h>
using namespace std;

void Node::insert(set<int>& key)
{
	//���ΪҶ�ӽڵ㣬��ֱ�Ӳ���
	if (this->isLeaf) {
		this->keys.push_back(key);
		this->values.push_back(0);
		//�����С�������Ʋ�����Ԫ�ؿ��ԱȽϣ���ִ�з��Ѳ���
		if (this->keys.size() > size && this->level < this->size)
			this->split();
	}
	//���Ϊ��Ҷ�ӽڵ㣬��������������뵽��ͬ������
	else {
		//���ݼ����е�level���ֵ��ȷ����Ҫ�����λ��
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
		//���ú���ʼָ�룡
		//begin ָ��ɢ��Ŀ�ʼλ��
		set<int>::iterator begin = transaction.begin();
		advance(begin, level);
		//itr��Ϊ�ı���ʼɢ��λ�õ�ָ�����
		set<int>::iterator itr = begin;
		//end��Ϊɢ�����λ��
		set<int>::iterator end = transaction.begin();
		assert(transaction.size() - (size - level - 1) >= 1);
		advance(end, transaction.size() - (size - level - 1));
		for (;itr != end;++itr) {
			int branch = *itr%size;
			if (this->children[branch] == NULL)
				continue;
			set<int> sub;
			//�����Ѿ�ɢ����Ĳ���
			sub.insert(transaction.begin(), begin);
			//�����µĲ���
			sub.insert(itr, transaction.end());
			//�Ӵ������������
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
		//Խ���ˣ�
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

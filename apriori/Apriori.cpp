#include "Apriori.h"
#include "HashTree.h"
#include <algorithm>
#include <sstream>
#include <iostream>

void Apriori::readFromFile(ifstream &in)
{
	string line, value;
	while (getline(in, line)) {
		istringstream linestream(line);
		set<int> itemset;
		while (getline(linestream, value, ',')) {
			istringstream valuestream(value);
			int num;
			valuestream >> num;
			itemset.insert(num);
		}
		//T.push_back(itemset);
		T.push_back(set<int>());
		T.back().swap(itemset);
	}
	this->min_sup_count = T.size()*min_sup;
}

map<set<int>, int>& Apriori::apriori_gen(map<set<int>, int>& C, int k)
{
	set<int> itemset;
	map<set<int>, int>::iterator itr_a,itr_b;
	map<set<int>, int>::iterator begin, end;
	begin = Fk[k].begin();
	end = Fk[k].end();
	for (itr_a = begin;itr_a != end;itr_a++)
		for (itr_b = begin;itr_b != end;itr_b++) {
			if (itr_a == itr_b)
				continue;
			if (equal(itr_a->first.begin(), --itr_a->first.end(), itr_b->first.begin())) {
				set<int> itemset(itr_a->first);
				itemset.insert(*(itr_b->first.rbegin()));
				
				if (k - 1<1) {
					C[itemset] = 0;
					continue;
				}
				//计数前剪枝
				set<int>::iterator itr;
				//TODO:这里可能有问题！
				set<int>::iterator sub_end = ----itemset.end();
				for (itr = itemset.begin();itr != sub_end;itr++) {
					set<int> sub_itemset(itemset);
					sub_itemset.erase(*itr);
					map<set<int>, int>::iterator itr_result;
					itr_result = Fk[k - 1].find(sub_itemset);
					if (itr_result!=Fk[k-1].end())
						continue;//查找成功
					else
						break;//查找失败
				}
				if(itr!=sub_end)//说明其所以的子集合都是频繁的！
					C[itemset] = 0;
			}
		}
	// TODO: 在此处插入 return 语句
	return C;
}

void Apriori::run()
{
	//候选项集
	map<set<int>, int> C;

	//计算频繁一项集
	int size = T.size();
	for (size_t i = 0;i < size;i++) {
		set<int>::iterator itr;
		for (itr = T[i].begin();itr != T[i].end();itr++) {
			set<int> itemset;
			itemset.insert(*itr);
			C[itemset]++;
		}
	}

	Fk.push_back(map<set<int>, int>());//频繁0项集
	Fk.push_back(map<set<int>, int>());//频繁1项集
	
	//存放频繁的项，用于对事务进行过滤！
	set<int> filter;
	//将频繁1项集搜集起来
	map<set<int>, int>::iterator itr;
	for (itr = C.begin();itr != C.end();itr++) {
		if (itr->second >= this->min_sup_count) {
			Fk[1][itr->first] = itr->second;
			filter.insert(*itr->first.begin());
		}
	}
	C.clear();
	//过滤事务中非频繁的项
	//int size = T.size();
	for (size_t i = 0;i < size;i++) {
		vector<int> temp;
		set_intersection(T[i].begin(),T[i].end(),filter.begin(),filter.end(),back_inserter(temp));
		set<int> temp_set(temp.begin(),temp.end());
		T[i].swap(temp_set);
		temp_set.clear();
		temp.clear();
	}

	int k = 1;
	while (!Fk[k].empty()) {
		k++;
		//候选生成
		C = apriori_gen(C,k-1);
		HashTree hashtree(50);
		map<set<int>, int>::iterator itr;
		for (itr = C.begin();itr != C.end();itr++) {
			set<int> temp = itr->first;
			hashtree.insert(temp);
		}

		for (int i = 0;i < T.size();i++) {
			if (T[i].size() < k)
				continue;
			hashtree.hashCount(T[i]);
		}

		Fk.push_back(map<set<int>,int>());
		hashtree.collectFk(Fk[k],min_sup_count);
		C.clear();
	}

}

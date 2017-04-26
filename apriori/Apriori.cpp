#include <sstream>
#include <iomanip>
#include <iostream>
#include <algorithm>
#include "Apriori.h"
#include "HashTree.h"

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

void Apriori::readTransactions(ifstream & in)
{
	string line, value;
	while (getline(in, line)) {
		istringstream linestream(line);
		set<int> itemset;
		while (getline(linestream, value, ',')) {
			int num = dict[value];//查询，如若失败，则插入，值为0
			if (num == 0) {
				num = dict.size();//查询失败，则重新编号
				dict[value] = num;
			}
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
	map<set<int>, int>::iterator itr_a, itr_b;
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

				if (k - 1 < 1) {
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
					if (itr_result != Fk[k - 1].end())
						continue;//查找成功
					else
						break;//查找失败
				}
				if (itr != sub_end)//说明其所以的子集合都是频繁的！
					C[itemset] = 0;
			}
		}
	// TODO: 在此处插入 return 语句
	return C;
}

vector<set<int>>& Apriori::gen_postCondition(vector<set<int>>& Cm, vector<set<int>>& Hm)
{
	assert(!Hm.empty());
	assert(Hm[0].size() >= 1);

	int m = Hm.size();
	for (int i = 0;i < Hm.size();i++) {
		for (int j = i + 1;j < Hm.size();j++) {
			assert(Hm[i] != Hm[j]);
			if (equal(Hm[i].begin(), --Hm[i].end(), Hm[j].begin())) {
				set<int> itemset(Hm[i]);
				itemset.insert(*Hm[j].rbegin());
				Cm.push_back(set<int>());
				Cm.back().swap(itemset);
			}
		}
	}
	return Cm;
	// TODO: 在此处插入 return 语句
}

void Apriori::run()
{
	generateFn();
	generateRules();

}

void Apriori::generateFn()
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
		set_intersection(T[i].begin(), T[i].end(), filter.begin(), filter.end(), back_inserter(temp));
		set<int> temp_set(temp.begin(), temp.end());
		T[i].swap(temp_set);
		temp_set.clear();
		temp.clear();
	}

	int k = 1;
	while (!Fk[k].empty()) {
		k++;
		//候选生成
		C = apriori_gen(C, k - 1);
		HashTree hashtree(k);
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

		Fk.push_back(map<set<int>, int>());
		hashtree.collectFk(Fk[k], min_sup_count);
		C.clear();
	}
}

void Apriori::generateRules()
{
	for (int k = 2;k < Fk.size();k++) {
		map<set<int>, int>::iterator itr;
		for (itr = Fk[k].begin();itr != Fk[k].end();itr++) {
			int m = 1;
			vector<int> C1(itr->first.begin(), itr->first.end());
			vector<set<int>> Hm;
			//生成一个项作为后件的规则
			for (int i = 0;i < C1.size();i++) {
				set<int> post;
				post.insert(C1[i]);
				set<int> pre(itr->first);
				pre.erase(C1[i]);
				double confidence = (double)itr->second / (double)Fk[k - 1][pre];
				if (confidence >= this->min_conf) {
					Rule r;
					r.pre = pre;
					r.post = post;
					r.conf = confidence;
					r.supp = (double)itr->second / (double)(this->T.size());
					r.lift = confidence / ((double)Fk[1][post] / (double)(this->T.size()));
					rules.push_back(r);
					Hm.push_back(post);
				}
			}
			vector<set<int>> Cm;
			while (k > m + 1 && !Hm.empty()) {
				m++;
				Cm = gen_postCondition(Cm, Hm);
				//set<set<int>> temp(Cm.begin(),Cm.end());
				//Cm.assign(temp.begin(),temp.end());
				Hm.clear();
				for (int i = 0;i < Cm.size();i++) {
					set<int> post(Cm[i]);
					vector<int> dif_set;
					set_difference(itr->first.begin(), itr->first.end(), post.begin(), post.end(), back_inserter(dif_set));
					set<int> pre(dif_set.begin(), dif_set.end());

					assert(pre.size() == k - m);
					double confidence = (double)itr->second / (double)Fk[k - m][pre];
					if (confidence >= this->min_conf) {
						Rule r;
						r.pre = pre;
						r.post = post;
						r.conf = confidence;
						r.supp = (double)itr->second / (double)(this->T.size());
						assert(m == post.size());
						r.lift = confidence / ((double)Fk[m][post] / (double)(this->T.size()));
						rules.push_back(r);
						Hm.push_back(post);
					}//end if
				}//end for
				Cm.clear();

			}//end while
		}//end for
	}//end for
}
void Apriori::save(ofstream & out)
{
	out << "support\tconfidence\tlift\trule\n";
	for (int i = 0;i < this->rules.size();i++) {
		rules[i].save(out);
	}
}
void Apriori::saveAssociation(ofstream & out)
{
	map<int, string> dictionary;
	map<string, int>::iterator itr_dict;
	for (itr_dict = dict.begin();itr_dict != dict.end();itr_dict++) {
		dictionary[itr_dict->second] = itr_dict->first;
	}

	out << setw(8) << setiosflags(ios::left) << "support" << "\t" << setw(8) << "confidence" << "\t" << setw(8) << "lift" << "\t" << setw(8) << "rule" << endl;
	for (int i = 0;i < this->rules.size();i++) {
		rules[i].saveAssociation(out, dictionary);
	}
}
///end function

void Rule::save(ofstream& out)
{
	out << this->supp << "\t\t" << this->conf << "\t\t" << this->lift << "\t\t";
	out << "{";
	set<int>::iterator itr_pre, itr_post;
	for (itr_pre = this->pre.begin();itr_pre != this->pre.end();itr_pre++) {
		out << *itr_pre;
		if (itr_pre != --this->pre.end())
			out << ",";
	}
	out << "=>";
	for (itr_post = this->post.begin();itr_post != this->post.end();itr_post++) {
		out << *itr_post;
		if (itr_post != --this->post.end())
			out << ",";
	}
	out << "}";
	out << endl;
}

void Rule::saveAssociation(ofstream & out, map<int, string> &dictionary)
{
	out << setprecision(2) << this->supp << "\t\t" << this->conf << "\t\t" << this->lift << "\t\t";
	out << "{ ";
	set<int>::iterator itr_pre, itr_post;
	for (itr_pre = this->pre.begin();itr_pre != this->pre.end();itr_pre++) {
		out << dictionary[*itr_pre];
		if (itr_pre != --this->pre.end())
			out << ", ";
	}
	out << " => ";
	for (itr_post = this->post.begin();itr_post != this->post.end();itr_post++) {
		out << dictionary[*itr_post];
		if (itr_post != --this->post.end())
			out << ", ";
	}
	out << " }";
	out << endl;
}

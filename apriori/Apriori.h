#pragma once
#include <set>
#include <map>
#include <vector>
#include <fstream>
#include <algorithm>

using namespace std;

class Rule;

class Apriori {
public:

	Apriori(ifstream &_in, double _min_sup, double _min_conf) :min_sup(_min_sup), min_conf(_min_conf) {
		this->readFromFile(_in);
	}

	void run();

	void generateFn();

	void generateRules();

	void save(ofstream &out);


private:

	double min_sup;//最小支持度

	double min_conf;//最小置信度

	int min_sup_count;//最小支持度计数

	vector<set<int>> T;//所有事务的集合

	vector<map<set<int>, int>> Fk;//频繁项集的集合

	vector<Rule> rules;

	void readFromFile(ifstream &in);

	map<set<int>, int>& apriori_gen(map<set<int>, int> &C, int k);

	vector<set<int>>& gen_postCondition(vector<set<int>>& Cm, vector<set<int>> &Hm);

public:
	double getMinConfidence() {
		return this->min_conf;
	}

	int getMin_sup_count() {
		return this->min_sup_count;
	}

	size_t getTransactionSize() {
		return T.size();
	}

	size_t getFrequentItemSetSize() {
		size_t size=0;
		for (int i = 0;i < Fk.size();i++) {
			size += Fk[i].size();
		}
		return size;
	}

	size_t getRulesSize() {
		return rules.size();
	}
};

class Rule {

public:

	set<int> pre;//precondition,规则前件

	set<int> post;//postcondition，规则后件

	double conf;//confidence 置信度

	double supp;//support 支持度

	double lift;//lift 提升度

	void save(ofstream &out);

};
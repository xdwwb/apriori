#pragma once
#include <fstream>
#include <vector>
#include <set>
#include <map>

using namespace std;

class Apriori {
public:
	Apriori(ifstream &_in, double _min_sup, double _min_conf) :min_sup(_min_sup), min_conf(_min_conf) {
		this->readFromFile(_in);
	}
	void run();

private:
	double min_sup;
	double min_conf;
	int min_sup_count;
	vector<set<int>> T;//所有事务的集合
	vector<map<set<int>, int>> Fk;//频繁项集的集合


	void readFromFile(ifstream &in);
	map<set<int>, int>& apriori_gen(map<set<int>,int> &C,int k);
};
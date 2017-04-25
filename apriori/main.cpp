#include <fstream>
#include <iostream>
#include <stdlib.h>

#include "test.h"
#include "Apriori.h"

using namespace std;

int main(int argc, char** argv) {

	double support = 0;
	double confidence = 0;

	//检查参数数量
	if (argc != 4) {
		cerr << "Argument number is wrong, please check your input!" << endl;
		//system("pause");
		return 0;
	}

	//接收参数
	ifstream in(argv[1]);
	support = atof(argv[2]);
	confidence = atof(argv[3]);


	//检查参数
	if (!(support >= 0 && support <= 1)) {
		cerr << "The argument of support is out of the right range, please input again!" << endl;
		//system("pause");
		return 0;
	}
	if (!(confidence >= 0 && confidence <= 1)) {
		cerr << "The argument of confidence is out of the right range, please input again!" << endl;
		//system("pause");
		return 0;
	}
	if (!in) {
		cerr << "The input file path is wrong, please input again!" << endl;
		//system("pause");
		return 0;
	}

	//test t;
	clog << "------------------------------------------------------" << endl;
	clog << "Welcome to use Apriori to mine association rules!" << endl;
	clog << "Author: Weibing Wang" << endl;
	clog << "Data: 2017-04-25" << endl;
	clog << "------------------------------------------------------" << endl;
	clog << "Minimun Support: " << support << endl;
	clog << "Minimum Confidence: " << confidence << endl;
	
	Apriori apriori(in, support, confidence);
	clog << "Minimum Support Count: " << apriori.getMin_sup_count() << endl;
	clog << "Transactions Size: " << apriori.getTransactionSize() << endl;
	apriori.run();
	clog << "Apriori is mining......" << endl;
	clog << "Mined done!" << endl;
	clog << "Frequent Item Set Size: " << apriori.getFrequentItemSetSize() << endl;
	clog << "Association Rules Size: " << apriori.getRulesSize() << endl;

	ofstream out("rules.csv");
	apriori.save(out);
	clog << "Mining result have been saved to file \"rules.csv\"" << endl;

	//clog << "OK!" << endl;
	in.close();
	out.close();
	//system("type rules.csv");
	//system("pause");
	return 0;
}
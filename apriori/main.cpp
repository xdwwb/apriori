#include <iostream>
#include <stdlib.h>
#include <fstream>
#include "Apriori.h"
#include "test.h"

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

	test t;

	Apriori apriori(in, support, confidence);
	apriori.run();

	//clog << "OK!" << endl;
	in.close();
	system("pause");
	return 0;
}
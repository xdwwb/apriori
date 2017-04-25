#include "test.h"

test::test() {
	//hashTreeTest();//已经成功
}

inline void test::hashTreeTest() {
	HashTree hashTree(3);

	hashTree.insert(set<int>({ 1,4,5 }));
	hashTree.insert(set<int>({ 1,2,4 }));
	hashTree.insert(set<int>({ 4,5,7 }));
	hashTree.insert(set<int>({ 1,2,5 }));
	hashTree.insert(set<int>({ 4,5,8 }));

	hashTree.insert(set<int>({ 1,5,9 }));
	hashTree.insert(set<int>({ 1,3,6 }));
	hashTree.insert(set<int>({ 2,3,4 }));
	hashTree.insert(set<int>({ 5,6,7 }));
	hashTree.insert(set<int>({ 3,4,5 }));

	hashTree.insert(set<int>({ 3,5,6 }));
	hashTree.insert(set<int>({ 3,5,7 }));
	hashTree.insert(set<int>({ 6,8,9 }));
	hashTree.insert(set<int>({ 3,6,7 }));
	hashTree.insert(set<int>({ 3,6,8 }));

	hashTree.hashCount(set<int>({ 1,2,3,5,6 }));
	map<set<int>, int> result;
	hashTree.collectFk(result, 1);
	//本测试已经成功
}

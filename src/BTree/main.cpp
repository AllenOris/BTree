#include <iostream>
#include <cstdio>
#include "BTree.h"
#include "BTree.cpp"	
using namespace std;

bool cinFail() {
	if (!cin.fail())return 0;
	cout << "格式有误！" << endl;
	std::cin.clear();
	cin.ignore(1024, '\n');
	return 0;
}

int main() {
	cout << "请输入关键字个数及B树阶数：";
	int n, k;
	do {
		cin >> n >> k;
		if (cin.fail()) {
			cinFail();
			cout << "请重新输入关键字个数及B树阶数：";
		}
		else break;
	} while (1);

	BTree<int>*tree = new BTree<int>(k);

	for (int i = 0; i < n; ++i) {
		int x;
		cin >> x;
		if (cin.fail()) {
			cout << "格式有误!";
			delete tree;
			return 0;
		}
		tree->insert(x);
	}

	tree->printWholeTree();

	cout << "输入需要进行的操作,格式： 1.插入：I 整数 2.删除：D 整数 \n（E退出）\n";
	while (1) {
		char x;
		int key;
		cin >> x;
		if (cinFail())continue;
		if (x == 'E')break;
		cin >> key;
		if (cinFail())continue;
		else if (x == 'I') {
			if (!tree->insert(key))cout << "键值已存在!\n";
		}
		else if (x == 'D') {
			if (!tree->erase(key))cout << "未找到键值!\n";
		}
		else {
			cinFail();
			continue;
		}
		tree->printWholeTree();
	}
	delete tree;
	
	return 0;
}

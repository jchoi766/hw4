#include "avlbst.h"

#include <iostream> 
using namespace std;
int main() {
	AVLTree<uint16_t, uint16_t> testTree;

	testTree.insert(std::make_pair(5, 8));
	cout << "\n\nAfter adding 5:" << endl;
	testTree.print();

	testTree.insert(std::make_pair(3, 159));
	cout << "\n\nAfter adding 3:" << endl;
	testTree.print();
	testTree.insert(std::make_pair(1, 9));
	cout << "\n\nAfter adding 1:" << endl;
	testTree.print();
	testTree.insert(std::make_pair(3, 0x5a));
	cout << "\n\nAfter adding 3 (dup):" << endl;
	testTree.print();
	testTree.insert(std::make_pair(4, 0xb));
	cout << "\n\nAfter adding 4:" << endl;
	testTree.print();
	testTree.insert(std::make_pair(1, 4));
	cout << "\n\nAfter adding 1(dup):" << endl;
	testTree.print();
	testTree.insert(std::make_pair(3, 159));
	cout << "\n\nAfter adding 3(dup):" << endl;
	testTree.print();

}
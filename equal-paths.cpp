#ifndef RECCHECK
//if you want to add any #includes like <iostream> you must do them here (before the next endif)

#endif

#include "equal-paths.h"
using namespace std;


// You may add any prototypes of helper functions here
int countHeight(Node *curr);


bool equalPaths(Node * root)
{
  // Add your code below
	if (root == nullptr) return true;
	return countHeight(root->left) == countHeight(root->right);	

}


int countHeight(Node *curr) {
	if (curr == nullptr) return 0;
	if (curr->right == nullptr && curr->left == nullptr) return 0;
	return 1+countHeight(curr->right)+countHeight(curr->left);

}


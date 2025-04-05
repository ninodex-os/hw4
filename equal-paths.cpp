#ifndef RECCHECK
//if you want to add any #includes like <iostream> you must do them here (before the next endif)
#include "equal-paths.h"
using namespace std;
#endif





// You may add any prototypes of helper functions here

int checkDepth(Node* node) {
    if (!node->left && !node->right) {
        return 0;
    }

    int leftDepth = -1;
    if (node->left) {
        leftDepth = checkDepth(node->left);
        if (leftDepth == -1) return -1;
    }

    int rightDepth = -1;
    if (node->right) {
        rightDepth = checkDepth(node->right);
        if (rightDepth == -1) return -1;
    }

    if (node->left && node->right) {
        if (leftDepth != rightDepth) return -1;
        return leftDepth + 1;
    } else if (node->left) {
        return leftDepth + 1;
    } else {
        return rightDepth + 1;
    }
  }


bool equalPaths(Node* root) {
    if (!root) return true;
    return checkDepth(root) != -1;
}



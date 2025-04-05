#include "equal-paths.h"
#include <iostream>
using namespace std;

Node* createLeaf() {
    Node* leaf = new Node{0, nullptr, nullptr};
    leaf->left = nullptr;
    leaf->right = nullptr;
    return leaf;
}

int main() {
    // Test 1: Empty tree (should return true)
    bool result1 = equalPaths(nullptr);
    cout << "Test 1 (Empty tree): " << (result1 ? "Passed" : "Failed") << endl;

    // Test 2: Single node (should return true)
    Node* root2 = createLeaf();
    bool result2 = equalPaths(root2);
    cout << "Test 2 (Single node): " << (result2 ? "Passed" : "Failed") << endl;

    /** Test 3: Tree with equal paths
    *      A
    *     / \
    *     B   C
    */
    Node* root3 = createLeaf();
    root3->left = createLeaf();
    root3->right = createLeaf();
    bool result3 = equalPaths(root3);
    cout << "Test 3 (Equal paths): " << (result3 ? "Passed" : "Failed") << endl;

    /** Test 4: Tree with unequal paths
    *       A
    *     /
    *     B
    *    /
    *   C
    */
    Node* root4 = createLeaf();
    root4->left = createLeaf();
    root4->right = createLeaf();
    root4->left->left = createLeaf();
    bool result4 = equalPaths(root4);
    cout << "Test 4 (Unequal paths): " << (!result4 ? "Passed" : "Failed") << endl;

    return 0;
}

// BPlusTree.cpp
#include "BPlusTree.h"

// Node constructor
Node::Node(bool leaf) : isLeaf(leaf), parent(nullptr) {
    for (int i = 0; i < MAX_KEYS; ++i) keys[i] = 0;
}

// LeafNode constructor
LeafNode::LeafNode() : Node(true), next(nullptr) {
    for (int i = 0; i < MAX_KEYS; ++i) values[i] = 0;
}

// InternalNode constructor
InternalNode::InternalNode() : Node(false) {
    for (int i = 0; i < MAX_KEYS + 1; ++i) children[i] = nullptr;
}

// BPlusTree constructor
BPlusTree::BPlusTree() : root(nullptr) {}

// Insert into a B+ Tree
void BPlusTree::insert(int key, int value) {
    if (!root) {
        root = new LeafNode();
    }
    insertRecursive(root, key, value);
}
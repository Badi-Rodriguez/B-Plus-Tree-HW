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

void BPlusTree::insertRecursive(Node* node, int key, int value) {
    if (node->isLeaf) {
        LeafNode* leaf = static_cast<LeafNode*>(node);
        int i;
        for (i = 0; i < MAX_KEYS && leaf->keys[i] != 0 && key > leaf->keys[i]; ++i);

        // Shift keys and values to make space for the new key-value pair
        for (int j = MAX_KEYS - 1; j > i; --j) {
            leaf->keys[j] = leaf->keys[j - 1];
            leaf->values[j] = leaf->values[j - 1];
        }

        // Insert key and value
        leaf->keys[i] = key;
        leaf->values[i] = value;

        if (leaf->keys[MAX_KEYS - 1] != 0) { // Leaf node overflow
            splitLeafNode(leaf);
        }
    } else {
        InternalNode* internal = static_cast<InternalNode*>(node);
        int i;
        for (i = 0; i < MAX_KEYS && internal->keys[i] != 0 && key > internal->keys[i]; ++i);

        insertRecursive(internal->children[i], key, value);

        if (internal->keys[MAX_KEYS - 1] != 0) { // Internal node overflow
            splitInternalNode(internal);
        }
    }
}
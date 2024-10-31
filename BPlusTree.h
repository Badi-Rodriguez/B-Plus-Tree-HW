// BPlusTree.h
#ifndef BPLUSTREE_H
#define BPLUSTREE_H

#include <iostream>

const int MAX_KEYS = 3; // Adjust this for a different order B+ Tree

// Basic Node structure, supporting both internal and leaf nodes
struct Node {
    bool isLeaf;
    int keys[MAX_KEYS];
    Node* parent;

    Node(bool leaf = false);
};

struct LeafNode : public Node {
    int values[MAX_KEYS];
    LeafNode* next;

    LeafNode();
};

struct InternalNode : public Node {
    Node* children[MAX_KEYS + 1];

    InternalNode();
};

class BPlusTree {
private:
    Node* root;

    // Helper functions
    void splitLeafNode(LeafNode* leaf);
    void splitInternalNode(InternalNode* internal);
    void insertRecursive(Node* node, int key, int value);

public:
    BPlusTree();

    void insert(int key, int value);
    int search(int key);
    void remove(int key);
};

#endif // BPLUSTREE_H

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

void BPlusTree::splitLeafNode(LeafNode* leaf) {
    LeafNode* newLeaf = new LeafNode();
    int mid = MAX_KEYS / 2;

    for (int i = mid; i < MAX_KEYS; ++i) {
        newLeaf->keys[i - mid] = leaf->keys[i];
        newLeaf->values[i - mid] = leaf->values[i];
        leaf->keys[i] = 0;
        leaf->values[i] = 0;
    }

    newLeaf->next = leaf->next;
    leaf->next = newLeaf;

    // Handle parent
    if (!leaf->parent) {
        InternalNode* newRoot = new InternalNode();
        newRoot->keys[0] = newLeaf->keys[0];
        newRoot->children[0] = leaf;
        newRoot->children[1] = newLeaf;
        leaf->parent = newLeaf->parent = newRoot;
        root = newRoot;
    } else {
        InternalNode* parent = static_cast<InternalNode*>(leaf->parent);
        int i;
        for (i = 0; i < MAX_KEYS && parent->keys[i] != 0 && newLeaf->keys[0] > parent->keys[i]; ++i);

        for (int j = MAX_KEYS; j > i; --j) {
            parent->keys[j] = parent->keys[j - 1];
            parent->children[j + 1] = parent->children[j];
        }

        parent->keys[i] = newLeaf->keys[0];
        parent->children[i + 1] = newLeaf;
        newLeaf->parent = parent;
    }
}

void BPlusTree::splitInternalNode(InternalNode* internal) {
    InternalNode* newInternal = new InternalNode();
    int mid = MAX_KEYS / 2;

    for (int i = mid + 1, j = 0; i < MAX_KEYS; ++i, ++j) {
        newInternal->keys[j] = internal->keys[i];
        newInternal->children[j] = internal->children[i];
        internal->keys[i] = 0;
        internal->children[i] = nullptr;
    }
    newInternal->children[mid] = internal->children[MAX_KEYS];

    if (!internal->parent) {
        InternalNode* newRoot = new InternalNode();
        newRoot->keys[0] = internal->keys[mid];
        newRoot->children[0] = internal;
        newRoot->children[1] = newInternal;
        internal->parent = newInternal->parent = newRoot;
        root = newRoot;
    } else {
        // Propagate the split up to the parent
    }
}

int BPlusTree::search(int key) {
    Node* current = root;
    while (current && !current->isLeaf) {
        InternalNode* internal = static_cast<InternalNode*>(current);
        int i;
        for (i = 0; i < MAX_KEYS && internal->keys[i] != 0 && key >= internal->keys[i]; ++i);
        current = internal->children[i];
    }

    if (current) {
        LeafNode* leaf = static_cast<LeafNode*>(current);
        for (int i = 0; i < MAX_KEYS && leaf->keys[i] != 0; ++i) {
            if (leaf->keys[i] == key) {
                return leaf->values[i];
            }
        }
    }
    return -1; // Key not found
}

void BPlusTree::remove(int key) {
    // Step 1: Traverse to the leaf node
    Node* current = root;
    while (current && !current->isLeaf) {
        InternalNode* internal = static_cast<InternalNode*>(current);
        int i = 0;
        while (i < MAX_KEYS && internal->keys[i] != 0 && key >= internal->keys[i]) {
            i++;
        }
        current = internal->children[i];
    }

    // Step 2: Look for the key in the leaf node
    if (current && current->isLeaf) {
        LeafNode* leaf = static_cast<LeafNode*>(current);
        for (int i = 0; i < MAX_KEYS; i++) {
            if (leaf->keys[i] == key) {  // Key found
                leaf->keys[i] = 0;      // "Delete" the key by setting it to 0
                leaf->values[i] = 0;    // Also "delete" the value by setting it to 0
                break;
            }
        }
    }
}

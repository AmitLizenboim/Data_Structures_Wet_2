
#ifndef DATA_STRUCTURES_WET_2_AVLRANKTREE_H
#define DATA_STRUCTURES_WET_2_AVLRANKTREE_H

#endif //DATA_STRUCTURES_WET_2_AVLRANKTREE_H

#pragma once

template <class T>
struct Node {
    T* data;
    Node* left;
    Node* right;
    int height;
    int rank; // addition for rank tree

    explicit Node(T* data, int rank = 1) : data(data), left(nullptr), right(nullptr),
        height(1), rank(rank) {} // O(1)
};

template <class T>
class AVLTree {
private:
    Node<T>* root = nullptr;

    void recursiveDeletion(Node<T>* root) {
        // function for AVLTree destructor, O(number of Nodes in the tree)
        if (root == nullptr)
            return;
        recursiveDeletion(root->left);
        recursiveDeletion(root->right);
        delete root->data;
        delete root;
    }

    void recursiveNodesDeletion(Node<T>* root) {
        // function for AVLTree destructor, O(number of Nodes in the tree)
        if (root == nullptr)
            return;
        recursiveNodesDeletion(root->left);
        recursiveNodesDeletion(root->right);
        delete root;
    }

    int getHeight(Node<T>* node) {
        // function returns the Node's height, O(1)
        if (node == nullptr)
            return 0;
        return node->height;
    }

    int getRank(Node<T>* node) {
        // function returns the Node's rank, O(1)
        if (node == nullptr)
            return 0;
        return node->rank;
    }

    int balanceFactor(Node<T>* node) {
        // function returns the Node's balance factor, O(1)
        if (node == nullptr)
            return 0;
        return getHeight(node->left) - getHeight(node->right);
    }

    int max(int x, int y) {
        // function returns the maximum of the 2 values, O(1)
        if (x < y)
            return y;
        return x;
    }

    void updateNode(Node<T>* node) { // function updates the Node's height and rank value
        if (node == nullptr)
            return;
        node->height = max(getHeight(node->left), getHeight(node->right)) + 1;
        node->rank = getRank(node->left) + getRank(node->right) + 1;
    }

    Node<T>* leftRotation(Node<T>* A) {
        // function for RR rotation, O(1)
        Node<T>* C = A->right;
        Node<T>* B = C->left;
        T* temp = A->data;
        A->data = C->data;
        A->right = C-> right;
        delete C;
        Node<T>* D = A->left;
        A->left = new Node<T>(temp);
        A->left->left = D;
        A->left->right = B;
        // update Nodes
        updateNode(A->left);
        updateNode(A);
        return A;
    }

    Node<T>* rightRotation(Node<T>* C) {
        // function for LL rotation, O(1)
        Node<T>* A = C->left;
        Node<T>* B = A->right;
        T* temp = C->data;
        C->data = A->data;
        C->left = A-> left;
        delete A;
        Node<T>* D = C->right;
        C->right = new Node<T>(temp);
        C->right->right = D;
        C->right->left = B;
        // update Nodes
        updateNode(C->right);
        updateNode(C);
        return C;
    }

    Node<T>* find(int key, Node<T>* node) {
        // AVL search implementation, O(log(Nodes in the tree))
        if (node == nullptr)
            return nullptr;
        if (*node->data > key)
            return find(key, node->left);
        if (*node->data < key)
            return find(key, node->right);
        // else root->data == key
        return node;
    }

    Node<T>* insert(T* data, int key, int value, Node<T>* node) {
        // AVL insertion implementation, O(log(Nodes in the tree))
        if (node == nullptr)
            return node;
        if (*node->data > key)
            if(insert(data, key, value, node->left) == nullptr)
                node->left = new Node<T>(data, value);
        if (*node->data < key)
            if(insert(data, key, value, node->right) == nullptr)
                node->right = new Node<T>(data, value);
        // updating Node
        updateNode(node);
        // balancing tree
        int balance = balanceFactor(node);
        if (balance > 1 && balanceFactor(node->left) == -1) { // LR rotation needed
            leftRotation(node->left);
            return rightRotation(node);
        }
        if (balance < -1 && balanceFactor(node->right) == 1) { // RL rotation needed
            rightRotation(node->right);
            return leftRotation(node);
        }
        if (balance > 1) // LL rotation needed
            return rightRotation(node);
        if (balance < -1) // RR rotation needed
            return leftRotation(node);
        // else the tree stays balanced
        return node;
    }

    Node<T>* remove(int key, Node<T>* node) {
        // AVL removal implementation, O(log(Nodes in the tree))
        if (node == nullptr)
            return node;
        if (*node->data > key) {
            if(remove(key, node->left) == nullptr)
                return nullptr;
        } else if (*node->data < key) {
            if(remove(key, node->right) == nullptr)
                return nullptr;
        } else if (node->left != nullptr && node->right != nullptr) {
            // switch with the following node (inorder)
                Node<T>* nextNode = node->right;
                while(nextNode->left != nullptr)
                    nextNode = nextNode->left;
                T* temp = node->data;
                node->data = nextNode->data;
                nextNode->data = temp;
                remove(key, node->right);
        } else {
            return node;
        }
        //remove the node
        Node<T>* deletedNode;
        if(node->left != nullptr && !(*node->left->data < key) && !(*node->left->data > key)) {
            deletedNode = node->left;
            node->left = deletedNode->right == nullptr ? deletedNode->left : deletedNode->right;
            delete deletedNode;
        } else if (node->right != nullptr && !(*node->right->data < key) && !(*node->right->data > key)) {
            deletedNode = node->right;
            node->right = deletedNode->right == nullptr ? deletedNode->left : deletedNode->right;
            delete deletedNode;
        }
        // update Node
        updateNode(node);
        // balancing tree
        int balance = balanceFactor(node);
        if (balance > 1 && balanceFactor(node->left) == -1) { // LR rotation needed
            leftRotation(node->left);
            return rightRotation(node);
        }
        if (balance < -1 && balanceFactor(node->right) == 1) { // RL rotation needed
            rightRotation(node->right);
            return leftRotation(node);
        }
        if (balance > 1) // LL rotation needed
            return rightRotation(node);
        if (balance < -1) // RR rotation needed
            return leftRotation(node);
        // else the tree stays balanced
        return node;
    }

public:
    AVLTree() = default;
    AVLTree(const AVLTree &tree) = delete;
    AVLTree &operator=(const AVLTree &tree) = delete;
    ~AVLTree() { recursiveDeletion(root); }

    void deleteNodes() {
        recursiveNodesDeletion(root);
        root = nullptr;
    }

    bool isEmpty() {
        return root == nullptr;
    }

    T* find(int key) {
        // AVL search implementation, O(log(Nodes in the tree))
        Node<T>* node = find(key, root);
        return node ? node->data : nullptr;
    }

    Node<T>* insert(T* data, int key, int value) {
        // AVL insertion implementation, O(log(Nodes in the tree))
        if (root == nullptr) {
            root = new Node<T>(data, value);
            return root;
        }
        return insert(data, key, value, root);
    }

    Node<T>* remove(int key) {
        // AVL removal implementation, O(log(Nodes in the tree))
        if (root == nullptr) {
            return root;
        }
        if (!(*root->data < key) && !(*root->data > key)) {
            Node<T>* oldRoot = root;
            if (root->right == nullptr) {
                root = oldRoot->left;
                delete oldRoot;
                return root;
            }
            if (root->left == nullptr) {
                root = oldRoot->right;
                delete oldRoot;
                return root;
            }
        }
        return remove(key, root);
    }

    T* getIthValue(int i) { // function returns the data of the ith Node in the tree
                               // O(log(Nodes in the tree))
        if (root == nullptr || i < 1 || i > root->rank)
            return nullptr; // invalid inputs
        Node<T>* current = root;
        while (current != nullptr) {
            int leftSubTreeSize = getRank(current->left);
            if (leftSubTreeSize == i - 1) {
                return current->data; // Node found
            }
            if (leftSubTreeSize >= i) {
                current = current->left; // keep searching in left subtree
            }
            else { // keep searching in right subtree
                i -= leftSubTreeSize + 1;
                current = current->right;
            }
        }
        return nullptr;
    }
};
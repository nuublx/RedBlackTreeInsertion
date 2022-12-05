#include<bits/stdc++.h>
using namespace std;

class RB {
private:
    class Node {
    public:
        int data;
        Node *left;
        Node *right;
        char colour;
        Node *parent;

        Node(int data) {
            this->data = data; // only including data. not key
            this->left = nullptr; // left subtree
            this->right = nullptr; // right subtree
            this->colour = 'R'; // colour . either 'R' or 'B'
            this->parent = nullptr; // required at time of rechecking.
        }
    };

    void inorderTraversalHelper(Node *node) {
        if (node != nullptr) {
            inorderTraversalHelper(node->left);
            cout << node->data << " ";
            inorderTraversalHelper(node->right);
        }
    }
    // this function performs left rotation
    Node *rotateLeft(Node *node) {
        Node *x = node->right;
        Node *y = x->left;
        x->left = node;
        node->right = y;
        node->parent = x; // parent resetting is also important.
        if (y != nullptr)
            y->parent = node;
        return (x);
    }
    // rotations are done if flags are true.
    bool ll = false;
    bool rr = false;
    bool lr = false;
    bool rl = false;
    //this function performs right rotation
    Node *rotateRight(Node *node) {
        Node *x = node->left;
        Node *y = x->right;
        x->right = node;
        node->left = y;
        node->parent = x;
        if (y != nullptr)
            y->parent = node;
        return (x);
    }
    // helper function for insertion. Actually this function performs all tasks in single pass only.
    Node *insertHelp(Node *root, int data) {
        // f is true when RED RED conflict is there.
        bool f = false;

        //recursive calls to insert at proper position according to BST properties.
        if (root == nullptr)
            return (new Node(data));
        else if (data < root->data) {
            root->left = insertHelp(root->left, data);
            root->left->parent = root;
            if (root != this->root) {
                if (root->colour == 'R' && root->left->colour == 'R')
                    f = true;
            }
        } else {
            root->right = insertHelp(root->right, data);
            root->right->parent = root;
            if (root != this->root) {
                if (root->colour == 'R' && root->right->colour == 'R')
                    f = true;
            }
            // at the same time of insertion, we are also assigning parent nodes
            // also we are checking for RED RED conflicts
        }

        if (this->ll) // for left rotate.
        {
            root = rotateLeft(root);
            root->colour = 'B';
            root->left->colour = 'R';
            this->ll = false;
        } else if (this->rr) // for right rotate
        {
            root = rotateRight(root);
            root->colour = 'B';
            root->right->colour = 'R';
            this->rr = false;
        } else if (this->rl) // for right and then left
        {
            root->right = rotateRight(root->right);
            root->right->parent = root;
            root = rotateLeft(root);
            root->colour = 'B';
            root->left->colour = 'R';

            this->rl = false;
        } else if (this->lr) // for left and then right.
        {
            root->left = rotateLeft(root->left);
            root->left->parent = root;
            root = rotateRight(root);
            root->colour = 'B';
            root->right->colour = 'R';
            this->lr = false;
        }
        // when rotation and recolouring is done flags are reset.
        // Now lets take care of RED RED conflict
        if (f) {
            if (root->parent->right == root) // to check which child is the current node of its parent
            {
                if (root->parent->left == nullptr ||
                    root->parent->left->colour == 'B') // case when parent's sibling is black
                {// perform certaing rotation and recolouring. This will be done while backtracking. Hence setting up respective flags.
                    if (root->left != nullptr && root->left->colour == 'R')
                        this->rl = true;
                    else if (root->right != nullptr && root->right->colour == 'R')
                        this->ll = true;
                } else // case when parent's sibling is red
                {
                    root->parent->left->colour = 'B';
                    root->colour = 'B';
                    if (root->parent != this->root)
                        root->parent->colour = 'R';
                }
            } else {
                if (root->parent->right == nullptr || root->parent->right->colour == 'B') {
                    if (root->left != nullptr && root->left->colour == 'R')
                        this->rr = true;
                    else if (root->right != nullptr && root->right->colour == 'R')
                        this->lr = true;
                } else {
                    root->parent->right->colour = 'B';
                    root->colour = 'B';
                    if (root->parent != this->root)
                        root->parent->colour = 'R';
                }
            }
            f = false;
        }
        return (root);
    }
public:
    Node *root;
    RB() {
        root = nullptr;
    }

    // function to insert data into tree.
    void insert(int data) {
        if (this->root == nullptr) {
            this->root = new Node(data);
            this->root->colour = 'B';
        } else
            this->root = insertHelp(this->root, data);
    }

    //function to print inorder traversal
    void inorderTraversal() {
        inorderTraversalHelper(this->root);
    }
};

int main() {
    RB red;
    red.insert(2);
    red.insert(3);
    red.insert(4);
    red.insert(5);
    red.insert(6);
    red.inorderTraversal();
}
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "treemap.h"

typedef struct TreeNode TreeNode;


struct TreeNode {
    Pair* pair;
    TreeNode * left;
    TreeNode * right;
    TreeNode * parent;
};

struct TreeMap {
    TreeNode * root;
    TreeNode * current;
    int (*lower_than) (void* key1, void* key2);
};

int is_equal(TreeMap* tree, void* key1, void* key2){
    if(tree->lower_than(key1,key2)==0 &&  
        tree->lower_than(key2,key1)==0) return 1;
    else return 0;
}


TreeNode * createTreeNode(void* key, void * value) {
    TreeNode * new = (TreeNode *)malloc(sizeof(TreeNode));
    if (new == NULL) return NULL;
    new->pair = (Pair *)malloc(sizeof(Pair));
    new->pair->key = key;
    new->pair->value = value;
    new->parent = new->left = new->right = NULL;
    return new;
}

TreeMap * createTreeMap(int (*lower_than) (void* key1, void* key2)) {

    TreeMap * new = (TreeMap *)malloc(sizeof(TreeMap));
    
    new->root = NULL;
    new->current = NULL;
    new->lower_than = lower_than;
    
    return new;
}


void insertTreeMap(TreeMap * tree, void* key, void * value) {
    if (tree == NULL || key == NULL) return; 
    if (searchTreeMap(tree, key) != NULL) return;  
    TreeNode *new = createTreeNode(key, value);
    if (new == NULL) return;  
    TreeNode *aux = tree->root;
    TreeNode *parent = NULL;
    while (aux != NULL) {
        parent = aux;
        if (tree->lower_than(key, aux->pair->key)) {
            aux = aux->left;
        } else {
            aux = aux->right;
        }
    }
    new->parent = parent;
    if (parent == NULL) {
        tree->root = new;
    } else {
        if (tree->lower_than(key, parent->pair->key)) {
            parent->left = new;
        } else {
            parent->right = new;
        }
    }
    tree->current = new;
}
    
 
   
TreeNode * minimum(TreeNode * x){
    while (x->left != NULL){
        x = x->left;
    }
    
    return x;                                                         }

// cambiar
void remove_node(TreeMap *tree, TreeNode *z) {
    if (z == NULL) return;

    TreeNode *y, *x;

    // Node z has no left child
    if (z->left == NULL) {
        x = z->right;
        // Node z is the root
        if (z->parent == NULL) {
            tree->root = x;
        } else if (z == z->parent->left) { // Node z is a left child
            z->parent->left = x;
        } else { // Node z is a right child
            z->parent->right = x;
        }
        if (x != NULL) {
            x->parent = z->parent;
        }
    }
    // Node z has no right child
    else if (z->right == NULL) {
        x = z->left;
        // Node z is the root
        if (z->parent == NULL) {
            tree->root = x;
        } else if (z == z->parent->left) { // Node z is a left child
            z->parent->left = x;
        } else { // Node z is a right child
            z->parent->right = x;
        }
        x->parent = z->parent;
    }
    // Node z has two children
    else {
        // Find the minimum node from the right subtree
        y = z->right;
        while (y->left != NULL) {
            y = y->left;
        }

        // y is successor of z
        x = y->right;
        if (y->parent != z) {
            if (x != NULL) {
                x->parent = y->parent;
            }
            y->parent->left = x;  // y is assumed to be the left child
            y->right = z->right;
            z->right->parent = y;
        } else {
            x = y;
        }

        // Replace z with y
        if (z->parent == NULL) {
            tree->root = y;
        } else if (z == z->parent->left) {
            z->parent->left = y;
        } else {
            z->parent->right = y;
        }
        y->parent = z->parent;
        y->left = z->left;
        z->left->parent = y;
    }

    // Free the removed node
    free(z);
}
    



void eraseTreeMap(TreeMap * tree, void* key){
    if (tree == NULL || tree->root == NULL) return;
    if (searchTreeMap(tree, key) == NULL) return;
    //TreeNode* node = tree->current;
    //emoveNode(tree, node);
}




Pair * searchTreeMap(TreeMap * tree, void* key) {
    if (tree == NULL || tree->root == NULL) return NULL;
    TreeNode * aux = tree->root;
    while (aux != NULL){
        if (is_equal(tree, key, aux->pair->key)){
            tree->current = aux;
            return aux->pair;
        } 
        if (tree->lower_than(key, aux->pair->key) == 1){
            aux = aux->left;
        } else {
            aux = aux->right;
        }
    }
    return NULL;
}


Pair * upperBound(TreeMap * tree, void* key) {
    if (tree == NULL || tree->root == NULL) return NULL;
    TreeNode * aux = tree->root;
    TreeNode * ub = NULL;
    while (aux != NULL){
        if (is_equal(tree, key, aux->pair->key)){
            return aux->pair;
        } else if (tree->lower_than(key, aux->pair->key) == 1){
            ub = aux;
            aux = aux->left;   
        } else {
            aux = aux->right;
        }
    }
    if (ub != NULL){
        return ub->pair;
    } 
    return NULL;
}


Pair * firstTreeMap(TreeMap * tree){
    if (tree == NULL || tree->root == NULL) return NULL;
    tree->current = minimum(tree->root);
    
    return tree->current->pair;
}


Pair * nextTreeMap(TreeMap * tree) {
    if (tree == NULL || tree->current == NULL) return NULL;
    TreeNode * aux = tree->current;
    if (aux->right != NULL){
        tree->current = minimum(aux->right);
        return tree->current->pair;
    } 
    TreeNode * parent = aux->parent;
    while (parent != NULL && aux == parent->right){
        aux = parent;
        parent = parent->parent;
    }
    tree->current = parent;
    if (parent != NULL) return parent->pair;
    return NULL;
}

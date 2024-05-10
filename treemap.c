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
    return x;
}

void removeNode(TreeMap * tree, TreeNode * node){
    if (node == NULL) return;

    TreeNode *y = node;
    TreeNode *x;
    Pair *pair;
    int n;
    if (node->left == NULL){
        x = node->right;
        n = 1;
    } else {
        x = minimum(node->right);
        n = 2;
    } 
    if (x != NULL){
        pair = x->pair;
        y->pair = pair;
        y->pair->key = pair->key;
        y->pair->value = pair->value;
        y->pair = x->pair;
        y->pair->key = x->pair->key;
        y->pair->value = x->pair->value;
    }
    if (n == 1){
        y->right = x->right;
        if (x->right != NULL){
            x->right->parent = y;
        }
        
    }
    if (x != NULL){
        x->parent = node->parent;
    } else {
        x = node;
    }
    if (node->parent == NULL){
        tree->root = x;
    } else {
        if (node == node->parent->left){
            node->parent->left = x;
        } else {
            node->parent->right = x;
        }
    }
    if (n == 1){
        y->left = node->left;
    } else {
        if (n == 2){
            y->left = node->right;
        } else {
            y->right = node->right;
        }
    }
    if (y != node){
        pair = node->pair;
        node->pair = y->pair;
        y->pair = pair;
    } else {
        pair = node->pair;
        free(pair);
    }
    free(node);
    
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

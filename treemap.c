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
    new->lower_than = lower_than;
    new->root = NULL;
    new->current = NULL;
    return new;
}


void insertTreeMap(TreeMap * tree, void* key, void * value) {
    if (tree == NULL || tree->root == NULL) return;
    if (tree->root == NULL){
        tree->root = createTreeNode(key, value);
    }
    if (searchTreeMap(tree, key) != NULL) return;
    TreeNode * parent = NULL;
    TreeNode * current = tree->root;
    while (current != NULL){
        parent = current;
        if (tree->lower_than(key, current->pair->key) == 1){
            current = current->left;
        }
        else{
            current = current->right;
        }
    }
    TreeNode * new = createTreeNode(key, value);
    new->parent = parent;
    if (parent == NULL) tree->root = new;
    else{
        if (tree->lower_than(key, parent->pair->key) == 1){
            parent->left = new;
        }
        else{
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


void removeNode(TreeMap * tree, TreeNode* node) {
    if (node == NULL) return;

    if (node->left == NULL){
        if (node->parent == NULL){
            tree->root = node->right;
        }
        else if (node == node->parent->left){
            node->parent->left = node->right;
        }
        else{
            node->parent->right = node->right;
        }

        if (node->right != NULL){
            node->right->parent = node->parent;
        }
    }

    else if (node->right == NULL){
        if (node->parent == NULL){
            tree->root = node->left;
        }
        else if (node == node->parent->left){
            node->parent->left = node->left;
        }
        else{
            node->parent->right = node->left;
        }
        if (node->left != NULL){
            node->left->parent = node->parent;
        }
    }
    else {
        TreeNode * min = minimum(node->right);
        if (min->parent != node){
            if (min->right != NULL){
                min->right->parent = min->parent;
            }
            min->parent->left = min->right;
            min->right = node->right;
            min->right->parent = min;
        }
        if (node->parent == NULL){
            tree->root = min;
        }
        else if (node == node->parent->left){
            node->parent->left = min;
        }
        else{
            node->parent->right = min;
        }
        min->left = node->left;
        min->left->parent = min;
        min->parent = node->parent;
    }
    free(node->pair->key);

}

void eraseTreeMap(TreeMap * tree, void* key){
    if (tree == NULL || tree->root == NULL) return;
    if (searchTreeMap(tree, key) == NULL) return;
    TreeNode* node = tree->current;
    removeNode(tree, node);

}




Pair * searchTreeMap(TreeMap * tree, void* key) {
    if (tree == NULL || tree->root == NULL) return NULL;
    TreeNode * current = tree->root;
    while (current != NULL){
        if (is_equal(tree,key,current->pair->key) == 1){
            tree->current = current;
            return current->pair;
        }
        else if (tree->lower_than(key, current->pair->key) == 1){
            current = current->left;
        }
        else{
            current = current->right;
        }
    }
    return NULL;
}


Pair * upperBound(TreeMap * tree, void* key) {
    if (tree == NULL || tree->root == NULL) return NULL;
    TreeNode * current = tree->root;
    TreeNode * ub = NULL;
    while (current != NULL){
        if (is_equal(tree,key,current->pair->key))
            return current->pair;
        else if (tree->lower_than(key, current->pair->key) == 1){
            ub = current;
            current = current->left;
        }
        else{
            current = current->right;
        }
    }
    if (ub != NULL) return ub->pair;
    return NULL;
}

Pair * firstTreeMap(TreeMap * tree) {
    if (tree == NULL || tree->root == NULL) return NULL;
    TreeNode * current = tree->root;
    while (current->left != NULL){
        current = current->left;
    }
    tree->current = current;
    return current->pair;
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

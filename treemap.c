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

void removeNode(TreeMap *tree, TreeNode *node) {
    if (node == NULL) {
        return; // Si el nodo es nulo, simplemente retornar.
    }

    // Caso 1: Nodo sin hijos
    if (node->left == NULL && node->right == NULL) {
        if (node->parent == NULL) {
            tree->root = NULL; // El nodo es la raíz y no tiene hijos.
        } else {
            // Desvincular el nodo de su padre
            if (node == node->parent->left) {
                node->parent->left = NULL;
            } else {
                node->parent->right = NULL;
            }
        }
    }
    // Caso 2: Nodo con un hijo
    else if (node->left == NULL || node->right == NULL) {
        TreeNode *child = (node->left != NULL) ? node->left : node->right;
        if (node->parent == NULL) {
            tree->root = child; // El nodo es la raíz y tiene un solo hijo.
        } else {
            // Reconectar el hijo directamente al abuelo
            if (node == node->parent->left) {
                node->parent->left = child;
            } else {
                node->parent->right = child;
            }
        }
        child->parent = node->parent; // Actualizar el parent del hijo.
    }
    // Caso 3: Nodo con dos hijos
    else {
        TreeNode *minNode = minimum(node->right); // Encontrar el mínimo en el subárbol derecho.
        // Copiar los datos de minNode a node.
        node->pair->key = minNode->pair->key;
        node->pair->value = minNode->pair->value;
        // Eliminar el nodo mínimo.
        removeNode(tree, minNode);
    }

    // Liberar memoria del nodo original solo si no tiene dos hijos
    // (Si tiene dos hijos, sólo se elimina el nodo mínimo, no el nodo original)
    if (node->left == NULL || node->right == NULL) {
        free(node->pair->key);
        free(node->pair->value);
        free(node->pair);
        free(node);
    }
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

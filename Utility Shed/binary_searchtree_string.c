/*
 * binary_searchtree_string.c
 *
 *  Created on: Aug 25, 2023
 *      Author: user
 */

// SIDE NOTE: This is a binary tree like structure where the node value is a string

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "binary_searchtree_string.h"
#include "constants.h"



// Function to create a new tree node
TreeNode* createNode(const char* key, const char* value) {
    TreeNode* newNode = (TreeNode*)malloc(sizeof(TreeNode));
    if (newNode) {
        strncpy(newNode->key, key, MAX_MACRO_NAME_LENGTH - 1);
        newNode->key[MAX_MACRO_NAME_LENGTH - 1] = '\0'; // Null-terminate the key string
        strncpy(newNode->value, value, MAX_MACRO_CONTENT_LENGTH - 1);
        newNode->value[MAX_MACRO_CONTENT_LENGTH - 1] = '\0'; // Null-terminate the value string
        newNode->left = NULL;
        newNode->right = NULL;
    }
    return newNode;
}

// Function to insert a new node into the tree
TreeNode* insertNode(TreeNode* root, const char* key, const char* value) {
    if (root == NULL) {
        return createNode(key, value);
    }

    int cmpResult = strcmp(key, root->key);
    if (cmpResult < 0) {
        root->left = insertNode(root->left, key, value);
    } else if (cmpResult > 0) {
        root->right = insertNode(root->right, key, value);
    }

    return root;
}

// Function to search for a node with a given key
TreeNode* searchNode(TreeNode* root, const char* key) {
    if (root == NULL || strcmp(root->key, key) == 0) {
        return root;
    }

    if (strcmp(key, root->key) < 0) {
        return searchNode(root->left, key);
    } else {
        return searchNode(root->right, key);
    }
}

// Function to find the node with minimum key value
TreeNode* findMinNode(TreeNode* node) {
    while (node->left != NULL) {
        node = node->left;
    }
    return node;
}

// Function to delete a node with a given key
TreeNode* deleteNode(TreeNode* root, const char* key) {
    if (root == NULL) {
        return root;
    }

    int cmpResult = strcmp(key, root->key);
    if (cmpResult < 0) {
        root->left = deleteNode(root->left, key);
    } else if (cmpResult > 0) {
        root->right = deleteNode(root->right, key);
    } else {
        if (root->left == NULL) {
            TreeNode* temp = root->right;
            free(root);
            return temp;
        } else if (root->right == NULL) {
            TreeNode* temp = root->left;
            free(root);
            return temp;
        }

        TreeNode* temp = findMinNode(root->right);
        strcpy(root->key, temp->key);
        strcpy(root->value, temp->value);
        root->right = deleteNode(root->right, temp->key);
    }

    return root;
}

// Function to free the memory used by the tree
void freeTree(TreeNode* root) {
    if (root == NULL) {
        return;
    }
    freeTree(root->left);
    freeTree(root->right);
    free(root);
}

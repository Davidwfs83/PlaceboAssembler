/*
 * binary_searchtree_string.h
 *
 *  Created on: Aug 25, 2023
 *      Author: user
 */

#ifndef BINARY_SEARCHTREE_STRING_H_
#define BINARY_SEARCHTREE_STRING_H_

#include "constants.h"

// Define the TreeNode structure
typedef struct TreeNode {
    char key[MAX_MACRO_NAME_LENGTH];
    char value[MAX_MACRO_CONTENT_LENGTH];
    struct TreeNode *left;
    struct TreeNode *right;
} TreeNode;

TreeNode* createNode(const char* key, const char* value);
TreeNode* insertNode(TreeNode* root, const char* key, const char* value);
TreeNode* searchNode(TreeNode* root, const char* key);
TreeNode* deleteNode(TreeNode* root, const char* key);
void freeTree(TreeNode* root);


#endif /* BINARY_SEARCHTREE_STRING_H_ */

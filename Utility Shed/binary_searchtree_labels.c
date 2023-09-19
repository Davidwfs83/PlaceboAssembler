#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "binary_searchtree_labels.h"
#include "constants.h"



LabelObj *createLabelObj(const char *labelName, LabelType labelType) {
    LabelObj *newLabel = (LabelObj *)malloc(sizeof(LabelObj));
    if (newLabel) {
        newLabel->labelName = strdup(labelName);
        newLabel->labelType = labelType;
        newLabel->labelVal = 0;
    }
    return newLabel;
}

ITreeNode *IcreateNode(const char *key, const LabelObj value) {
    ITreeNode *newNode = (ITreeNode *)malloc(sizeof(ITreeNode));
    if (newNode) {
        strncpy(newNode->key, key, MAX_LABEL_NAME_LENGTH - 1);
        newNode->key[MAX_LABEL_NAME_LENGTH - 1] = '\0';

        newNode->value = (LabelObj *)malloc(sizeof(LabelObj));
        if (newNode->value) {
            newNode->value->labelName = strdup(value.labelName);
            newNode->value->labelType = value.labelType;
            newNode->value->labelVal = value.labelVal;
        } else {
            free(newNode);
            return NULL;
        }

        newNode->left = NULL;
        newNode->right = NULL;
    }
    return newNode;
}

void IaddValueToAllNodes(ITreeNode* root, int valueToAdd) {
    if (root == NULL) {
        return;
    }
    root->value->labelVal += valueToAdd;
    IaddValueToAllNodes(root->left, valueToAdd);
    IaddValueToAllNodes(root->right, valueToAdd);
}

ITreeNode* IinsertNode(ITreeNode* root, const char* key, LabelObj value) {
    if (root == NULL) {
        return IcreateNode(key, value);
    }

    int cmpResult = strcmp(key, root->key);
    if (cmpResult < 0) {
        root->left = IinsertNode(root->left, key, value);
    } else if (cmpResult > 0) {
        root->right = IinsertNode(root->right, key, value);
    }

    return root;
}

ITreeNode* IsearchNode(ITreeNode* root, const char* key) {
    if (root == NULL || strcmp(root->key, key) == 0) {
        return root;
    }

    if (strcmp(key, root->key) < 0) {
        return IsearchNode(root->left, key);
    } else {
        return IsearchNode(root->right, key);
    }
}

ITreeNode* IfindMinNode(ITreeNode* node) {
    while (node->left != NULL) {
        node = node->left;
    }
    return node;
}

ITreeNode *IdeleteNode(ITreeNode *root, const char *key) {
    if (root == NULL) {
        return root;
    }

    int cmpResult = strcmp(key, root->key);
    if (cmpResult < 0) {
        root->left = IdeleteNode(root->left, key);
    } else if (cmpResult > 0) {
        root->right = IdeleteNode(root->right, key);
    } else {
        if (root->left == NULL) {
            ITreeNode *temp = root->right;
            free(root->value->labelName);
            free(root->value);
            free(root);
            return temp;
        } else if (root->right == NULL) {
            ITreeNode *temp = root->left;
            free(root->value->labelName);
            free(root->value);
            free(root);
            return temp;
        }

        ITreeNode *temp = IfindMinNode(root->right);
        strcpy(root->key, temp->key);
        LabelObj *tempValue = root->value;
        root->value = temp->value;
        temp->value = tempValue;

        root->right = IdeleteNode(root->right, temp->key);
    }

    return root;
}

void freeLabelObj(LabelObj *label) {
    if (label) {
        free(label->labelName);
        free(label);
    }
}

void IfreeTree(ITreeNode *root) {
    if (root == NULL) {
        return;
    }
    IfreeTree(root->left);
    IfreeTree(root->right);
    freeLabelObj(root->value);
    free(root);
}

// Function to merge two trees
ITreeNode* mergeTrees(ITreeNode* tree1, ITreeNode* tree2) {
    if (tree2 == NULL) {
        return tree1;
    }
    if (tree1 == NULL) {
        return tree2;
    }

    // Insert the root node of tree2 into tree1
    tree1 = IinsertNode(tree1, tree2->key, *(tree2->value));

    // Recursively merge the left and right subtrees
    tree1->left = mergeTrees(tree1->left, tree2->left);
    tree1->right = mergeTrees(tree1->right, tree2->right);

    // Free the memory for the root node of tree2
    IfreeTree(tree2);

    return tree1;
}

#ifndef BINARY_SEARCHTREE_LABELS_H
#define BINARY_SEARCHTREE_LABELS_H
#include "constants.h"

typedef struct ITreeNode {
    char key[MAX_LABEL_NAME_LENGTH];
    LabelObj *value;
    struct ITreeNode* left;
    struct ITreeNode* right;
} ITreeNode;

// Function prototypes
ITreeNode* IcreateNode(const char* key, LabelObj value);
ITreeNode* IinsertNode(ITreeNode* root, const char* key, LabelObj value);
ITreeNode* IsearchNode(ITreeNode* root, const char* key);
ITreeNode* IfindMinNode(ITreeNode* node);
ITreeNode* IdeleteNode(ITreeNode* root, const char* key);
void IfreeTree(ITreeNode* root);
void IaddValueToAllNodes(ITreeNode* root, int valueToAdd);
ITreeNode* mergeTrees(ITreeNode* tree1, ITreeNode* tree2);

#endif // BINARY_SEARCHTREE_LABELS_H

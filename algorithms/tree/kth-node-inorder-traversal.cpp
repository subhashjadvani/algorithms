/*
Find K-th node in inorder traversal.

Given a binary tree where a node also holds number of descendents and an int k .
Find the k-th node in its inorder traversal (the number if brackets is number
of children of each node).

Example:

	     25(4)
	    /    \
	 20(2)   13(0)
	 /  \
       1(0)  4(0) 

	[1, 20, 4, 25, 13]

public TreeNode getNode(TreeNode root, int k) {
	// todo
}

getNode(root, 3); // 4
getNode(root, 2); // 20

*/

#include <stdlib.h>
#include <stdio.h>

class TreeNode {
	TreeNode *kth_node;
public:
	int val;
	int descendents;
	TreeNode *left;
	TreeNode *right;

	TreeNode (int v, int d, TreeNode *l, TreeNode *r)
	{
		val = v;
		descendents = d;
		left = l;
		right = r;
	}

	int helper(TreeNode* root, int k, int rank) {
		//printf("%s: enter: root->val %d k %d rank %d\n", __func__, root->val, k, rank);

		if (!root->left && !root->right) {
			rank++;
			if (rank == k)
				kth_node = root;
			
			//printf("%s: exit: root->val %d k %d rank %d\n", __func__, root->val, k, rank);
			return rank;
		}
			
		int left_descendents = 0;
		if (root->left) {
			left_descendents = root->left->descendents + 1;
			if (k <= rank + left_descendents) { 
				rank = helper(root->left, k, rank);
			} else {
				rank = rank + left_descendents;
			}
		}

		if (k == rank + 1)
			kth_node = root;
			
		int right_descendents = 0;
		if (root->right) {
			right_descendents = root->right->descendents + 1;
			if (k <= rank + 1 + right_descendents)
				rank = helper(root->right, k, rank + 1);
			else
				rank = rank + 1 + right_descendents;
		}

		printf("%s: exit: root->val %d k %d rank %d\n", __func__, root->val, k, rank);
		return rank;
	}

	TreeNode* getNode(TreeNode* root, int k) {
		if (!root)
			return root;

		kth_node = NULL;
		helper(root, k, 0);
		return kth_node;
	}
};


int main() {
/*
	     25(4)
	    /    \
	 20(2)   13(0)
	 /  \
       1(0)  4(0) 
*/
	TreeNode *node1 = new TreeNode(1, 0, NULL, NULL);
	TreeNode *node4 = new TreeNode(4, 0, NULL, NULL);

	TreeNode *node20 = new TreeNode(20, 2, node1, node4);
	TreeNode *node13 = new TreeNode(13, 0, NULL, NULL);

	TreeNode *node25 = new TreeNode(25, 4, node20, node13);

	int k = 1;
	TreeNode *result = node25->getNode(node25, k);
	printf("getNode(node25, k=%d) -> %d\n", k, result->val);

	#if 0
	TreeNode *curr = node25;
	while (curr) {
		printf("%d\n", curr->val);
		curr = curr->left;
	}
	#endif

	k++;
	result = node25->getNode(node25, k);
	printf("getNode(node25, k=%d) -> %d\n", k, result->val);

	k++;
	result = node25->getNode(node25, k);
	printf("getNode(node25, k=%d) -> %d\n", k, result->val);

	k++;
	result = node25->getNode(node25, k);
	printf("getNode(node25, k=%d) -> %d\n", k, result->val);

	k++;
	result = node25->getNode(node25, k);
	printf("getNode(node25, k=%d) -> %d\n", k, result->val);
}

#ifndef SOLUTION_H
#define SOLUTION_H

/**
 * Definition for a binary tree node.
 * struct TreeNode {
 *     int val;
 *     TreeNode *left;
 *     TreeNode *right;
 *     TreeNode() : val(0), left(nullptr), right(nullptr) {}
 *     TreeNode(int x) : val(x), left(nullptr), right(nullptr) {}
 *     TreeNode(int x, TreeNode *left, TreeNode *right) : val(x), left(left), right(right) {}
 * };
 */
#include <vector>

class BinaryTreeIterator {
public:
  enum class TraverseOrder {
    PreOrder,
    InOrder,
    PostOrder
  };

  BinaryTreeIterator(TreeNode* root)
    : root_(root)
  {
    if (root) {
        InOrderLeftmost(root, true);
    }
  }

  bool hasNext() const {
    return !nodes_.empty();
  }

  std::pair<int,bool> next() {
    auto [next_root, is_left] = nodes_.top();
    nodes_.pop();

    if (next_root->right) {
      InOrderLeftmost(next_root->right, false);
    }

    return [next_root->val, is_left];
  }

private:
    void InOrderLeftmost(TreeNode* root, bool is_left) {
        while(root) {
            nodes_.push({root,is_left});
            root = root->left;
            is_left = true;
        }
    }

  TreeNode* root_ = nullptr;
  std::stack<pair<TreeNode*,bool>> nodes_;
};

vector<pair<int,bool>> inorderTraversal(TreeNode* root) {
    vector<int,bool> result;
    BinaryTreeIterator iter(root);
    while (iter.hasNext()) {
        result.push_back(iter.next());
    }

    return result;
}

class Solution {
public:
    bool isSymmetric(TreeNode* root) {
        vector<int> values = inorderTraversal(root);
        return std::equal(values.begin(), values.end(), values.rbegin());
    }
};

#endif // SOLUTION_H

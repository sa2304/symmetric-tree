#include <algorithm>
#include <iostream>
#include <queue>
#include <stack>
#include <string>
#include <sstream>
#include <vector>

using namespace std;

 struct TreeNode {
     int val;
     TreeNode *left;
     TreeNode *right;
     TreeNode(int x) : val(x), left(NULL), right(NULL) {}
 };


string treeNodeToString(TreeNode* root) {
    if (root == nullptr) {
      return "[]";
    }

    string output = "";
    queue<TreeNode*> q;
    q.push(root);
    while(!q.empty()) {
        TreeNode* node = q.front();
        q.pop();

        if (node == nullptr) {
          output += "null, ";
          continue;
        }

        output += to_string(node->val) + ", ";
        q.push(node->left);
        q.push(node->right);
    }
    return "[" + output.substr(0, output.length() - 2) + "]";
}

void trimLeftTrailingSpaces(string &input) {
    input.erase(input.begin(), find_if(input.begin(), input.end(), [](int ch) {
        return !isspace(ch);
    }));
}

void trimRightTrailingSpaces(string &input) {
    input.erase(find_if(input.rbegin(), input.rend(), [](int ch) {
        return !isspace(ch);
    }).base(), input.end());
}

TreeNode* stringToTreeNode(string input) {
    trimLeftTrailingSpaces(input);
    trimRightTrailingSpaces(input);
    input = input.substr(1, input.length() - 2);
    if (!input.size()) {
        return nullptr;
    }

    string item;
    stringstream ss;
    ss.str(input);

    getline(ss, item, ',');
    TreeNode* root = new TreeNode(stoi(item));
    queue<TreeNode*> nodeQueue;
    nodeQueue.push(root);

    while (true) {
        TreeNode* node = nodeQueue.front();
        nodeQueue.pop();

        if (!getline(ss, item, ',')) {
            break;
        }

        trimLeftTrailingSpaces(item);
        if (item != "null") {
            int leftNumber = stoi(item);
            node->left = new TreeNode(leftNumber);
            nodeQueue.push(node->left);
        }

        if (!getline(ss, item, ',')) {
            break;
        }

        trimLeftTrailingSpaces(item);
        if (item != "null") {
            int rightNumber = stoi(item);
            node->right = new TreeNode(rightNumber);
            nodeQueue.push(node->right);
        }
    }
    return root;
}

void prettyPrintTree(TreeNode* node, string prefix = "", bool isLeft = true) {
    if (node == nullptr) {
        cout << "Empty tree";
        return;
    }

    if(node->right) {
        prettyPrintTree(node->right, prefix + (isLeft ? "│   " : "    "), false);
    }

    cout << prefix + (isLeft ? "└── " : "┌── ") + to_string(node->val) + "\n";

    if (node->left) {
        prettyPrintTree(node->left, prefix + (isLeft ? "    " : "│   "), true);
    }
}

class BinaryTreeIterator {
public:
  enum class TraverseOrder {
    PreOrder,
    InOrder,
    PostOrder
  };

  BinaryTreeIterator(TreeNode* root, TraverseOrder order)
    : root_(root),
      order_(order)
  {
    if (root) {
      switch (order_) {
      case TraverseOrder::PreOrder:
        nodes_.push(root);
        break;
      case TraverseOrder::InOrder:
        InOrderLeftmost(root);
        break;
      case TraverseOrder::PostOrder:
        PostOrder(root);
        break;
      }
    }
  }

  bool hasNext() const {
    return !nodes_.empty();
  }

  int next() {
    TreeNode* next_root = nodes_.top();
    nodes_.pop();
    switch (order_) {
    case TraverseOrder::PreOrder:
      if (next_root->right) {
        nodes_.push(next_root->right);
      }
      if (next_root->left) {
        nodes_.push(next_root->left);
      }
      break;
    case TraverseOrder::InOrder:
      if (next_root->right) {
        InOrderLeftmost(next_root->right);
      }
      break;
    case TraverseOrder::PostOrder:

      break;
    }

    return next_root->val;
  }

private:
    void InOrderLeftmost(TreeNode* root) {
        while(root) {
            nodes_.push(root);
            root = root->left;
        }
    }

    void PostOrder(TreeNode* root) {
      if (root) {
        nodes_.push(root);
        PostOrder(root->right);
        PostOrder(root->left);
      }
    }

  TreeNode* root_ = nullptr;
  TraverseOrder order_;
  std::stack<TreeNode*> nodes_;
};


vector<int> preorderTraversal(TreeNode* root) {
    vector<int> result;
    BinaryTreeIterator iter(root, BinaryTreeIterator::TraverseOrder::PreOrder);
    while (iter.hasNext()) {
        result.push_back(iter.next());
    }

    return result;
}

vector<int> inorderTraversal(TreeNode* root) {
    vector<int> result;
    BinaryTreeIterator iter(root, BinaryTreeIterator::TraverseOrder::InOrder);
    while (iter.hasNext()) {
        result.push_back(iter.next());
    }

    return result;
}

vector<int> postorderTraversal(TreeNode* root) {
    vector<int> result;
    BinaryTreeIterator iter(root, BinaryTreeIterator::TraverseOrder::PostOrder);
    while (iter.hasNext()) {
        result.push_back(iter.next());
    }

    return result;
}

bool isSymmetric(TreeNode* root) {
  bool result = true;
  if (root) {
    const int Root = 0;
    const int Left = 1;
    const int Right = 2;
    vector<pair<int, int>> values;
    stack<pair<TreeNode*, int>> nodes;

    auto inorder = [&nodes] (TreeNode* next, int kind) {
      while (next) {
        nodes.emplace(next, kind);
        next = next->left;
        kind = Left;
      }
    };
    inorder(root, Root);

    while (!nodes.empty()) {
      auto [next, kind] = nodes.top();
      nodes.pop();
      if (next->right) {
        inorder(next->right, Right);
      }
      values.push_back({next->val, kind});
    }

    auto predicate = [] (const pair<int, int> & lhs, const pair<int, int> & rhs) {
      auto [left_val, left_kind] = lhs;
      auto [right_val, right_kind] = rhs;
      bool mirror_nodes = (left_kind == Left && right_kind == Right) || (left_kind == Right && right_kind == Left);
      return left_kind == Root || right_kind == Root || (left_val == right_val && mirror_nodes);
    };
    result = values.size() % 2 == 1 && std::equal(values.begin(), values.end(), values.rbegin(), predicate);
  }

  return result;
}

int main() {
    string line;
    while (getline(cin, line)) {
        TreeNode* root = stringToTreeNode(line);
        prettyPrintTree(root);

        cout << "Pre-order: ";
        for (int val : preorderTraversal(root)) {
            cout << val << " ";
        }
        cout << endl;

        cout << "In-order: ";
        for (int val : inorderTraversal(root)) {
            cout << val << " ";
        }
        cout << endl;

        cout << "Post-order: ";
        for (int val : postorderTraversal(root)) {
            cout << val << " ";
        }
        cout << endl;

        cout << "isSymmetric = " << isSymmetric(root) << endl;
    }

    return 0;
}

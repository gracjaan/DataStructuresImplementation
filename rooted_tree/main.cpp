#include <iostream>
#include <queue>
#include <stack>

struct Node {
  int value;
  Node *top;
  Node *left;
  Node *right;
  int depth;
  Node() : value(0), top(nullptr), left(nullptr), right(nullptr), depth(0) {}
  explicit Node(int val)
      : value(val), top(nullptr), left(nullptr), right(nullptr), depth(0) {}
};

class rooted_tree {
 private:
  Node *root;
  int height;
  Node *deepest;

 public:
  rooted_tree() {
    root = nullptr;
    height = 0;
    deepest = nullptr;
  }

  void insert_at(int value, int parent_data) {
    auto node = new Node(value);
    if (!root) {
      root = node;
      deepest = node;
      return;
    }
    auto top = bfs(parent_data);
    if (!top) return;
    node->top = top;
    if (!top->left) {
      top->left = node;
      node->depth = top->depth + 1;
      if (node->depth > deepest->depth) deepest = node;
    } else {
      auto child = top->left;
      while (child->right) {
        child = child->right;
      }
      child->right = node;
      node->depth = top->depth + 1;
    }
  }

  Node *bfs(int parent_data) {
    auto node = root;
    std::queue<Node *> qq;
    qq.push(node);
    while (qq.size() > 0) {
      auto current = qq.front();
      if (current->value == parent_data) return current;
      qq.pop();
      if (current->left) {
        qq.push(current->left);
        current = current->left;
        while (current->right) {
          qq.push(current->right);
          current = current->right;
        }
      }
    }
    return nullptr;
  }

  Node *dfs(int depth) {
    auto node = root;
    std::stack<Node *> st;
    st.push(node);
    while (st.size() > 0) {
      auto current = st.top();
      if (current->depth == depth && current != deepest) return current;
      st.pop();
      if (current->left) {
        st.push(current->left);
        current = current->left;
        while (current->right) {
          st.push(current->right);
          current = current->right;
        }
      }
    }
    return nullptr;
  }

  void update_height_in_subtree(Node *st_root) {
    auto node = st_root;
    std::stack<Node *> st;
    st.push(node);
    while (st.size() > 0) {
      auto current = st.top();
      current->depth -= 1;
      st.pop();
      if (current->left) {
        st.push(current->left);
        current = current->left;
        while (current->right) {
          st.push(current->right);
          current = current->right;
        }
      }
    }
  }

  int get_height() { return deepest->depth; }
  Node *get_deepest() { return deepest; }

  void delete_node(int value) {
    auto node = bfs(value);
    if (!node) return;
    if (!node->top) {
      if (!node->left) {
        root = nullptr;
        deepest = nullptr;
        delete node;
        return;
      } else {
        root = node->left;
        root->top = nullptr;
        auto tmp = root->right;
        while (tmp) {
          tmp->top = root;
          tmp = tmp->right;
        }
        tmp = root->left;
        while (tmp->right) {
          tmp = tmp->right;
        }

        tmp->right = root->right;
        root->right = nullptr;
        update_height_in_subtree(root);
        return;
      }
    }
    if (node->right) {
      auto tmp = node->top->left;
      if (tmp != node) {
        while (tmp->right != node) {
          tmp = tmp->right;
        }
      }
      tmp->right = node->right;
      if (node->top->left == node) node->top->left = node->right;
      if (node->left) {
        node->right->left = node->left;
        tmp = node->left;
        while (tmp) {
          tmp->top = node->right;
          tmp = tmp->right;
        }
      }
      delete node;
      return;
    }
    auto eq = dfs(node->depth);
    if (node->left) {
      if (node == deepest) {
        if (eq)
          deepest = eq;
        else
          deepest = node->left;
      }
      node->left->top = node->top;
      node->top->left = node->left;
      delete node;
      return;
    }
    if (node == deepest) {
      if (eq) {
        deepest = eq;
      } else {
        deepest = node->top;
      }
    }
    if (node->top->left == node) {
      node->top->left = nullptr;
    } else {
      auto tmp = node->top->left;
      while (tmp->right != node) {
        tmp = tmp->right;
      }
      tmp->right = nullptr;
    }
    node->top = nullptr;
    delete node;
  }
};

auto main() -> int {
  auto r_tree = rooted_tree();

  r_tree.insert_at(1, 0);
  r_tree.insert_at(2, 1);
  r_tree.insert_at(3, 1);
  r_tree.insert_at(4, 1);
  r_tree.insert_at(5, 2);
  r_tree.insert_at(6, 2);
  r_tree.insert_at(7, 3);
  r_tree.insert_at(8, 3);
  r_tree.insert_at(9, 3);
  r_tree.insert_at(10, 3);
  r_tree.insert_at(11, 4);
  r_tree.insert_at(12, 6);
  r_tree.insert_at(13, 9);
  r_tree.insert_at(14, 9);
  r_tree.insert_at(15, 14);
  r_tree.insert_at(99, 7);
  r_tree.delete_node(7);

  return 0;
}
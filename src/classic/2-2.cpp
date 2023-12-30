#include <algorithm>
#include <iostream>
#include <queue>
#include <sstream>
#include <stack>
#include <unordered_map>
#include <vector>
using namespace std;

// 节点类
class Node {
  public:
    char data;
    vector<Node*> children;
    Node(char val) : data(val) {}
    ~Node() {
        for (Node* child : children) {
            delete child;
        }
    }
};

// 表达式树类
class Tree {
  private:
    Node* root;

  public:
    Tree() : root(nullptr) {}

    Node* getRoot() { return root; }

    // 从字符串构建表达式树，优点是可以构建复数子节点的
    void buildTree(const string& expression) {
        stack<size_t> subTree_count;
        stack<Node*> nodeStack, subTree;
        subTree_count.push(0); //初始化根计数变量
        for (char ch : expression) {
            if (isalpha(ch)) {
                nodeStack.push(new Node(ch));
                subTree_count.top()++;
            } else if (ch == '(') { //碰到子树边沿，开始计数子树数量
                subTree_count.push(0);
            } else if (ch == ')') { //碰到子树边沿，收敛
                for (size_t index = 0; index < subTree_count.top(); index++) {
                    subTree.push(nodeStack.top());
                    nodeStack.pop();
                }
                for (size_t index = 0; index < subTree_count.top(); index++) {
                    nodeStack.top()->children.push_back(subTree.top());
                    subTree.pop();
                }
                subTree_count.pop();
            }
        }
        root = nodeStack.top();
    }
    // 先序遍历输出表达式树
    void preOrderTraversal() { preOrderTraversal(root); }
    void preOrderTraversal(Node* node) {
        if (node) {
            cout << node->data << " ";
            for (Node* child : node->children) {
                preOrderTraversal(child);
            }
        }
    }

    // 中序遍历输出表达式树
    void inOrderTraversal() { inOrderTraversal(root); }
    void inOrderTraversal(Node* node) {
        if (node) {
            if (node->children.size() >= 1) {
                inOrderTraversal(node->children[0]);
            }
            cout << node->data << " ";
            for (size_t i = 1; i < node->children.size(); i++) {
                inOrderTraversal(node->children[i]);
            }
        }
    }

    // 后序遍历输出表达式树
    void postOrderTraversal() { postOrderTraversal(root); }
    void postOrderTraversal(Node* node) {
        if (node) {
            for (Node* child : node->children) {
                preOrderTraversal(child);
            }
            cout << node->data << " ";
        }
    }

    // 层次遍历输出表达式树
    void levelOrderTraversal() {
        if (!root)
            return;

        queue<Node*> nodeQueue;
        nodeQueue.push(root);

        while (!nodeQueue.empty()) {
            Node* current = nodeQueue.front();
            nodeQueue.pop();

            cout << current->data << " ";

            for (Node* child : current->children) {
                if (child)
                    nodeQueue.push(child);
            }
        }
    }

    // 计算节点高度
    int getNodeHeight(Node* node) {
        if (!node)
            return 0;

        int maxHeight = 0;
        for (Node* child : node->children) {
            int childHeight = getNodeHeight(child);
            maxHeight = max(maxHeight, childHeight);
        }

        return 1 + maxHeight;
    }

    // 计算两个节点之间的路径，由于路径肯定不会成环，那必有一个拐点，这个拐点为两个节点到根节点路径的第一个交点。
    void nodeToNode(Node* start, Node* end, std::vector<Node*>& path) {
        vector<Node*> path_start, path_end;
        nodeToRoot(start, path_start);
        nodeToRoot(end, path_end);
        size_t i = 0;
        for (; i < min(path_start.size(), path.size()); i++) //其实相当于求差集
            if (path_start[path_start.size() - 1 - i] !=
                path_end[path_end.size() - 1 - i])
                break;
        if (i > 0) {
            path_start.erase(path_start.end() - i, path_start.end());
            path_end.erase(path_end.end() - i, path_end.end());
        }
        path_start.pop_back();
        reverse(path_end.begin(), path_end.end());
        path.clear();
        path.insert(path.end(), path_start.begin(), path_start.end());
        path.insert(path.end(), path_end.begin(), path_end.end());
    }

    // 计算节点到根节点的路径
    void nodeToRoot(Node* node, vector<Node*>& path) {
        // 使用广度优先搜索
        std::queue<Node*> task;
        std::unordered_map<Node*, Node*> parentMap;

        task.push(root);
        parentMap[root] = nullptr;

        // 构建邻接图
        while (!task.empty()) {
            Node* current = task.front();
            task.pop();
            for (Node* child : current->children) {
                if (child) {
                    task.push(child);
                    parentMap[child] = current;
                }
            }
        }
        // 构建路径
        Node* currentPathNode = node;
        while (currentPathNode != nullptr) {
            path.push_back(currentPathNode);
            currentPathNode = parentMap[currentPathNode];
        }
    }

    // 计算parent
    Node* nodeToParent(Node* node) {
        if (node == root)
            return nullptr;

        vector<Node*> path;
        nodeToRoot(node, path);

        return path[1];
    }
    ~Tree() { delete root; }
};

int main() {
    string expression = "A(B(D,E),C(F,G))";
    Tree tree;
    tree.buildTree(expression);

    cout << "先序遍历树：";
    tree.preOrderTraversal();
    cout << endl;

    cout << "中序遍历树：";
    tree.inOrderTraversal();
    cout << endl;

    cout << "后序遍历树：";
    tree.postOrderTraversal();
    cout << endl;

    cout << "层次遍历树：";
    tree.levelOrderTraversal();
    cout << endl;

    Node* nodeD = tree.getRoot()->children[0]->children[0];
    Node* nodeG = tree.getRoot()->children[1]->children[1];

    cout << "节点A的高度：" << tree.getNodeHeight(tree.getRoot()) << endl;

    cout << "路径从节点D到节点G：";
    vector<Node*> path;
    tree.nodeToNode(nodeD, nodeG, path);
    for (auto c : path) {
        cout << c->data << " ";
    }

    cout << endl
         << "路径从节点D的双亲节点：" << tree.nodeToParent(nodeD)->data << endl;

    return 0;
}
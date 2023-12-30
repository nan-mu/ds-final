#include <iostream>

class Node {
  public:
    int data;
    Node* next;
    Node(int value) : data(value), next(nullptr) {}
};

class LinkedList {
  private:
    Node* head;

  public:
    LinkedList() : head(nullptr) {}
    // 头插入法建立线性表
    void insertAtBeginning(int value) {
        Node* newNode = new Node(value);
        newNode->next = head;
        head = newNode;
    }

    // 尾插入法建立线性表
    void insertAtEnd(int value) {
        Node* newNode = new Node(value);
        if (!head) {
            head = newNode;
            return;
        }
        Node* current = head;
        while (current->next) {
            current = current->next;
        }
        current->next = newNode;
    }

    // 输出线性表
    void display() {
        Node* current = head;
        while (current) {
            std::cout << current->data << " ";
            current = current->next;
        }
        std::cout << std::endl;
    }

    // 在第i个位置插入结点
    void insertAtPosition(int position, int value) {
        Node* newNode = new Node(value);
        if (position == 1) {
            newNode->next = head;
            head = newNode;
            return;
        }
        Node* current = head;
        for (int i = 1; i < position - 1 && current; ++i) {
            current = current->next;
        }
        if (!current) {
            std::cerr << "Invalid position." << std::endl;
            return;
        }
        newNode->next = current->next;
        current->next = newNode;
    }

    // 求结点的前驱
    Node* findPredecessor(int value) {
        Node* current = head;
        while (current && current->next && current->next->data != value) {
            current = current->next;
        }
        return current;
    }

    // 删除第i个位置的结点
    void deleteAtPosition(int position) {
        if (position == 1) {
            Node* temp = head;
            head = head->next;
            delete temp;
            return;
        }
        Node* predecessor = head;
        for (int i = 1; i < position - 1 && predecessor; ++i) {
            predecessor = predecessor->next;
        }
        if (!predecessor || !predecessor->next) {
            std::cerr << "Invalid position." << std::endl;
            return;
        }
        Node* temp = predecessor->next;
        predecessor->next = temp->next;
        delete temp;
    }

    // 删除指定关键字数值的结点
    void deleteByValue(int value) {
        Node* predecessor = findPredecessor(value);
        if (!predecessor) {
            std::cerr << "Value not found." << std::endl;
            return;
        }
        Node* temp = predecessor->next;
        predecessor->next = temp->next;
        delete temp;
    }

    // 将数据按照从大到小的顺序排序
    void sort() {
        if (!head || !head->next) {
            // 0或1个结点，无需排序
            return;
        }
        Node* current = head;
        Node* nextNode = nullptr;
        bool swapped;
        do {
            swapped = false;
            current = head;
            while (current->next) {
                nextNode = current->next;
                if (current->data < nextNode->data) {
                    std::swap(current->data, nextNode->data);
                    swapped = true;
                }
                current = nextNode;
            }
        } while (swapped);
    }
};

int main() {
    LinkedList myList;

    // 使用尾插入法建立线性表
    myList.insertAtEnd(3);
    myList.insertAtEnd(1);
    myList.insertAtEnd(4);
    myList.insertAtEnd(1);
    myList.insertAtEnd(5);

    // 输出线性表
    std::cout << "表：";
    myList.display();

    // 在第3个位置插入结点
    myList.insertAtPosition(3, 2);
    std::cout << "在第三个位置插入节点：";
    myList.display();

    // 求结点的前驱
    Node* predecessor = myList.findPredecessor(2);
    std::cout << "求内容为2得节点的前驱：" << predecessor->data << std::endl;

    // 删除第2个位置的结点
    myList.deleteAtPosition(2);
    std::cout << "删掉第二个位置得元素：";
    myList.display();

    // 删除指定关键字数值的结点
    myList.deleteByValue(4);
    std::cout << "删除内容为4的节点：";
    myList.display();

    // 排序
    myList.sort();
    std::cout << "排序：";
    myList.display();

    return 0;
}

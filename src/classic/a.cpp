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
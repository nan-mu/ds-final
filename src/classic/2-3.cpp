#include <limits.h>
#include <stdio.h>
#include <stdlib.h>

// 结点定义
typedef struct Node {
    int vertex;
    int weight;
    struct Node* next;
} Node;

// 邻接表中的表头结点定义
typedef struct AdjList {
    Node* head;
} AdjList;

// 图的结构定义
typedef struct Graph {
    int vertices;
    AdjList* array;
} Graph;

// 创建结点
Node* createNode(int vertex, int weight) {
    Node* newNode = (Node*)malloc(sizeof(Node));
    newNode->vertex = vertex;
    newNode->weight = weight;
    newNode->next = NULL;
    return newNode;
}

// 创建图
Graph* createGraph(int vertices) {
    Graph* graph = (Graph*)malloc(sizeof(Graph));
    graph->vertices = vertices;

    // 分配邻接表的空间
    graph->array = (AdjList*)malloc(vertices * sizeof(AdjList));

    // 初始化邻接表
    for (int i = 0; i < vertices; ++i) {
        graph->array[i].head = NULL;
    }

    return graph;
}

// 添加边
void addEdge(Graph* graph, int src, int dest, int weight) {
    // 添加边到源结点
    Node* newNode = createNode(dest, weight);
    newNode->next = graph->array[src].head;
    graph->array[src].head = newNode;

    // 添加边到目标结点（无向图）
    newNode = createNode(src, weight);
    newNode->next = graph->array[dest].head;
    graph->array[dest].head = newNode;
}

// 广度优先遍历
void bfs(Graph* graph, int start) {
    bool* visited = (bool*)malloc(graph->vertices * sizeof(bool));
    for (int i = 0; i < graph->vertices; ++i) {
        visited[i] = false;
    }

    int* queue = (int*)malloc(graph->vertices * sizeof(int));
    int front = 0, rear = 0;

    visited[start] = true;
    printf("BFS starting from vertex %d: ", start);

    queue[rear++] = start;
    while (front < rear) {
        int current = queue[front++];
        printf("%d ", current);

        Node* temp = graph->array[current].head;
        while (temp != NULL) {
            int neighbor = temp->vertex;
            if (!visited[neighbor]) {
                visited[neighbor] = true;
                queue[rear++] = neighbor;
            }
            temp = temp->next;
        }
    }

    free(visited);
    free(queue);
    printf("\n");
}

// 深度优先遍历
void dfsUtil(Graph* graph, int vertex, int visited[]) {
    visited[vertex] = 1;
    printf("%d ", vertex);

    Node* temp = graph->array[vertex].head;
    while (temp != NULL) {
        int neighbor = temp->vertex;
        if (!visited[neighbor]) {
            dfsUtil(graph, neighbor, visited);
        }
        temp = temp->next;
    }
}

void dfs(Graph* graph, int start) {
    int* visited = (int*)malloc(graph->vertices * sizeof(int));
    for (int i = 0; i < graph->vertices; ++i) {
        visited[i] = 0;
    }

    dfsUtil(graph, start, visited);
    free(visited);
}

// Dijkstra算法
void dijkstra(Graph* graph, int src) {
    int* dist = (int*)malloc(graph->vertices * sizeof(int));
    int* visited = (int*)malloc(graph->vertices * sizeof(int));

    for (int i = 0; i < graph->vertices; ++i) {
        dist[i] = INT_MAX;
        visited[i] = 0;
    }

    dist[src] = 0;

    for (int count = 0; count < graph->vertices - 1; ++count) {
        int u = -1;

        for (int v = 0; v < graph->vertices; ++v) {
            if (!visited[v] && (u == -1 || dist[v] < dist[u]))
                u = v;
        }

        visited[u] = 1;

        Node* temp = graph->array[u].head;
        while (temp != NULL) {
            int v = temp->vertex;
            int weight = temp->weight;

            if (!visited[v] && dist[u] != INT_MAX &&
                dist[u] + weight < dist[v]) {
                dist[v] = dist[u] + weight;
            }

            temp = temp->next;
        }
    }

    printf("Shortest paths from vertex %d:\n", src);
    for (int i = 0; i < graph->vertices; ++i) {
        printf("To %d: %d\n", i, dist[i]);
    }

    free(dist);
    free(visited);
}

// Prim算法
void prim(Graph* graph) {
    int* parent = (int*)malloc(graph->vertices * sizeof(int));
    int* key = (int*)malloc(graph->vertices * sizeof(int));
    bool* mstSet = (bool*)malloc(graph->vertices * sizeof(bool));

    for (int i = 0; i < graph->vertices; ++i) {
        key[i] = INT_MAX;
        mstSet[i] = false;
    }

    key[0] = 0;
    parent[0] = -1;

    for (int count = 0; count < graph->vertices - 1; ++count) {
        int u = -1;

        for (int v = 0; v < graph->vertices; ++v) {
            if (!mstSet[v] && (u == -1 || key[v] < key[u]))
                u = v;
        }

        mstSet[u] = true;

        Node* temp = graph->array[u].head;
        while (temp != NULL) {
            int v = temp->vertex;
            int weight = temp->weight;

            if (!mstSet[v] && weight < key[v]) {
                parent[v] = u;
                key[v] = weight;
            }

            temp = temp->next;
        }
    }

    printf("Edges in the MST using Prim's algorithm:\n");
    for (int i = 1; i < graph->vertices; ++i) {
        printf("(%d, %d) - Weight: %d\n", parent[i], i, key[i]);
    }

    free(parent);
    free(key);
    free(mstSet);
}

// Kruskal算法
void kruskal(Graph* graph) {
    // 实现略
}

int main() {
    // 创建图
    int vertices = 6;
    Graph* graph = createGraph(vertices);

    // 添加边
    addEdge(graph, 0, 1, 4);
    addEdge(graph, 0, 2, 3);
    addEdge(graph, 1, 2, 1);
    addEdge(graph, 1, 3, 2);
    addEdge(graph, 2, 3, 5);
    addEdge(graph, 2, 4, 4);
    addEdge(graph, 3, 4, 7);
    addEdge(graph, 3, 5, 2);
    addEdge(graph, 4, 5, 6);

    // 执行图操作
    bfs(graph, 0);
    dfs(graph, 0);
    dijkstra(graph, 0);
    prim(graph);
    kruskal(graph);

    // 释放资源
    free(graph->array);
    free(graph);

    return 0;
}

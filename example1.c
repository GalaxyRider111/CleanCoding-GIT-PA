#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

typedef struct Node {
    int data;
    struct Node *next;
} NODE;

typedef struct Graph {
    int vertices;
    int *visited;
    NODE **adjacencyList;
} GRAPH;

typedef struct Stack {
    int top;
    int capacity;
    int *array;
} STACK;

NODE *createNode(int value) {
    NODE *newNode = (NODE *)malloc(sizeof(NODE));
    if (newNode == NULL) {
        fprintf(stderr, "Alocarea memoriei a eșuat\n");
        exit(EXIT_FAILURE);
    }
    newNode->data = value;
    newNode->next = NULL;
    return newNode;
}

void addEdge(GRAPH *graph, int source, int destination) {
    NODE *newNode = createNode(destination);
    newNode->next = graph->adjacencyList[source];
    graph->adjacencyList[source] = newNode;
    
    newNode = createNode(source);
    newNode->next = graph->adjacencyList[destination];
    graph->adjacencyList[destination] = newNode;
}

GRAPH *createGraph(int vertices) {
    GRAPH *graph = (GRAPH *)malloc(sizeof(GRAPH));
    if (graph == NULL) {
        fprintf(stderr, "Alocarea memoriei a eșuat\n");
        exit(EXIT_FAILURE);
    }
    
    graph->vertices = vertices;
    
    graph->adjacencyList = (NODE **)malloc(vertices * sizeof(NODE *));
    if (graph->adjacencyList == NULL) {
        fprintf(stderr, "Alocarea memoriei a eșuat\n");
        free(graph);
        exit(EXIT_FAILURE);
    }
    
    graph->visited = (int *)malloc(vertices * sizeof(int));
    if (graph->visited == NULL) {
        fprintf(stderr, "Alocarea memoriei a eșuat\n");
        free(graph->adjacencyList);
        free(graph);
        exit(EXIT_FAILURE);
    }
    
    for (int i = 0; i < vertices; i++) {
        graph->adjacencyList[i] = NULL;
        graph->visited[i] = 0;
    }
    
    return graph;
}

STACK *createStack(int capacity) {
    STACK *stack = (STACK *)malloc(sizeof(STACK));
    if (stack == NULL) {
        fprintf(stderr, "Alocarea memoriei a eșuat\n");
        exit(EXIT_FAILURE);
    }
    
    stack->array = (int *)malloc(capacity * sizeof(int));
    if (stack->array == NULL) {
        fprintf(stderr, "Alocarea memoriei a eșuat\n");
        free(stack);
        exit(EXIT_FAILURE);
    }
    
    stack->top = -1;
    stack->capacity = capacity;
    
    return stack;
}

void push(STACK *stack, int value) {
    if (stack->top == stack->capacity - 1) {
        fprintf(stderr, "Stack overflow\n");
        return;
    }
    stack->array[++stack->top] = value;
}

void DFS(GRAPH *graph, STACK *stack, int startVertex) {
    if (graph == NULL || stack == NULL) {
        return;
    }
    
    graph->visited[startVertex] = 1;
    
    printf("%d ", startVertex);
    
    push(stack, startVertex);
    
    NODE *temp = graph->adjacencyList[startVertex];
    while (temp != NULL) {
        int adjacentVertex = temp->data;
        if (graph->visited[adjacentVertex] == 0) {
            DFS(graph, stack, adjacentVertex);
        }
        temp = temp->next;
    }
}

void insertEdges(GRAPH *graph, int edgeCount, int vertexCount) {
    int source, destination;
    
    printf("Adaugă %d muchii (de la 0 la %d)\n", edgeCount, vertexCount - 1);
    for (int i = 0; i < edgeCount; i++) {
        scanf("%d %d", &source, &destination);
        
        if (source < 0 || source >= vertexCount || destination < 0 || destination >= vertexCount) {
            printf("Noduri invalide. Te rog folosește valori între 0 și %d\n", vertexCount - 1);
            i--;
            continue;
        }
        
        addEdge(graph, source, destination);
    }
}

void resetVisited(GRAPH *graph, int vertexCount) {
    for (int i = 0; i < vertexCount; i++) {
        graph->visited[i] = 0;
    }
}

bool hasPath(GRAPH *graph, int source, int destination, int vertexCount) {
    if (source == destination) {
        return true;
    }
    
    STACK *stack = createStack(vertexCount);
    
    resetVisited(graph, vertexCount);
    
    graph->visited[source] = 1;
    push(stack, source);
    
    NODE *temp = graph->adjacencyList[source];
    
    while (temp != NULL) {
        int adjacent = temp->data;
        
        if (adjacent == destination) {
            free(stack->array);
            free(stack);
            return true;
        }
        
        if (graph->visited[adjacent] == 0) {
            graph->visited[adjacent] = 1;
            push(stack, adjacent);
            
            temp = graph->adjacencyList[adjacent];
            continue;
        }
        
        temp = temp->next;
    }
    
    free(stack->array);
    free(stack);
    return false;
}

void freeGraph(GRAPH *graph) {
    if (graph == NULL) {
        return;
    }
    
    for (int i = 0; i < graph->vertices; i++) {
        NODE *current = graph->adjacencyList[i];
        while (current != NULL) {
            NODE *temp = current;
            current = current->next;
            free(temp);
        }
    }
    
    free(graph->adjacencyList);
    free(graph->visited);
    free(graph);
}

void freeStack(STACK *stack) {
    if (stack == NULL) {
        return;
    }
    free(stack->array);
    free(stack);
}

int main() {
    int vertexCount, edgeCount;
    int source, destination;
    
    printf("Câte restaurante (noduri) sunt în rețea? ");
    scanf("%d", &vertexCount);
    
    if (vertexCount <= 0) {
        printf("Număr invalid de noduri\n");
        return 1;
    }
    
    printf("Câte conexiuni (muchii) sunt în rețea? ");
    scanf("%d", &edgeCount);
    
    if (edgeCount < 0) {
        printf("Număr invalid de muchii\n");
        return 1;
    }
    
    GRAPH *graph = createGraph(vertexCount);
    
    insertEdges(graph, edgeCount, vertexCount);
    
    printf("Introduceți restaurantul sursă (0-%d): ", vertexCount - 1);
    scanf("%d", &source);
    
    printf("Introduceți restaurantul destinație (0-%d): ", vertexCount - 1);
    scanf("%d", &destination);
    
    if (source < 0 || source >= vertexCount || destination < 0 || destination >= vertexCount) {
        printf("Noduri invalide. Te rog folosește valori între 0 și %d\n", vertexCount - 1);
        freeGraph(graph);
        return 1;
    }
    
    bool pathExists = hasPath(graph, source, destination, vertexCount);
    
    if (pathExists) {
        printf("Da, există un drum de la restaurantul %d la restaurantul %d\n", source, destination);
    } else {
        printf("Nu, nu există drum de la restaurantul %d la restaurantul %d\n", source, destination);
    }
    
    freeGraph(graph);
    
    return 0;
}
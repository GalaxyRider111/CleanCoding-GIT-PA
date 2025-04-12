#include <stdlib.h>
#include <stdio.h>

typedef struct Node {
    int data;
    struct Node *next;
} NODE;

typedef struct Graph {
    int vertices;
    int *visited;
    struct Node **adjacency_lists;
} GPH;

NODE *create_node(int v) {
    NODE *new_node = (NODE*)malloc(sizeof(NODE));
    if (new_node == NULL) {
        fprintf(stderr, "Alocarea memoriei a eșuat\n");
        exit(EXIT_FAILURE);
    }
    new_node->data = v;
    new_node->next = NULL;
    return new_node;
}

GPH *create_graph(int vertices) {
    GPH *graph = (GPH*)malloc(sizeof(GPH));
    if (graph == NULL) {
        fprintf(stderr, "Alocarea memoriei a eșuat\n");
        exit(EXIT_FAILURE);
    }
    
    graph->vertices = vertices;
    graph->adjacency_lists =(Node**) malloc(vertices * sizeof(NODE *));
    if (graph->adjacency_lists == NULL) {
        fprintf(stderr, "Alocarea memoriei a eșuat\n");
        free(graph);
        exit(EXIT_FAILURE);
    }
    
    graph->visited = (int*)malloc(sizeof(int) * vertices);
    if (graph->visited == NULL) {
        fprintf(stderr, "Alocarea memoriei a eșuat\n");
        free(graph->adjacency_lists);
        free(graph);
        exit(EXIT_FAILURE);
    }
    
    for (int i = 0; i < vertices; i++) {
        graph->adjacency_lists[i] = NULL;
        graph->visited[i] = 0;
    }
    
    return graph;
}

void add_edge(GPH *graph, int src, int dest) {
    NODE *new_node = create_node(dest);
    new_node->next = graph->adjacency_lists[src];
    graph->adjacency_lists[src] = new_node;
    
    new_node = create_node(src);
    new_node->next = graph->adjacency_lists[dest];
    graph->adjacency_lists[dest] = new_node;
}

void insedg(int nr_of_vertices, int nr_of_edges, GPH *graph) {
    int src, dest;
    printf("Adaugă %d muchii (de la 0 la %d)\n", nr_of_edges, nr_of_vertices - 1);
    
    for (int i = 0; i < nr_of_edges; i++) {
        scanf("%d %d", &src, &dest);
        
        if (src < 0 || src >= nr_of_vertices || dest < 0 || dest >= nr_of_vertices) {
            printf("Noduri invalide. Te rog folosește valori între 0 și %d\n", nr_of_vertices - 1);
            i--;
            continue;
        }
        
        add_edge(graph, src, dest);
    }
}

int is_empty(NODE *queue) {
    return queue == NULL;
}

void enqueue(NODE **queue, int data) {
    NODE *new_node = create_node(data);
    
    if (is_empty(*queue)) {
        *queue = new_node;
    } else {
        NODE *temp = *queue;
        while (temp->next) {
            temp = temp->next;
        }
        temp->next = new_node;
    }
}

int dequeue(NODE **queue) {
    if (is_empty(*queue)) {
        fprintf(stderr, "Coada este goală\n");
        exit(EXIT_FAILURE);
    }
    
    int data = (*queue)->data;
    NODE *temp = *queue;
    *queue = (*queue)->next;
    free(temp);
    
    return data;
}

void print_graph(GPH *graph) {
    for (int i = 0; i < graph->vertices; i++) {
        printf("Nodul %d: ", i);
        NODE *temp = graph->adjacency_lists[i];
        
        while (temp) {
            printf("%d ", temp->data);
            temp = temp->next;
        }
        printf("\n");
    }
}

void print_queue(NODE *queue) {
    NODE *temp = queue;
    while (temp != NULL) {
        printf("%d ", temp->data);
        temp = temp->next;
    }
    printf("\n");
}

void wipe_visited_list(GPH *graph, int nr_of_vertices) {
    for (int i = 0; i < nr_of_vertices; i++) {
        graph->visited[i] = 0;
    }
}

void DFS(GPH *graph, int vertex_nr) {
    NODE *adj_list = graph->adjacency_lists[vertex_nr];
    NODE *temp = adj_list;
    
    graph->visited[vertex_nr] = 1;
    printf("%d ", vertex_nr);
    
    while (temp != NULL) {
        int connected_vertex = temp->data;
        
        if (graph->visited[connected_vertex] == 0) {
            DFS(graph, connected_vertex);
        }
        temp = temp->next;
    }
}

void BFS(GPH *graph, int start) {
    NODE *queue = NULL;
    
    graph->visited[start] = 1;
    enqueue(&queue, start);
    
    while (!is_empty(queue)) {
        int current = dequeue(&queue);
        printf("%d ", current);
        
        NODE *temp = graph->adjacency_lists[current];
        
        while (temp) {
            int adj_vertex = temp->data;
            
            if (graph->visited[adj_vertex] == 0) {
                graph->visited[adj_vertex] = 1;
                enqueue(&queue, adj_vertex);
            }
            temp = temp->next;
        }
    }
}

void free_graph(GPH *graph) {
    if (graph == NULL) {
        return;
    }
    
    for (int i = 0; i < graph->vertices; i++) {
        NODE *current = graph->adjacency_lists[i];
        while (current) {
            NODE *temp = current;
            current = current->next;
            free(temp);
        }
    }
    
    free(graph->adjacency_lists);
    free(graph->visited);
    free(graph);
}

int main() {
    int nr_of_vertices;
    int nr_of_edges;
    int starting_vertex;
    
    printf("Câte noduri are graful? ");
    scanf("%d", &nr_of_vertices);
    
    if (nr_of_vertices <= 0) {
        printf("Număr invalid de noduri\n");
        return 1;
    }
    
    printf("Câte muchii are graful? ");
    scanf("%d", &nr_of_edges);
    
    if (nr_of_edges < 0) {
        printf("Număr invalid de muchii\n");
        return 1;
    }
    
    GPH *graph = create_graph(nr_of_vertices);
    
    insedg(nr_of_vertices, nr_of_edges, graph);
    
    printf("De unde plecăm în DFS? ");
    scanf("%d", &starting_vertex);
    
    if (starting_vertex < 0 || starting_vertex >= nr_of_vertices) {
        printf("Nod invalid. Te rog folosește valori între 0 și %d\n", nr_of_vertices - 1);
        free_graph(graph);
        return 1;
    }
    
    printf("Parcurgere cu DFS: ");
    DFS(graph, starting_vertex);
    
    wipe_visited_list(graph, nr_of_vertices);
    printf("\n");
    
    printf("De unde plecăm în BFS? ");
    scanf("%d", &starting_vertex);
    
    if (starting_vertex < 0 || starting_vertex >= nr_of_vertices) {
        printf("Nod invalid. Te rog folosește valori între 0 și %d\n", nr_of_vertices - 1);
        free_graph(graph);
        return 1;
    }
    
    printf("Parcurgere cu BFS: ");
    BFS(graph, starting_vertex);
    printf("\n");
    
    free_graph(graph);
    
    return 0;
}
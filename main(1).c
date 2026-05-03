#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_NODES 100
#define MAX_EDGES 500

typedef struct {
    int u, v, w;
} Edge;

typedef struct {
    int parent;
    int rank;
} Subset;

int find(Subset subsets[], int i) {
    if (subsets[i].parent != i)
        subsets[i].parent = find(subsets, subsets[i].parent);
    return subsets[i].parent;
}

void Union(Subset subsets[], int x, int y) {
    int xroot = find(subsets, x);
    int yroot = find(subsets, y);
    if (subsets[xroot].rank < subsets[yroot].rank)
        subsets[xroot].parent = yroot;
    else if (subsets[xroot].rank > subsets[yroot].rank)
        subsets[yroot].parent = xroot;
    else {
        subsets[yroot].parent = xroot;
        subsets[xroot].rank++;
    }
}

int compareEdges(const void* a, const void* b) {
    return ((Edge*)a)->w - ((Edge*)b)->w;
}

void runKruskal(int V, int E, Edge edges[]) {
    Edge result[MAX_NODES];
    int e = 0, i = 0, totalCost = 0;
    qsort(edges, E, sizeof(Edge), compareEdges);
    Subset* subsets = (Subset*)malloc(V * sizeof(Subset));
    for (int v = 0; v < V; ++v) {
        subsets[v].parent = v;
        subsets[v].rank = 0;
    }
    while (e < V - 1 && i < E) {
        Edge next_edge = edges[i++];
        int x = find(subsets, next_edge.u);
        int y = find(subsets, next_edge.v);
        if (x != y) {
            result[e++] = next_edge;
            Union(subsets, x, y);
        }
    }
    printf("\n--- Kruskal ---\nOrdin muchii:\n");
    FILE *f = fopen("kruskal_output.csv", "w");
    fprintf(f, "Sursa,Destinatie,Cost\n");
    for (i = 0; i < e; ++i) {
        printf("%d - %d: %d\n", result[i].u, result[i].v, result[i].w);
        fprintf(f, "%d,%d,%d\n", result[i].u, result[i].v, result[i].w);
        totalCost += result[i].w;
    }
    printf("Cost total: %d\n", totalCost);
    fprintf(f, "TOTAL,,%d\n", totalCost);
    fclose(f);
    free(subsets);
}

void runPrim(int V, int adj[MAX_NODES][MAX_NODES]) {
    int parent[MAX_NODES], key[MAX_NODES], mstSet[MAX_NODES], totalCost = 0;
    for (int i = 0; i < V; i++) {
        key[i] = 1000000;
        mstSet[i] = 0;
    }
    key[0] = 0;
    parent[0] = -1;
    for (int count = 0; count < V - 1; count++) {
        int min = 1000000, u;
        for (int v = 0; v < V; v++)
            if (mstSet[v] == 0 && key[v] < min)
                min = key[v], u = v;
        mstSet[u] = 1;
        for (int v = 0; v < V; v++)
            if (adj[u][v] && mstSet[v] == 0 && adj[u][v] < key[v])
                parent[v] = u, key[v] = adj[u][v];
    }
    printf("\n--- Prim ---\nOrdin muchii:\n");
    FILE *f = fopen("prim_output.csv", "w");
    fprintf(f, "Sursa,Destinatie,Cost\n");
    for (int i = 1; i < V; i++) {
        printf("%d - %d: %d\n", parent[i], i, adj[i][parent[i]]);
        fprintf(f, "%d,%d,%d\n", parent[i], i, adj[i][parent[i]]);
        totalCost += adj[i][parent[i]];
    }
    printf("Cost total: %d\n", totalCost);
    fprintf(f, "TOTAL,,%d\n", totalCost);
    fclose(f);
}

int main() {
    FILE* file = fopen("graf_intrare.csv", "r");
    if (!file) return 1;
    char line[100];
    Edge edges[MAX_EDGES];
    int adj[MAX_NODES][MAX_NODES] = {0};
    int E = 0, V = 15;
    fgets(line, 100, file);
    while (fgets(line, 100, file)) {
        sscanf(line, "%d,%d,%d", &edges[E].u, &edges[E].v, &edges[E].w);
        adj[edges[E].u][edges[E].v] = edges[E].w;
        adj[edges[E].v][edges[E].u] = edges[E].w;
        E++;
    }
    fclose(file);
    runKruskal(V, E, edges);
    runPrim(V, adj);
    return 0;
}

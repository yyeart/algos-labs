#include <stdio.h>
#include <stdlib.h>

typedef struct {
    int src;
    int dest;
} Edge;

void topological_sort(int n, int m, Edge edges[]){
    int *in_degree = (int*)calloc(n, sizeof(int));
    int *result = (int*)malloc(n * sizeof(int));
    int *queue = (int*)malloc(n * sizeof(int));
    int head = 0, tail = 0;
    int visited_count = 0;

    for(int i = 0; i < m; ++i){
        if(edges[i].dest >= n || edges[i].src >= n){
            printf("Ошибка: индекс вершины вне диапазона\n");
            return;
        }
        in_degree[edges[i].dest]++;
    }

    for(int i = 0; i < n; ++i){
        if(in_degree[i] == 0){
            queue[tail++] = i;
        }
    }

    while (head < tail){
        int u = queue[head++];
        result[visited_count++] = u;
        
        for(int i = 0; i < m; ++i){
            if(edges[i].src == u){
                int v = edges[i].dest;
                if(--in_degree[v] == 0){
                    queue[tail++] = v;
                }
            }
        }
    }

    if(visited_count < n){
        printf("Ошибка: обнаружен цикл в графе\n");
    }
    else {
        printf("Топологический порядок: ");
        for(int i = 0; i < visited_count; ++i){
            printf("%d ", result[i]);
        }
        printf("\n");
    }

    free(in_degree);
    free(result);
    free(queue);
}

int main(void){
    int n, m;
    printf("Введите количество вершин и ребер: ");
    if(scanf("%d %d", &n, &m) != 2) return 1;
    Edge *edges = (Edge*)malloc(m * sizeof(Edge));
    printf("Введите ребра (формат: откуда куда):\n");
    for(int i = 0; i < m; ++i){
        scanf("%d %d", &edges[i].src, &edges[i].dest);
    }

    topological_sort(n, m, edges);

    free(edges);
    return 0;
}
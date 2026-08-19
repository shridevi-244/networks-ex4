/*=========================================================
        LINK STATE ROUTING USING PRIORITY QUEUE
=========================================================*/

#include <stdio.h>
#include <stdlib.h>

#define MAX_ROUTERS 20
#define INF 9999

/*=========================================================
                    STRUCTURES
=========================================================*/

typedef struct
{
    int vertex;
    int distance;
} PQNode;

typedef struct
{
    int size;
    int capacity;
    int position[MAX_ROUTERS];
    PQNode node[MAX_ROUTERS];
} PriorityQueue;


/*=========================================================
                FUNCTION PROTOTYPES
=========================================================*/

void dijkstra(int n,
              int graph[MAX_ROUTERS][MAX_ROUTERS],
              int source);

void printLSDB(int n,
               int graph[MAX_ROUTERS][MAX_ROUTERS]);

void printIntermediateTable(int n,
                            int dist[],
                            int parent[],
                            int visited[],
                            int current);

void printRoutingTable(int n,
                       int source,
                       int dist[],
                       int parent[]);

void printShortestTree(int n,
                       int source,
                       int parent[]);

void printFinalPaths(int n,
                     int source,
                     int dist[],
                     int parent[]);

void printPath(int parent[],
               int vertex);

int getNextHop(int parent[],
               int source,
               int destination);


/*=========================================================
            PRIORITY QUEUE FUNCTIONS
=========================================================*/

void swapNode(PQNode *a, PQNode *b)
{
    PQNode temp;

    temp = *a;
    *a = *b;
    *b = temp;
}


/*---------------- Heapify ----------------*/

void heapify(PriorityQueue *pq, int index)
{
    int smallest = index;
    int left = 2 * index + 1;
    int right = 2 * index + 2;

    if(left < pq->size &&
       pq->node[left].distance <
       pq->node[smallest].distance)
    {
        smallest = left;
    }

    if(right < pq->size &&
       pq->node[right].distance <
       pq->node[smallest].distance)
    {
        smallest = right;
    }

    if(smallest != index)
    {
        PQNode smallestNode = pq->node[smallest];
        PQNode currentNode = pq->node[index];

        pq->position[smallestNode.vertex] = index;
        pq->position[currentNode.vertex] = smallest;

        swapNode(&pq->node[smallest],
                 &pq->node[index]);

        heapify(pq, smallest);
    }
}


/*---------------- Empty Check ----------------*/

int isEmpty(PriorityQueue *pq)
{
    if(pq->size == 0)
        return 1;

    return 0;
}


/*---------------- Extract Minimum ----------------*/

PQNode extractMinimum(PriorityQueue *pq)
{
    PQNode root;
    PQNode last;
    PQNode temp;

    if(isEmpty(pq))
    {
        temp.vertex = -1;
        temp.distance = INF;

        return temp;
    }

    root = pq->node[0];

    last = pq->node[pq->size - 1];

    pq->node[0] = last;

    pq->position[last.vertex] = 0;
    pq->position[root.vertex] = pq->size - 1;

    pq->size--;

    heapify(pq, 0);

    return root;
}


/*---------------- Decrease Key ----------------*/

void decreaseKey(PriorityQueue *pq,
                 int vertex,
                 int distance)
{
    int i;

    i = pq->position[vertex];

    pq->node[i].distance = distance;

    while(i != 0 &&
          pq->node[i].distance <
          pq->node[(i - 1) / 2].distance)
    {
        pq->position[pq->node[i].vertex] =
            (i - 1) / 2;

        pq->position[pq->node[(i - 1) / 2].vertex] =
            i;

        swapNode(&pq->node[i],
                 &pq->node[(i - 1) / 2]);

        i = (i - 1) / 2;
    }
}


/*---------------- Check Vertex ----------------*/

int isInPriorityQueue(PriorityQueue *pq,
                      int vertex)
{
    if(pq->position[vertex] < pq->size)
        return 1;

    return 0;
}

/*=========================================================
                PRINT SHORTEST PATH
=========================================================*/

void printPath(int parent[], int vertex)
{
    if(parent[vertex] == -1)
    {
        printf("%d", vertex + 1);
        return;
    }

    printPath(parent, parent[vertex]);

    printf(" -> %d", vertex + 1);
}


/*=========================================================
                FIND NEXT HOP
=========================================================*/

int getNextHop(int parent[],
               int source,
               int destination)
{
    int current = destination;

    if(destination == source)
        return -1;

    while(parent[current] != -1 &&
          parent[current] != source)
    {
        current = parent[current];
    }

    return current + 1;
}


/*=========================================================
                LINK STATE DATABASE
=========================================================*/

void printLSDB(int n,
               int graph[MAX_ROUTERS][MAX_ROUTERS])
{
    int i, j;

    printf("\n=====================================================\n");
    printf("                LINK STATE DATABASE\n");
    printf("=====================================================\n");

    printf("%-10s%-12s%-10s\n",
           "Router",
           "Neighbor",
           "Cost");

    printf("-----------------------------------------------------\n");

    for(i = 0; i < n; i++)
    {
        for(j = 0; j < n; j++)
        {
            if(i != j && graph[i][j] != INF)
            {
                printf("%-10d%-12d%-10d\n",
                       i + 1,
                       j + 1,
                       graph[i][j]);
            }
        }
    }

    printf("=====================================================\n");
}


/*=========================================================
            INTERMEDIATE EXECUTION TABLE
=========================================================*/

void printIntermediateTable(int n,
                            int dist[],
                            int parent[],
                            int visited[],
                            int current)
{
    int i;

    printf("\n==============================================================\n");
    printf("Current Router : %d\n", current + 1);
    printf("==============================================================\n");

    printf("%-10s%-12s%-12s%-12s\n",
           "Router",
           "Distance",
           "Parent",
           "Status");

    printf("--------------------------------------------------------------\n");

    for(i = 0; i < n; i++)
    {
        printf("%-10d", i + 1);

        if(dist[i] == INF)
            printf("%-12s", "INF");
        else
            printf("%-12d", dist[i]);

        if(parent[i] == -1)
            printf("%-12s", "-");
        else
            printf("%-12d", parent[i] + 1);

        if(i == current)
            printf("%-12s", "Current");
        else if(visited[i])
            printf("%-12s", "Visited");
        else
            printf("%-12s", "Pending");

        printf("\n");
    }

    printf("==============================================================\n");
}


/*=========================================================
            DIJKSTRA USING PRIORITY QUEUE
=========================================================*/

void dijkstra(int n,
              int graph[MAX_ROUTERS][MAX_ROUTERS],
              int source)
{
    int dist[MAX_ROUTERS];
    int parent[MAX_ROUTERS];
    int visited[MAX_ROUTERS];

    PriorityQueue pq;

    int i;
    int step = 1;

    pq.size = n;
    pq.capacity = n;

    for(i = 0; i < n; i++)
    {
        dist[i] = INF;
        parent[i] = -1;
        visited[i] = 0;

        pq.node[i].vertex = i;
        pq.node[i].distance = INF;

        pq.position[i] = i;
    }

    dist[source] = 0;

    pq.node[source].distance = 0;

    decreaseKey(&pq, source, 0);

    printLSDB(n, graph);

    while(!isEmpty(&pq))
    {
        PQNode currentNode = extractMinimum(&pq);

        int u = currentNode.vertex;

        if(u == -1)
            break;

        if(visited[u])
            continue;

        visited[u] = 1;

        printf("\n");
        printf("******************************************************\n");
        printf("STEP %d\n", step++);
        printf("******************************************************\n");

        printf("Processing Router %d\n\n", u + 1);

        for(i = 0; i < n; i++)
        {
            if(graph[u][i] == INF)
                continue;

            if(!isInPriorityQueue(&pq, i))
                continue;

            if(dist[u] == INF)
                continue;

            printf("Checking Link %d -> %d\n",
                   u + 1,
                   i + 1);

            if(dist[u] + graph[u][i] < dist[i])
            {
                dist[i] = dist[u] + graph[u][i];

                parent[i] = u;

                decreaseKey(&pq,
                            i,
                            dist[i]);

                printf("Updated Distance = %d\n",
                       dist[i]);
            }
            else
            {
                printf("No Update\n");
            }
        }

        printIntermediateTable(n,
                               dist,
                               parent,
                               visited,
                               u);
    }

    printRoutingTable(n,
                      source,
                      dist,
                      parent);

    printShortestTree(n,
                      source,
                      parent);

    printFinalPaths(n,
                    source,
                    dist,
                    parent);
}

/*=========================================================
                ROUTING TABLE
=========================================================*/

void printRoutingTable(int n,
                       int source,
                       int dist[],
                       int parent[])
{
    int i;
    int nextHop;

    printf("\n");
    printf("===============================================================\n");
    printf("                     ROUTING TABLE\n");
    printf("===============================================================\n");

    printf("%-15s%-12s%-12s%-25s\n",
           "Destination",
           "Cost",
           "Next Hop",
           "Path");

    printf("---------------------------------------------------------------------\n");

    for(i = 0; i < n; i++)
    {
        printf("%-15d", i + 1);

        if(dist[i] == INF)
        {
            printf("%-12s%-12s%s\n",
                   "INF",
                   "-",
                   "No Path");
            continue;
        }

        printf("%-12d", dist[i]);

        nextHop = getNextHop(parent,
                             source,
                             i);

        if(nextHop == -1)
            printf("%-12s", "-");
        else
            printf("%-12d", nextHop);

        printPath(parent, i);

        printf("\n");
    }

    printf("===============================================================\n");
}


/*=========================================================
                SHORTEST PATH TREE
=========================================================*/

void printShortestTree(int n,
                       int source,
                       int parent[])
{
    int i;

    printf("\n");
    printf("===============================================================\n");
    printf("                SHORTEST PATH TREE\n");
    printf("===============================================================\n");

    printf("Source Router : %d\n\n", source + 1);

    printf("%d\n", source + 1);

    for(i = 0; i < n; i++)
    {
        if(parent[i] != -1)
        {
            printf("|-- %d (Parent : %d)\n",
                   i + 1,
                   parent[i] + 1);
        }
    }

    printf("===============================================================\n");
}


/*=========================================================
                FINAL SHORTEST PATHS
=========================================================*/

void printFinalPaths(int n,
                     int source,
                     int dist[],
                     int parent[])
{
    int i;
    int nextHop;

    printf("\n");
    printf("===============================================================\n");
    printf("                FINAL SHORTEST PATHS\n");
    printf("===============================================================\n");

    printf("%-15s%-12s%-12s%-30s\n",
           "Destination",
           "Cost",
           "Next Hop",
           "Shortest Path");

    printf("--------------------------------------------------------------------------\n");

    for(i = 0; i < n; i++)
    {
        printf("%-15d", i + 1);

        if(dist[i] == INF)
        {
            printf("%-12s%-12s%s\n",
                   "INF",
                   "-",
                   "UNREACHABLE");
            continue;
        }

        printf("%-12d", dist[i]);

        nextHop = getNextHop(parent,
                             source,
                             i);

        if(nextHop == -1)
            printf("%-12s", "-");
        else
            printf("%-12d", nextHop);

        printPath(parent, i);

        printf("\n");
    }

    printf("===============================================================\n");
}


/*=========================================================
                    MAIN FUNCTION
=========================================================*/

int main()
{
    int graph[MAX_ROUTERS][MAX_ROUTERS];
    int n;
    int i, j;
    int source;

    printf("=========================================\n");
    printf(" LINK STATE ROUTING USING PRIORITY QUEUE\n");
    printf("=========================================\n");

    printf("\nEnter Number of Routers : ");
    scanf("%d", &n);

    printf("\nEnter Cost Matrix\n");
    printf("(Use %d for No Link)\n\n", INF);

    for(i = 0; i < n; i++)
    {
        for(j = 0; j < n; j++)
        {
            printf("Cost from Router %d to Router %d : ",
                   i + 1,
                   j + 1);

            scanf("%d", &graph[i][j]);

            if(i == j)
                graph[i][j] = 0;
        }
    }

    printf("\nEnter Source Router : ");
    scanf("%d", &source);

    source--;

    printf("\n");
    printf("#############################################################\n");
    printf("        EXECUTING DIJKSTRA USING PRIORITY QUEUE\n");
    printf("#############################################################\n");

    dijkstra(n,
             graph,
             source);

    printf("\n");
    printf("===============================================================\n");
    printf("        LINK STATE ROUTING COMPLETED SUCCESSFULLY\n");
    printf("===============================================================\n");

    return 0;
}
#include <stdio.h>
#include <stdlib.h>

#define MAX_ROUTES 500
#define MAX_STOPS 100000
#define MAX_STOP_VALUE 1000000

typedef struct {
    int *stops;
    int size;
} Route;

int numBusesToDestination(int **routes, int routesSize,
                          int *routesColSize, int source, int target) {

    if (source == target)
        return 0;

    // stopToBuses[stop] contains the buses passing through that stop
    int **stopToBuses = malloc(MAX_STOP_VALUE * sizeof(int *));
    int *busCount = calloc(MAX_STOP_VALUE, sizeof(int));

    for (int i = 0; i < MAX_STOP_VALUE; i++) {
        stopToBuses[i] = NULL;
    }

    // Build stop -> buses mapping
    for (int bus = 0; bus < routesSize; bus++) {
        for (int j = 0; j < routesColSize[bus]; j++) {
            int stop = routes[bus][j];

            stopToBuses[stop] = realloc(
                stopToBuses[stop],
                (busCount[stop] + 1) * sizeof(int)
            );

            stopToBuses[stop][busCount[stop]++] = bus;
        }
    }

    // If source or target does not exist
    if (busCount[source] == 0 || busCount[target] == 0) {
        for (int i = 0; i < MAX_STOP_VALUE; i++)
            free(stopToBuses[i]);

        free(stopToBuses);
        free(busCount);

        return -1;
    }

    // BFS queue of stops
    int *queue = malloc(MAX_STOP_VALUE * sizeof(int));
    int front = 0, rear = 0;

    int *visitedStop = calloc(MAX_STOP_VALUE, sizeof(int));
    int visitedBus[MAX_ROUTES] = {0};

    queue[rear++] = source;
    visitedStop[source] = 1;

    int busesTaken = 0;

    while (front < rear) {

        int levelSize = rear - front;
        busesTaken++;

        for (int i = 0; i < levelSize; i++) {

            int currentStop = queue[front++];

            // Check all buses passing through current stop
            for (int j = 0; j < busCount[currentStop]; j++) {

                int bus = stopToBuses[currentStop][j];

                if (visitedBus[bus])
                    continue;

                visitedBus[bus] = 1;

                // Visit every stop of this bus
                for (int k = 0; k < routesColSize[bus]; k++) {

                    int nextStop = routes[bus][k];

                    if (nextStop == target) {
                        free(queue);
                        free(visitedStop);

                        for (int x = 0; x < MAX_STOP_VALUE; x++)
                            free(stopToBuses[x]);

                        free(stopToBuses);
                        free(busCount);

                        return busesTaken;
                    }

                    if (!visitedStop[nextStop]) {
                        visitedStop[nextStop] = 1;
                        queue[rear++] = nextStop;
                    }
                }
            }
        }
    }

    free(queue);
    free(visitedStop);

    for (int i = 0; i < MAX_STOP_VALUE; i++)
        free(stopToBuses[i]);

    free(stopToBuses);
    free(busCount);

    return -1;
}
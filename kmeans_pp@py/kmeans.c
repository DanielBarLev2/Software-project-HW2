#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include "vector.h"
#include "vector.c"

#define INF 999999999999
#define MAX_ROW_LEN 512

#ifndef bool
typedef enum { false, true } bool;
#endif

bool testValidation(int k, int n, int d, int max_iter);
void computeMinDistance(Vector *vectorList, Vector *centroids_list, int k, int n);
Vector* updateCentroids(Vector *vectorList, int n, int d, int k);
bool isConverged(Vector *centroidsList, Vector *updatedCentroidsList, int k);
Vector* Kmeans(Vector *vectorList, Vector *centroidList, int k, int n, int d, int maxIter, float eps);


void computeMinDistance(Vector *vectorList, Vector *centroidsList, int k, int n) {
    int i, j = 0;
    int closestCentroidIndex = -1;
    double minDistance = 100000000000000;

    for (i = 0; i < n; i++) {
        minDistance = INF;
        closestCentroidIndex = -1; 

        for (j = 0; j < k; j++) {
            double distance = euclidean_distance(vectorList[i], centroidsList[j]); 
            if (distance < minDistance) {
                minDistance = distance;
                closestCentroidIndex = j;
            }
        }
        vectorList[i].centroid = closestCentroidIndex;
    }
}

Vector* updateCentroids(Vector *vectorList, int n, int d, int k) {
    int i, j, l, centroidIndex;
    double *kNumbers;
    Vector currentVector;
    Vector *updatedCentroidsList;

    kNumbers = calloc(k, sizeof(double));
    updatedCentroidsList = (Vector *)malloc(k * sizeof(Vector));

    if (updatedCentroidsList == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        exit(EXIT_FAILURE);
    }

    if (kNumbers == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        exit(EXIT_FAILURE);
    }

    for (i = 0; i < k; i++) {
        updatedCentroidsList[i] = createVector(d, NULL); 
    }

   
    for (j = 0; j < n; j++) {
        currentVector = vectorList[j];
        centroidIndex = currentVector.centroid;

        updatedCentroidsList[centroidIndex] = add(currentVector, updatedCentroidsList[centroidIndex]);
        kNumbers[centroidIndex]++;
    }

    for (l = 0; l < k; l++) {
        if (kNumbers[l] != 0) {
            updatedCentroidsList[l] = multiplyScalar(updatedCentroidsList[l], 1.0f / kNumbers[l]);
        }
    }

    free(kNumbers);

    return updatedCentroidsList;
}

bool isConverged(Vector *centroidsList, Vector *updatedCentroidsList, int k) {
    int index, i;
    for (index = 0; index < k; index++) {
        for (i = 0; i < centroidsList[index].dimension; i++) {
            if (centroidsList[index].components[i] != updatedCentroidsList[index].components[i]) {
                return false;
            }
        }
    }
    return true; 
}


Vector* Kmeans(Vector *vectorList, Vector *centroidList, int k, int n, int d, int maxIter, float eps) {
    bool isValid;
    int iter, i, j;
    Vector *updatedCentroidsList;

    if (maxIter <= 0)
        maxIter = 200;
   
    for (iter = 0; iter < maxIter; iter++) {
        computeMinDistance(vectorList, centroidList, k, n); 
        updatedCentroidsList = updateCentroids(vectorList, n, d, k);

        if (isConverged(centroidList, updatedCentroidsList, k)) {
            free(updatedCentroidsList);
            break;
        }

        free(centroidList);
        centroidList = updatedCentroidsList;
    }

    return centroidList;
}

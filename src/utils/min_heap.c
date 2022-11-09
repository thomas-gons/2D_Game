#include "util.h"

Heap *heap_create() {
    Heap *heap = (Heap*) malloc(sizeof(Heap));
    heap->points = (Point*) malloc(sizeof(Point));
    heap->nb_points = 0;
    heap->size = 1;
    return heap;
}

void heapify_down(Heap *heap, uint16_t index) {
    if (index >= heap->nb_points)
        return;
    unsigned left = index * 2 + 1;
    unsigned right = index * 2 + 2;
    int leftflag = 0, rightflag = 0;

    unsigned minimum = ((heap->points) + index)->f;
    if (left < heap->nb_points && minimum > ((heap->points) + left)->f) {
        minimum = ((heap->points) + left)->f;
        leftflag = 1;
    }
    if (right < heap->nb_points && minimum > ((heap->points) + right)->f) {
        minimum = ((heap->points) + right)->f;
        leftflag = 0;
        rightflag = 1;
    }
    if (leftflag) {
        *((heap->points) + left) = *((heap->points) + index);
        ((heap->points) + index)->f = minimum;
        heapify_down(heap, left);
    }
    if (rightflag) {
        *((heap->points) + right) = *((heap->points) + index);
        ((heap->points) + index)->f = minimum;
        heapify_down(heap, right);
    }
}

void heapify_up(Heap *heap, uint16_t index) {
    int16_t parent = (index - 1) / 2;
    if (parent < 0)
        return;
    if (((heap->points) + index)->f < ((heap->points) + parent)->f) {
        Point tmp = *((heap->points) + index);
        *((heap->points) + index) = *((heap->points) + parent);
        *((heap->points) + parent) = tmp;
        heapify_up(heap, parent);
    }
}

void heap_push(Heap *heap, Point p) {
    if (heap->nb_points >= heap->size)
        return;

    *((heap->points) + heap->nb_points) = p;
    heap->nb_points++;
    if (4 * heap->nb_points >= 3 * heap->size) {
        heap->size *= 2;
        (heap->points) = (Point*) realloc((heap->points), (heap->size) * sizeof(Point));
    }
    heapify_up(heap, heap->nb_points - 1);
}

Point heap_pop(Heap *heap) {
    if (!heap->nb_points) {
        printf("Error: Empty heap cannot pop\n");
        exit(-1);
    }
    Point popped_p = *(heap->points);
    heap->nb_points--;
    Point tmp = *((heap->points) + heap->nb_points);
    *((heap->points) + heap->nb_points) = *(heap->points);
    *(heap->points) = tmp;
    heapify_down(heap, 0);
    if (4 * heap->nb_points <= heap->size) {
        heap->size /= 2;
        (heap->points) = (Point*)realloc((heap->points), (heap->size) * sizeof(Point));
    }
    return popped_p;
}
#include "MergeSort.h"

void mergeParts(int times[], int order[], int temp[], int left, int mid, int right) {
    int i = left;
    int j = mid + 1;
    int k = left;

    while (i <= mid && j <= right) {
        if (times[order[j]] < times[order[i]]) {
            temp[k] = order[j];
            j++;
        } else {
            temp[k] = order[i];
            i++;
        }
        k++;
    }
    while (i <= mid) {
        temp[k] = order[i];
        i++;
        k++;
    }
    while (j <= right) {
        temp[k] = order[j];
        j++;
        k++;
    }
    for (int p = left; p <= right; p++) {
        order[p] = temp[p];
    }
}

void mergeSort(int times[], int order[], int temp[], int left, int right) {
    if (left >= right) {
        return;
    }
    int mid = (left + right) / 2;
    mergeSort(times, order, temp, left, mid);
    mergeSort(times, order, temp, mid + 1, right);
    mergeParts(times, order, temp, left, mid, right);
}

void sortByTime(int times[], int order[], int count) {
    for (int i = 0; i < count; i++) {
        order[i] = i;
    }
    if (count < 2) {
        return;
    }
    int* temp = new int[count];
    mergeSort(times, order, temp, 0, count - 1);
    delete[] temp;
}

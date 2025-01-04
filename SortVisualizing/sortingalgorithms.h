#ifndef SORTINGALGORITHMS_H
#define SORTINGALGORITHMS_H

#include <vector>
#include <QStringList>
#include <utility>
#include <algorithm>
#include <random>
#include <chrono>

class SortingAlgorithms {
public:
    static void bubbleSort(std::vector<int>& data, bool ascending, std::vector<std::pair<int, int>>& steps);
    static void selectionSort(std::vector<int>& data, bool ascending, std::vector<std::pair<int, int>>& steps);
    static void insertionSort(std::vector<int>& data, bool ascending, std::vector<std::pair<int, int>>& steps);
    static void quickSort(std::vector<int>& data, bool ascending, std::vector<std::pair<int, int>>& steps);
    static void mergeSort(std::vector<int>& data, bool ascending, std::vector<std::pair<int, int>>& steps);
    static void heapSort(std::vector<int>& data, bool ascending, std::vector<std::pair<int, int>>& steps);
    static void shellSort(std::vector<int>& data, bool ascending, std::vector<std::pair<int, int>>& steps);
    static void countingSort(std::vector<int>& data, bool ascending, std::vector<std::pair<int, int>>& steps);
    static void radixSort(std::vector<int>& data, bool ascending, std::vector<std::pair<int, int>>& steps);
    static void bucketSort(std::vector<int>& data, bool ascending, std::vector<std::pair<int, int>>& steps);
    static QStringList availableAlgorithms();

private:
    static void quickSortHelper(std::vector<int>& data, int low, int high, bool ascending, std::vector<std::pair<int, int>>& steps);
    static int partition(std::vector<int>& data, int low, int high, bool ascending, std::vector<std::pair<int, int>>& steps);
    static void mergeSortHelper(std::vector<int>& data, int left, int right, bool ascending, std::vector<std::pair<int, int>>& steps);
    static void merge(std::vector<int>& data, int left, int mid, int right, bool ascending, std::vector<std::pair<int, int>>& steps);
    static void heapify(std::vector<int>& data, int n, int i, bool ascending, std::vector<std::pair<int, int>>& steps);
};

#endif // SORTINGALGORITHMS_H

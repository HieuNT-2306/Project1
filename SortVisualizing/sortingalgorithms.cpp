#include "sortingalgorithms.h"

void SortingAlgorithms::bubbleSort(std::vector<int>& data, bool ascending,
                                   std::vector<std::pair<int, int>>& steps,
                                   int& comparisons, int& swaps) {
    comparisons = swaps = 0;
    for (size_t i = 0; i < data.size() - 1; ++i) {
        for (size_t j = 0; j < data.size() - i - 1; ++j) {
            comparisons++;
            if ((ascending && data[j] > data[j + 1]) ||
                (!ascending && data[j] < data[j + 1])) {
                std::swap(data[j], data[j + 1]);
                steps.emplace_back(j, j + 1);
                swaps++;
            }
        }
    }
}

void SortingAlgorithms::selectionSort(std::vector<int>& data, bool ascending,
                                      std::vector<std::pair<int, int>>& steps,
                                      int& comparisons, int& swaps) {
    comparisons = swaps = 0;
    for (size_t i = 0; i < data.size() - 1; ++i) {
        size_t keyIndex = i;
        for (size_t j = i + 1; j < data.size(); ++j) {
            comparisons++;
            if ((ascending && data[j] < data[keyIndex]) ||
                (!ascending && data[j] > data[keyIndex])) {
                keyIndex = j;
            }
        }
        if (keyIndex != i) {
            std::swap(data[i], data[keyIndex]);
            steps.emplace_back(i, keyIndex);
            swaps++;
        }
    }
}

void SortingAlgorithms::insertionSort(std::vector<int>& data, bool ascending,
                                      std::vector<std::pair<int, int>>& steps,
                                      int& comparisons, int& swaps) {
    comparisons = swaps = 0;
    for (size_t i = 1; i < data.size(); ++i) {
        int key = data[i];
        int j = i - 1;
        while (j >= 0) {
            comparisons++;
            if ((ascending && data[j] > key) || (!ascending && data[j] < key)) {
                data[j + 1] = data[j];
                steps.emplace_back(j + 1, j);
                swaps++;
                --j;
            } else {
                break;
            }
        }
        if (j + 1 != i) {
            data[j + 1] = key;
            steps.emplace_back(j + 1, i);
            swaps++;
        }
    }
}

int SortingAlgorithms::partition(std::vector<int>& data, int low, int high,
                                 bool ascending, std::vector<std::pair<int, int>>& steps,
                                 int& comparisons, int& swaps) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(low, high);
    int pivotIndex = dis(gen);
    int pivot = data[pivotIndex];

    std::swap(data[pivotIndex], data[high]);
    steps.emplace_back(pivotIndex, high);
    swaps++;

    int i = low;
    for (int j = low; j < high; ++j) {
        comparisons++;
        if ((ascending && data[j] < pivot) || (!ascending && data[j] > pivot)) {
            if (i != j) {
                std::swap(data[i], data[j]);
                steps.emplace_back(i, j);
                swaps++;
            }
            ++i;
        }
    }

    std::swap(data[i], data[high]);
    steps.emplace_back(i, high);
    swaps++;
    return i;
}

void SortingAlgorithms::quickSortHelper(std::vector<int>& data, int low, int high,
                                        bool ascending, std::vector<std::pair<int, int>>& steps,
                                        int& comparisons, int& swaps) {
    if (low < high) {
        int pivotIndex = partition(data, low, high, ascending, steps, comparisons, swaps);
        quickSortHelper(data, low, pivotIndex - 1, ascending, steps, comparisons, swaps);
        quickSortHelper(data, pivotIndex + 1, high, ascending, steps, comparisons, swaps);
    }
}

void SortingAlgorithms::quickSort(std::vector<int>& data, bool ascending,
                                  std::vector<std::pair<int, int>>& steps,
                                  int& comparisons, int& swaps) {
    comparisons = swaps = 0;
    quickSortHelper(data, 0, data.size() - 1, ascending, steps, comparisons, swaps);
}

void SortingAlgorithms::mergeSortHelper(std::vector<int>& data, int left, int right,
                                        bool ascending, std::vector<std::pair<int, int>>& steps,
                                        int& comparisons, int& swaps, std::vector<int>& temp) {
    if (left < right) {
        int mid = left + (right - left) / 2;
        mergeSortHelper(data, left, mid, ascending, steps, comparisons, swaps, temp);
        mergeSortHelper(data, mid + 1, right, ascending, steps, comparisons, swaps, temp);
        merge(data, left, mid, right, ascending, steps, comparisons, swaps, temp);
    }
}

void SortingAlgorithms::merge(std::vector<int>& data, int left, int mid, int right,
                              bool ascending, std::vector<std::pair<int, int>>& steps,
                              int& comparisons, int& swaps, std::vector<int>& temp) {
    int i = left;
    int j = mid + 1;
    int k = left;

    while (i <= mid && j <= right) {
        comparisons++;
        if ((ascending && data[i] <= data[j]) || (!ascending && data[i] >= data[j])) {
            temp[k] = data[i];
            steps.emplace_back(k, i);
            i++;
        } else {
            temp[k] = data[j];
            steps.emplace_back(k, j);
            j++;
        }
        swaps++;
        k++;
    }

    while (i <= mid) {
        temp[k] = data[i];
        steps.emplace_back(k, i);
        swaps++;
        i++;
        k++;
    }

    while (j <= right) {
        temp[k] = data[j];
        steps.emplace_back(k, j);
        swaps++;
        j++;
        k++;
    }

    for (i = left; i <= right; i++) {
        data[i] = temp[i];
        steps.emplace_back(i, i);
    }
}

void SortingAlgorithms::mergeSort(std::vector<int>& data, bool ascending,
                                  std::vector<std::pair<int, int>>& steps,
                                  int& comparisons, int& swaps) {
    comparisons = swaps = 0;
    std::vector<int> temp = data;
    mergeSortHelper(data, 0, data.size() - 1, ascending, steps, comparisons, swaps, temp);

    // Add final sorted state steps
    for (size_t i = 0; i < data.size(); i++) {
        steps.emplace_back(i, i);
    }
}
void SortingAlgorithms::heapify(std::vector<int>& data, int n, int i, bool ascending,
                                std::vector<std::pair<int, int>>& steps,
                                int& comparisons, int& swaps) {
    int largest = i;
    int left = 2 * i + 1;
    int right = 2 * i + 2;

    if (left < n) {
        comparisons++;
        if ((ascending && data[left] > data[largest]) ||
            (!ascending && data[left] < data[largest])) {
            largest = left;
        }
    }

    if (right < n) {
        comparisons++;
        if ((ascending && data[right] > data[largest]) ||
            (!ascending && data[right] < data[largest])) {
            largest = right;
        }
    }

    if (largest != i) {
        std::swap(data[i], data[largest]);
        steps.emplace_back(i, largest);
        swaps++;
        heapify(data, n, largest, ascending, steps, comparisons, swaps);
    }
}

void SortingAlgorithms::heapSort(std::vector<int>& data, bool ascending,
                                 std::vector<std::pair<int, int>>& steps,
                                 int& comparisons, int& swaps) {
    comparisons = swaps = 0;
    int n = data.size();

    for (int i = n / 2 - 1; i >= 0; i--) {
        heapify(data, n, i, ascending, steps, comparisons, swaps);
    }

    for (int i = n - 1; i >= 0; i--) {
        std::swap(data[0], data[i]);
        steps.emplace_back(0, i);
        swaps++;
        heapify(data, i, 0, ascending, steps, comparisons, swaps);
    }
}

void SortingAlgorithms::shellSort(std::vector<int>& data, bool ascending,
                                  std::vector<std::pair<int, int>>& steps,
                                  int& comparisons, int& swaps) {
    comparisons = swaps = 0;
    int n = data.size();
    for (int gap = n / 2; gap > 0; gap /= 2) {
        for (int i = gap; i < n; ++i) {
            int temp = data[i];
            int j = i;
            while (j >= gap) {
                comparisons++;
                if ((ascending && data[j - gap] > temp) ||
                    (!ascending && data[j - gap] < temp)) {
                    data[j] = data[j - gap];
                    steps.emplace_back(j, j - gap);
                    swaps++;
                    j -= gap;
                } else {
                    break;
                }
            }
            if (j != i) {
                data[j] = temp;
                swaps++;
            }
        }
    }
}

void SortingAlgorithms::countingSort(std::vector<int>& data, bool ascending,
                                     std::vector<std::pair<int, int>>& steps,
                                     int& comparisons, int& swaps) {
    if (data.empty()) return;
    comparisons = swaps = 0;

    int minVal = *std::min_element(data.begin(), data.end());
    int maxVal = *std::max_element(data.begin(), data.end());
    std::vector<int> count(maxVal - minVal + 1, 0);
    std::vector<int> output(data.size());

    // Count occurrences
    for (int num : data) {
        count[num - minVal]++;
    }

    // Calculate positions
    int index = ascending ? 0 : data.size() - 1;
    for (int i = 0; i < count.size(); i++) {
        while (count[i] > 0) {
            output[index] = i + minVal;
            steps.emplace_back(index, index);
            swaps++;
            ascending ? index++ : index--;
            count[i]--;
        }
    }

    // Copy back to original array
    for (size_t i = 0; i < data.size(); i++) {
        data[i] = output[i];
        steps.emplace_back(i, i);
    }

    // Add final state steps
    for (size_t i = 0; i < data.size(); i++) {
        steps.emplace_back(i, i);
    }
}

void SortingAlgorithms::radixSort(std::vector<int>& data, bool ascending,
                                  std::vector<std::pair<int, int>>& steps,
                                  int& comparisons, int& swaps) {
    if (data.empty()) return;
    comparisons = swaps = 0;

    int maxNum = *std::max_element(data.begin(), data.end());
    std::vector<int> output(data.size());

    for (int exp = 1; maxNum / exp > 0; exp *= 10) {
        std::vector<int> count(10, 0);

        // Count occurrences
        for (size_t i = 0; i < data.size(); i++) {
            count[(data[i] / exp) % 10]++;
        }

        // Calculate positions
        if (ascending) {
            for (int i = 1; i < 10; i++) {
                count[i] += count[i - 1];
            }
        } else {
            for (int i = 8; i >= 0; i--) {
                count[i] += count[i + 1];
            }
        }

        // Build output array
        for (int i = data.size() - 1; i >= 0; i--) {
            int digit = (data[i] / exp) % 10;
            int pos = count[digit] - 1;
            output[pos] = data[i];
            count[digit]--;
            steps.emplace_back(pos, i);
            swaps++;
        }

        // Copy back to original array
        for (size_t i = 0; i < data.size(); i++) {
            data[i] = output[i];
            steps.emplace_back(i, i);
        }
    }

    // Add final state steps
    for (size_t i = 0; i < data.size(); i++) {
        steps.emplace_back(i, i);
    }
}

void SortingAlgorithms::bucketSort(std::vector<int>& data, bool ascending,
                                   std::vector<std::pair<int, int>>& steps,
                                   int& comparisons, int& swaps) {
    if (data.empty()) return;
    comparisons = swaps = 0;

    int minVal = *std::min_element(data.begin(), data.end());
    int maxVal = *std::max_element(data.begin(), data.end());
    int range = maxVal - minVal + 1;
    int bucketCount = data.size();

    std::vector<std::vector<int>> buckets(bucketCount);
    std::vector<int> finalData = data;  // Store original indices

    // Distribute into buckets
    for (size_t i = 0; i < data.size(); i++) {
        int bucketIdx = (bucketCount - 1) * (data[i] - minVal) / range;
        buckets[bucketIdx].push_back(data[i]);
    }

    // Sort individual buckets
    int currentIndex = 0;
    for (auto& bucket : buckets) {
        if (ascending) {
            std::sort(bucket.begin(), bucket.end());
        } else {
            std::sort(bucket.begin(), bucket.end(), std::greater<int>());
        }

        // Place back into array and record steps
        for (int value : bucket) {
            data[currentIndex] = value;
            steps.emplace_back(currentIndex, currentIndex);
            swaps++;
            currentIndex++;
        }
    }

    // Add final state steps
    for (size_t i = 0; i < data.size(); i++) {
        steps.emplace_back(i, i);
    }
}
QStringList SortingAlgorithms::availableAlgorithms() {
    return {
        "Bubble Sort",
        "Selection Sort",
        "Insertion Sort",
        "Quick Sort",
        "Merge Sort",
        "Heap Sort",
        "Shell Sort",
        "Counting Sort",
        "Radix Sort",
        "Bucket Sort"
    };
}

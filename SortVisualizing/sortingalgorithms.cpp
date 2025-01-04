//sortingalgorithms.cpp
#include "sortingalgorithms.h"

void SortingAlgorithms::bubbleSort(std::vector<int>& data, bool ascending, std::vector<std::pair<int, int>>& steps) {
    for (size_t i = 0; i < data.size() - 1; ++i) {
        for (size_t j = 0; j < data.size() - i - 1; ++j) {
            if ((ascending && data[j] > data[j + 1]) || (!ascending && data[j] < data[j + 1])) {
                std::swap(data[j], data[j + 1]);
                steps.emplace_back(j, j + 1);
            }
        }
    }
}

void SortingAlgorithms::selectionSort(std::vector<int>& data, bool ascending, std::vector<std::pair<int, int>>& steps) {
    for (size_t i = 0; i < data.size() - 1; ++i) {
        size_t keyIndex = i;
        for (size_t j = i + 1; j < data.size(); ++j) {
            if ((ascending && data[j] < data[keyIndex]) || (!ascending && data[j] > data[keyIndex])) {
                keyIndex = j;
            }
        }
        if (keyIndex != i) {
            std::swap(data[i], data[keyIndex]);
            steps.emplace_back(i, keyIndex);
        }
    }
}

void SortingAlgorithms::insertionSort(std::vector<int>& data, bool ascending, std::vector<std::pair<int, int>>& steps) {
    for (size_t i = 1; i < data.size(); ++i) {
        int key = data[i];
        int j = i - 1;
        while (j >= 0 && ((ascending && data[j] > key) || (!ascending && data[j] < key))) {
            data[j + 1] = data[j];
            steps.emplace_back(j + 1, j);
            --j;
        }
        data[j + 1] = key;
    }
}

int SortingAlgorithms::partition(std::vector<int>& data, int low, int high, bool ascending, std::vector<std::pair<int, int>>& steps) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(low, high);
    int pivotIndex = dis(gen);
    int pivot = data[pivotIndex];

    std::swap(data[pivotIndex], data[high]);
    steps.emplace_back(pivotIndex, high);

    int i = low;
    for (int j = low; j < high; ++j) {
        if ((ascending && data[j] < pivot) || (!ascending && data[j] > pivot)) {
            if (i != j) {
                std::swap(data[i], data[j]);
                steps.emplace_back(i, j);
            }
            ++i;
        }
    }

    std::swap(data[i], data[high]);
    steps.emplace_back(i, high);
    return i;
}

void SortingAlgorithms::quickSortHelper(std::vector<int>& data, int low, int high, bool ascending, std::vector<std::pair<int, int>>& steps) {
    if (low < high) {
        int pivotIndex = partition(data, low, high, ascending, steps);
        quickSortHelper(data, low, pivotIndex - 1, ascending, steps);
        quickSortHelper(data, pivotIndex + 1, high, ascending, steps);
    }
}

void SortingAlgorithms::quickSort(std::vector<int>& data, bool ascending, std::vector<std::pair<int, int>>& steps) {
    quickSortHelper(data, 0, data.size() - 1, ascending, steps);
}

void SortingAlgorithms::merge(std::vector<int>& data, int left, int mid, int right, bool ascending, std::vector<std::pair<int, int>>& steps) {
    std::vector<int> temp(right - left + 1);
    int i = left, j = mid + 1, k = 0;

    while (i <= mid && j <= right) {
        if ((ascending && data[i] <= data[j]) || (!ascending && data[i] >= data[j])) {
            temp[k++] = data[i++];
        } else {
            temp[k++] = data[j++];
        }
    }

    while (i <= mid) {
        temp[k++] = data[i++];
    }

    while (j <= right) {
        temp[k++] = data[j++];
    }

    for (i = 0; i < k; ++i) {
        if (data[left + i] != temp[i]) {
            data[left + i] = temp[i];
            steps.emplace_back(left + i, left + i);
        }
    }
}

void SortingAlgorithms::mergeSortHelper(std::vector<int>& data, int left, int right, bool ascending, std::vector<std::pair<int, int>>& steps) {
    if (left < right) {
        int mid = left + (right - left) / 2;
        mergeSortHelper(data, left, mid, ascending, steps);
        mergeSortHelper(data, mid + 1, right, ascending, steps);
        merge(data, left, mid, right, ascending, steps);
    }
}

void SortingAlgorithms::mergeSort(std::vector<int>& data, bool ascending, std::vector<std::pair<int, int>>& steps) {
    mergeSortHelper(data, 0, data.size() - 1, ascending, steps);
}

void SortingAlgorithms::heapify(std::vector<int>& data, int n, int i, bool ascending, std::vector<std::pair<int, int>>& steps) {
    int largest = i;
    int left = 2 * i + 1;
    int right = 2 * i + 2;

    if (left < n && ((ascending && data[left] > data[largest]) || (!ascending && data[left] < data[largest]))) {
        largest = left;
    }

    if (right < n && ((ascending && data[right] > data[largest]) || (!ascending && data[right] < data[largest]))) {
        largest = right;
    }

    if (largest != i) {
        std::swap(data[i], data[largest]);
        steps.emplace_back(i, largest);
        heapify(data, n, largest, ascending, steps);
    }
}

void SortingAlgorithms::heapSort(std::vector<int>& data, bool ascending, std::vector<std::pair<int, int>>& steps) {
    int n = data.size();

    for (int i = n / 2 - 1; i >= 0; i--) {
        heapify(data, n, i, ascending, steps);
    }

    for (int i = n - 1; i >= 0; i--) {
        std::swap(data[0], data[i]);
        steps.emplace_back(0, i);
        heapify(data, i, 0, ascending, steps);
    }
}

void SortingAlgorithms::shellSort(std::vector<int>& data, bool ascending, std::vector<std::pair<int, int>>& steps) {
    int n = data.size();
    for (int gap = n / 2; gap > 0; gap /= 2) {
        for (int i = gap; i < n; ++i) {
            int temp = data[i];
            int j = i;
            while (j >= gap && ((ascending && data[j - gap] > temp) || (!ascending && data[j - gap] < temp))) {
                data[j] = data[j - gap];
                steps.emplace_back(j, j - gap);
                j -= gap;
            }
            data[j] = temp;
        }
    }
}

void SortingAlgorithms::countingSort(std::vector<int>& data, bool ascending, std::vector<std::pair<int, int>>& steps) {
    if (data.empty()) return;

    int minVal = *std::min_element(data.begin(), data.end());
    int maxVal = *std::max_element(data.begin(), data.end());
    std::vector<int> count(maxVal - minVal + 1, 0);
    std::vector<int> output(data.size());

    for (int num : data) {
        ++count[num - minVal];
    }

    int index = ascending ? 0 : data.size() - 1;
    for (int i = 0; i < count.size(); ++i) {
        while (count[i] > 0) {
            output[index] = i + minVal;
            steps.emplace_back(index, index);
            ascending ? ++index : --index;
            --count[i];
        }
    }

    data = output;
}

void SortingAlgorithms::radixSort(std::vector<int>& data, bool ascending, std::vector<std::pair<int, int>>& steps) {
    if (data.empty()) return;

    int maxNum = *std::max_element(data.begin(), data.end());

    for (int exp = 1; maxNum / exp > 0; exp *= 10) {
        std::vector<int> output(data.size());
        std::vector<int> count(10, 0);

        for (int num : data) {
            ++count[(num / exp) % 10];
        }

        if (ascending) {
            for (int i = 1; i < 10; ++i) {
                count[i] += count[i - 1];
            }
        } else {
            for (int i = 8; i >= 0; --i) {
                count[i] += count[i + 1];
            }
        }

        for (int i = data.size() - 1; i >= 0; --i) {
            int digit = (data[i] / exp) % 10;
            output[--count[digit]] = data[i];
            steps.emplace_back(i, count[digit]);
        }

        data = output;
    }
}

void SortingAlgorithms::bucketSort(std::vector<int>& data, bool ascending, std::vector<std::pair<int, int>>& steps) {
    if (data.empty()) return;

    int minVal = *std::min_element(data.begin(), data.end());
    int maxVal = *std::max_element(data.begin(), data.end());
    int bucketCount = data.size();
    double range = (maxVal - minVal + 1.0) / bucketCount;

    std::vector<std::vector<int>> buckets(bucketCount);

    for (int num : data) {
        int bucketIdx = (num - minVal) / range;
        if (bucketIdx == bucketCount) --bucketIdx;
        buckets[bucketIdx].push_back(num);
    }

    int currentIndex = 0;
    for (auto& bucket : buckets) {
        std::sort(bucket.begin(), bucket.end(), [ascending](int a, int b) {
            return ascending ? a < b : a > b;
        });

        for (int num : bucket) {
            data[currentIndex] = num;
            steps.emplace_back(currentIndex, currentIndex);
            ++currentIndex;
        }
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

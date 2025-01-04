#include <iostream>
#include <vector>
#include <fstream>
#include <algorithm> 
#include <random>
#include <ctime>
#include <cstdlib>
#include <string>
#include <chrono>
#include <thread>
#include <future> 
#include <mutex>
#include <atomic>

using namespace std;

string selected_algorithm = "Bubble Sort";
string comparison_status = "";
vector<int> data;
bool ascending = true;

void clear_screen() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

void render_menu() {
    clear_screen();
    cout << "\n==== Sorting Program ====\n";
    cout << "1.  Input data from keyboard\n";
    cout << "2.  Input data from file\n";
    cout << "3.  Select Sorting Algorithm\n";
    cout << "4.  Compare 2 Sorting Algorithms\n";
    cout << "5.  Compare All Sorting Algorithms\n";
    cout << "6.  Sort data (Ascending)\n";
    cout << "7.  Sort data (Descending)\n";
    cout << "8.  Save sorted data to file\n";
    cout << "9.  Display data\n";
    cout << "10. Generate random data for testing purpose\n";
    cout << "0.  Exit\n";
    cout << "-------------------------------------\n";
    if (comparison_status.empty()) {
        cout << "You are choosing: " << selected_algorithm << "\n";
    } else {
        cout << "You are comparing: " << comparison_status << "\n";
    }
    cout << "-------------------------------------\n";
    cout << "Enter your choice: ";
}

void algorithm_menu() {
    clear_screen();
    cout << "\n==== Algorithm Selection ====\n";
    cout << "1.  Bubble Sort\n";
    cout << "2.  Selection Sort\n";
    cout << "3.  Insertion Sort\n";
    cout << "4.  Quick Sort\n";
    cout << "5.  Merge Sort\n";
    cout << "6.  Heap Sort\n";
    cout << "7.  Shell Sort\n";
    cout << "8.  Counting Sort\n";
    cout << "9.  Radix Sort\n";
    cout << "10. Bucket Sort\n";
    cout << "0.  Return to Main Menu\n";
    cout << "Enter your choice: ";
}

void display_data(const vector<int>& data) {
    cout << "Total number of data: " << data.size() << "\n";
    cout << "Data: ";

    if (data.size() > 100) {
    	cout << "\nDisplaying the first 20 since the data is too large:\n";
        for (size_t i = 0; i < 20; ++i) {
            cout << data[i] << " ";
        }
        cout << ".....\n";
    } else {
        for (const int& num : data) {
            cout << num << " ";
        }
        cout << "\n";
    }

    cout << "Tap any button to continue.\n";
    cin.ignore();
    cin.get();
}

void save_to_file(const vector<int>& data) {
    clear_screen();
    ofstream outfile;
    string filename;
    cout << "Enter filename to save sorted data: ";
    cin >> filename;
    outfile.open(filename);
    if (outfile.is_open()) {
        for (const int& num : data) {
            outfile << num << " ";
        }
        outfile.close();
        cout << "Data saved successfully!\n";
    } else {
        cout << "Error: Could not write to file.\n";
    }
    cout << "Tap any button to continue.\n";
    cin.ignore();
    cin.get();
}

vector<int> input_from_keyboard() {
    clear_screen();
    vector<int> data;
    int n, num;
    cout << "Enter the number of elements: ";
    cin >> n;
    cout << "Enter the elements:\n";
    for (int i = 0; i < n; ++i) {
        cin >> num;
        data.push_back(num);
    }
    cout << "Data input successfully.\n";
    cout << "Tap any button to continue.\n";
    cin.ignore();
    cin.get();
    return data;
}

vector<int> input_from_file() {
    clear_screen();
    vector<int> data;
    ifstream infile;
    string filename;
    cout << "Enter filename: ";
    cin >> filename;
    infile.open(filename);
    if (infile.is_open()) {
        int num;
        while (infile >> num) {
            data.push_back(num);
        }
        infile.close();
        cout << "Data loaded successfully!\n";
    } else {
        cout << "Error: Could not open file.\n";
    }
    cout << "Tap any button to continue.\n";
    cin.ignore();
    cin.get();
    return data;
}

vector<int> bubble_sort(vector<int> arr) {
    cout << "Bubble Sort (" << (ascending ? "Ascending" : "Descending") << "):\n";
    int n = arr.size();
    for (int i = 0; i < n - 1; ++i) {
        for (int j = 0; j < n - i - 1; ++j) {
            if ((ascending && arr[j] > arr[j + 1]) || (!ascending && arr[j] < arr[j + 1])) {
                swap(arr[j], arr[j + 1]);
            }
        }
    }
    return arr;
}

vector<int> selection_sort(vector<int> arr) {
    cout << "Selection Sort (" << (ascending ? "Ascending" : "Descending") << "):\n";
    int n = arr.size();
    for (int i = 0; i < n - 1; ++i) {
        int keyIndex = i;
        for (int j = i + 1; j < n; ++j) {
            if ((ascending && arr[j] < arr[keyIndex]) || (!ascending && arr[j] > arr[keyIndex])) {
                keyIndex = j;
            }
        }
        if (keyIndex != i) {
            swap(arr[i], arr[keyIndex]);
        }
    }
    return arr;
}

vector<int> insertion_sort(vector<int> arr) {
    cout << "Insertion Sort (" << (ascending ? "Ascending" : "Descending") << "):\n";
    int n = arr.size();
    for (int i = 1; i < n; ++i) {
        int key = arr[i];
        int j = i - 1;
        while (j >= 0 && ((ascending && arr[j] > key) || (!ascending && arr[j] < key))) {
            arr[j + 1] = arr[j];
            --j;
        }
        arr[j + 1] = key;
    }
    return arr;
}

int partition(vector<int>& arr, int low, int high, bool ascending) {
    int pivotIndex = low + rand() % (high - low); // Random pivot
    int pivot = arr[pivotIndex];
    swap(arr[pivotIndex], arr[high]); // Move pivot to end
    int i = low;

    for (int j = low; j < high; ++j) {
        if ((ascending && arr[j] < pivot) || (!ascending && arr[j] > pivot)) {
            swap(arr[i], arr[j]);
            ++i;
        }
    }
    swap(arr[i], arr[high]); // Restore pivot
    return i;
}

void quick_sort_helper(vector<int>& arr, int low, int high, bool ascending) {
    if (low < high) {
        int pivotIndex = partition(arr, low, high, ascending);
        quick_sort_helper(arr, low, pivotIndex - 1, ascending);
        quick_sort_helper(arr, pivotIndex + 1, high, ascending);
    }
}

vector<int> quick_sort(vector<int> arr) {
    srand(time(0)); // RNG
    quick_sort_helper(arr, 0, arr.size() - 1, ascending);
    return arr;
}

void merge(vector<int>& arr, int left, int mid, int right, bool ascending) {
    int n1 = mid - left + 1;
    int n2 = right - mid;

    vector<int> leftArr(n1);
    vector<int> rightArr(n2);

    for (int i = 0; i < n1; ++i) {
        leftArr[i] = arr[left + i];
    }
    for (int j = 0; j < n2; ++j) {
        rightArr[j] = arr[mid + 1 + j];
    }

    int i = 0;
    int j = 0;
    int k = left;

    while (i < n1 && j < n2) {
        if ((ascending && leftArr[i] <= rightArr[j]) || (!ascending && leftArr[i] >= rightArr[j])) {
            arr[k] = leftArr[i];
            i++;
        } else {
            arr[k] = rightArr[j];
            j++;
        }
        k++;
    }

    while (i < n1) {
        arr[k] = leftArr[i];
        i++;
        k++;
    }

    while (j < n2) {
        arr[k] = rightArr[j];
        j++;
        k++;
    }
}

void merge_sort_helper(vector<int>& arr, int left, int right, bool ascending) {
    if (left < right) {
        int mid = left + (right - left) / 2;
        merge_sort_helper(arr, left, mid, ascending);
        merge_sort_helper(arr, mid + 1, right, ascending);
        merge(arr, left, mid, right, ascending);
    }
}
	
vector<int> merge_sort(vector<int> arr) {
    cout << "Merge Sort (" << (ascending ? "Ascending" : "Descending") << "):\n";
    merge_sort_helper(arr, 0, arr.size() - 1, ascending);
    return arr;
}

void heapify(vector<int>& arr, int n, int i, bool ascending) {
    int largest = i; // Root
    int left = 2 * i + 1;
    int right = 2 * i + 2;

    if (left < n && ((ascending && arr[left] > arr[largest]) || (!ascending && arr[left] < arr[largest]))) {
        largest = left;
    }

    if (right < n && ((ascending && arr[right] > arr[largest]) || (!ascending && arr[right] < arr[largest]))) {
        largest = right;
    }

    if (largest != i) {
        swap(arr[i], arr[largest]);
        heapify(arr, n, largest, ascending);
    }
}

vector<int> heap_sort(vector<int> arr) {
    cout << "Heap Sort (" << (ascending ? "Ascending" : "Descending") << "):\n";
    int n = arr.size();

    for (int i = n / 2 - 1; i >= 0; i--) {
        heapify(arr, n, i, ascending);
    }

    for (int i = n - 1; i >= 0; i--) {
        swap(arr[0], arr[i]);
        heapify(arr, i, 0, ascending);
    }

    return arr;
}

vector<int> shell_sort(vector<int> arr) {
    cout << "Shell Sort (" << (ascending ? "Ascending" : "Descending") << "):\n";

    int n = arr.size();
    for (int gap = n / 2; gap > 0; gap /= 2) {
        for (int i = gap; i < n; ++i) {
            int temp = arr[i];
            int j;
            for (j = i; j >= gap && ((ascending && arr[j - gap] > temp) || (!ascending && arr[j - gap] < temp)); j -= gap) {
                arr[j] = arr[j - gap];
            }
            arr[j] = temp;
        }
    }

    return arr;
}

vector<int> counting_sort(vector<int> arr) {
    cout << "Counting Sort (" << (ascending ? "Ascending" : "Descending") << "):\n";

    if (arr.empty()) return arr;

    int minElem = *min_element(arr.begin(), arr.end());
    int maxElem = *max_element(arr.begin(), arr.end());
    vector<int> count(maxElem - minElem + 1, 0);

    for (int num : arr) {
        count[num - minElem]++;
    }

    vector<int> sortedArr;
    if (ascending) {
        for (int i = 0; i < count.size(); ++i) {
            sortedArr.insert(sortedArr.end(), count[i], i + minElem);
        }
    } else {
        for (int i = count.size() - 1; i >= 0; --i) {
            sortedArr.insert(sortedArr.end(), count[i], i + minElem);
        }
    }

    return sortedArr;
}

vector<int> radix_sort(vector<int> arr) {
    cout << "Radix Sort (" << (ascending ? "Ascending" : "Descending") << "):\n";

    if (arr.empty()) return arr;

    auto getMax = [](const vector<int>& arr) {
        return *max_element(arr.begin(), arr.end());
    };

    auto countingSortByDigit = [&](vector<int>& arr, int exp) {
        vector<int> output(arr.size());
        vector<int> count(10, 0);

        for (int num : arr) {
            int digit = (num / exp) % 10;
            count[digit]++;
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

        for (int i = arr.size() - 1; i >= 0; --i) {
            int digit = (arr[i] / exp) % 10;
            output[--count[digit]] = arr[i];
        }

        arr = output;
    };

    int maxNum = getMax(arr);
    for (int exp = 1; maxNum / exp > 0; exp *= 10) {
        countingSortByDigit(arr, exp);
    }

    return arr;
}

vector<int> bucket_sort(vector<int> arr) {
    cout << "Bucket Sort (" << (ascending ? "Ascending" : "Descending") << "):\n";

    if (arr.empty()) return arr;

    int minElem = *min_element(arr.begin(), arr.end());
    int maxElem = *max_element(arr.begin(), arr.end());
    int bucketCount = arr.size();
    double range = (maxElem - minElem + 1.0) / bucketCount;

    vector<vector<int>> buckets(bucketCount);

    for (int num : arr) {
        int bucketIdx = (num - minElem) / range;
        if (bucketIdx == bucketCount) bucketIdx--;
        buckets[bucketIdx].push_back(num);
    }

    vector<int> sortedArr;
    for (auto& bucket : buckets) {
        sort(bucket.begin(), bucket.end(), [&](int a, int b) { return ascending ? a < b : a > b; });
        sortedArr.insert(sortedArr.end(), bucket.begin(), bucket.end());
    }

    return sortedArr;
}

void select_algorithm() {
    algorithm_menu();
    int choice;
    cin >> choice;

    switch (choice) {
        case 1: selected_algorithm = "Bubble Sort"; break;
        case 2: selected_algorithm = "Selection Sort"; break;
        case 3: selected_algorithm = "Insertion Sort"; break;
        case 4: selected_algorithm = "Quick Sort"; break;
        case 5: selected_algorithm = "Merge Sort"; break;
        case 6: selected_algorithm = "Heap Sort"; break;
        case 7: selected_algorithm = "Shell Sort"; break;
        case 8: selected_algorithm = "Counting Sort"; break;
        case 9: selected_algorithm = "Radix Sort"; break;
        case 10: selected_algorithm = "Bucket Sort"; break;
        case 0: return;
        default: cout << "Invalid choice. Please try again.\n"; return;
    }

    cout << "You have selected: " << selected_algorithm << "\n";
    comparison_status.clear();
    cout << "Tap any button to continue.\n";
    cin.ignore();
    cin.get();
}


void compare_two_algorithms() {
    algorithm_menu();
    int choice1, choice2;

    cout << "Select the first algorithm: ";
    cin >> choice1;
    cout << "Select the second algorithm: ";
    cin >> choice2;

    vector<int> original_data = data; 

    string alg1 = (choice1 == 1) ? "Bubble Sort" :
                  (choice1 == 2) ? "Selection Sort" :
                  (choice1 == 3) ? "Insertion Sort" :
                  (choice1 == 4) ? "Quick Sort" :
                  (choice1 == 5) ? "Merge Sort" :
                  (choice1 == 6) ? "Heap Sort" :
                  (choice1 == 7) ? "Shell Sort" :
                  (choice1 == 8) ? "Counting Sort" :
                  (choice1 == 9) ? "Radix Sort" :
                  (choice1 == 10) ? "Bucket Sort" : "Unknown";

    string alg2 = (choice2 == 1) ? "Bubble Sort" :
                  (choice2 == 2) ? "Selection Sort" :
                  (choice2 == 3) ? "Insertion Sort" :
                  (choice2 == 4) ? "Quick Sort" :
                  (choice2 == 5) ? "Merge Sort" :
                  (choice2 == 6) ? "Heap Sort" :
                  (choice2 == 7) ? "Shell Sort" :
                  (choice2 == 8) ? "Counting Sort" :
                  (choice2 == 9) ? "Radix Sort" :
                  (choice2 == 10) ? "Bucket Sort" : "Unknown";

    cout << "Comparing " << alg1 << " and " << alg2 << ":\n";

    auto start1 = chrono::high_resolution_clock::now();
    if (alg1 == "Bubble Sort") data = bubble_sort(original_data);
    else if (alg1 == "Selection Sort") data = selection_sort(original_data);
    else if (alg1 == "Insertion Sort") data = insertion_sort(original_data);
    else if (alg1 == "Quick Sort") data = quick_sort(original_data);
    else if (alg1 == "Merge Sort") data = merge_sort(original_data);
    else if (alg1 == "Heap Sort") data = heap_sort(original_data);
    else if (alg1 == "Shell Sort") data = shell_sort(original_data);
    else if (alg1 == "Counting Sort") data = counting_sort(original_data);
    else if (alg1 == "Radix Sort") data = radix_sort(original_data);
    else if (alg1 == "Bucket Sort") data = bucket_sort(original_data);
    auto end1 = chrono::high_resolution_clock::now();
    auto duration1 = chrono::duration_cast<chrono::milliseconds>(end1 - start1);

    auto start2 = chrono::high_resolution_clock::now();
    if (alg2 == "Bubble Sort") data = bubble_sort(original_data);
    else if (alg2 == "Selection Sort") data = selection_sort(original_data);
    else if (alg2 == "Insertion Sort") data = insertion_sort(original_data);
    else if (alg2 == "Quick Sort") data = quick_sort(original_data);
    else if (alg2 == "Merge Sort") data = merge_sort(original_data);
    else if (alg2 == "Heap Sort") data = heap_sort(original_data);
    else if (alg2 == "Shell Sort") data = shell_sort(original_data);
    else if (alg2 == "Counting Sort") data = counting_sort(original_data);
    else if (alg2 == "Radix Sort") data = radix_sort(original_data);
    else if (alg2 == "Bucket Sort") data = bucket_sort(original_data);
    auto end2 = chrono::high_resolution_clock::now();
    auto duration2 = chrono::duration_cast<chrono::milliseconds>(end2 - start2);

    cout << alg1 << " took " << duration1.count() << " milliseconds.\n";
    cout << alg2 << " took " << duration2.count() << " milliseconds.\n";
    cout << "Tap any button to continue.\n";
    cin.ignore();
    cin.get();
}
void compare_all_algorithms(const vector<int>& original_data) {
    vector<string> algorithms = {
        "Bubble Sort", "Selection Sort", "Insertion Sort", "Quick Sort",
        "Merge Sort", "Heap Sort", "Shell Sort", "Counting Sort",
        "Radix Sort", "Bucket Sort"
    };

    cout << "Comparing all algorithms concurrently:\n";
    cout << "----------------------------------------------------\n"; 

    auto run_algorithm = [](const string& algorithm, vector<int> data) {
        auto start = chrono::high_resolution_clock::now();

        if (algorithm == "Bubble Sort") data = bubble_sort(data);
        else if (algorithm == "Selection Sort") data = selection_sort(data);
        else if (algorithm == "Insertion Sort") data = insertion_sort(data);
        else if (algorithm == "Quick Sort") data = quick_sort(data);
        else if (algorithm == "Merge Sort") data = merge_sort(data);
        else if (algorithm == "Heap Sort") data = heap_sort(data);
        else if (algorithm == "Shell Sort") data = shell_sort(data);
        else if (algorithm == "Counting Sort") data = counting_sort(data);
        else if (algorithm == "Radix Sort") data = radix_sort(data);
        else if (algorithm == "Bucket Sort") data = bucket_sort(data);

        auto end = chrono::high_resolution_clock::now();
        long long duration = chrono::duration_cast<chrono::milliseconds>(end - start).count();

        return make_pair(algorithm, duration); // Return algorithm name and duration
    };

    vector<future<pair<string, long long>>> futures; // Store futures for each algorithm

    for (const string& algorithm : algorithms) {
        futures.push_back(async(launch::async, run_algorithm, algorithm, original_data));
    }
	
	
    for (auto& future : futures) {
        auto result = future.get(); // Get the result of the future
        cout << result.first << " took " << result.second << " milliseconds.\n";
    }
	cout << "----------------------------------------------------\n"; 
    cout << "Tap any button to continue.\n";
    cin.ignore();
    cin.get();
}

void sort_data(bool order) {
    ascending = order;
    cout << "Sorting started:\n";
    
    auto start = chrono::high_resolution_clock::now(); 

    if (selected_algorithm == "Bubble Sort") {
        data = bubble_sort(data);
    } else if (selected_algorithm == "Selection Sort") {
        data = selection_sort(data);
    } else if (selected_algorithm == "Insertion Sort") {
        data = insertion_sort(data);
    } else if (selected_algorithm == "Quick Sort") {
        data = quick_sort(data);
    } else if (selected_algorithm == "Merge Sort") {
        data = merge_sort(data);
    } else if (selected_algorithm == "Heap Sort") {
        data = heap_sort(data);
    } else if (selected_algorithm == "Shell Sort") {
        data = shell_sort(data);
    } else if (selected_algorithm == "Counting Sort") {
        data = counting_sort(data);
    } else if (selected_algorithm == "Radix Sort") {
        data = radix_sort(data);
    } else if (selected_algorithm == "Bucket Sort") {
        data = bucket_sort(data);
    }
    
    auto end = chrono::high_resolution_clock::now(); 
    
    auto duration = chrono::duration_cast<chrono::milliseconds>(end - start);

    cout << "Sorting completed!\n";
    cout << "Times taken: " << duration.count() << " milliseconds\n";
    cout << "Tap any button to continue.\n";
    cin.ignore();
    cin.get();
}

void generateRandomNumbers() {
	clear_screen();
	int x;
	string filename;
    vector<int> numbers;
    cout << "Enter the number of random unique numbers to generate: ";
    cin >> x;
    cout << "Enter the file name: ";
    cin >> filename;
    for (int i = 1; i <= 2 * x; ++i) {
        numbers.push_back(i);
    }

    shuffle(numbers.begin(), numbers.end(), std::default_random_engine(std::time(0)));

    numbers.resize(x);

    ofstream outFile(filename);
    if (outFile.is_open()) {
        for (const int& number : numbers) {
            outFile << number << " ";
        }
        outFile.close();
        cout << "Random numbers generated and saved to "<< filename << std::endl;
    } else {
        cerr << "Unable to open file for writing." << std::endl;
    }
    cout << "Tap any button to continue.\n";
    cin.ignore();
    cin.get();
}

int main() {
    int choice;

    do {
        render_menu();
        cin >> choice;

        switch (choice) {
            case 1: data = input_from_keyboard(); break;
            case 2: data = input_from_file(); break;
            case 3: select_algorithm(); break;
            case 4: compare_two_algorithms() ; break;
            case 5: compare_all_algorithms(data); break;
            case 6: sort_data(true); break;
            case 7: sort_data(false); break;
            case 8: save_to_file(data); break;
            case 9: display_data(data); break;
            case 10: generateRandomNumbers(); break;
            case 0: cout << "Exiting program. Goodbye!\n"; break;
            default: cout << "Invalid choice. Please try again.\n";
        }
    } while (choice != 0);

    return 0;
}


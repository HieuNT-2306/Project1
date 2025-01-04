#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <random>
#include <ctime>

void generateRandomNumbers() {
    vector<int> numbers;
    cout << "Enter the number of random unique numbers to generate: ";
    cin >> x;
    
    for (int i = 1; i <= 2 * x; ++i) {
        numbers.push_back(i);
    }

    shuffle(numbers.begin(), numbers.end(), std::default_random_engine(std::time(0)));

    numbers.resize(x);

    ofstream outFile("array.txt");
    if (outFile.is_open()) {
        for (const int& number : numbers) {
            outFile << number << " ";
        }
        outFile.close();
        cout << "Random numbers generated and saved to array.txt" << std::endl;
    } else {
        cerr << "Unable to open file for writing." << std::endl;
    }
}

int main() {
    int x;
    std::cout << "Enter the number of random unique numbers to generate: ";
    std::cin >> x;

    generateRandomNumbers(x);

    return 0;
}

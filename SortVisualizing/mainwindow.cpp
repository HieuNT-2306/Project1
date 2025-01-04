#include "mainwindow.h"
#include "sortingalgorithms.h"
#include <QRandomGenerator>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), currentStep(0) {
    // Central Widget and Main Layout
    centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);
    mainLayout = new QVBoxLayout(centralWidget);

    // Algorithm Selection
    algorithmComboBox = new QComboBox(this);
    algorithmComboBox->addItems(SortingAlgorithms::availableAlgorithms());
    mainLayout->addWidget(new QLabel("Select Sorting Algorithm:"));
    mainLayout->addWidget(algorithmComboBox);

    // Create horizontal layout for spinboxes
    QHBoxLayout *spinBoxLayout = new QHBoxLayout();

    // Size Selection Group
    QVBoxLayout *sizeLayout = new QVBoxLayout();
    sizeSpinBox = new QSpinBox(this);
    sizeSpinBox->setRange(5, 100);
    sizeSpinBox->setValue(20);
    sizeSpinBox->setWrapping(false);
    connect(sizeSpinBox, QOverload<int>::of(&QSpinBox::valueChanged),
            [this](int value) {
                if (value > sizeSpinBox->maximum()) {
                    sizeSpinBox->setValue(sizeSpinBox->maximum());
                }
            });
    sizeLayout->addWidget(new QLabel("Number of Elements (Max 100):"));
    sizeLayout->addWidget(sizeSpinBox);
    spinBoxLayout->addLayout(sizeLayout);

    // Delay Selection Group
    QVBoxLayout *delayLayout = new QVBoxLayout();
    delaySpinBox = new QSpinBox(this);
    delaySpinBox->setRange(10, 1000);
    delaySpinBox->setValue(100);
    delaySpinBox->setSingleStep(10);
    delaySpinBox->setWrapping(false);
    connect(delaySpinBox, QOverload<int>::of(&QSpinBox::valueChanged),
            [this](int value) {
                if (value > delaySpinBox->maximum()) {
                    delaySpinBox->setValue(delaySpinBox->maximum());
                }
                updateSortDelay();
            });
    delayLayout->addWidget(new QLabel("Sorting Delay (ms):"));
    delayLayout->addWidget(delaySpinBox);
    spinBoxLayout->addLayout(delayLayout);

    // Add spacing between the spinbox groups
    spinBoxLayout->addSpacing(20);

    // Add the spinbox layout to main layout
    mainLayout->addLayout(spinBoxLayout);

    // Ascending/Descending Selection
    ascendingRadioButton = new QRadioButton("Ascending", this);
    descendingRadioButton = new QRadioButton("Descending", this);
    ascendingRadioButton->setChecked(true);

    QHBoxLayout *radioLayout = new QHBoxLayout();
    radioLayout->addWidget(new QLabel("Sorting Order:"));
    radioLayout->addWidget(ascendingRadioButton);
    radioLayout->addWidget(descendingRadioButton);
    mainLayout->addLayout(radioLayout);

    // Sorting Time Label
    sortTimeLabel = new QLabel("Sorting Time: ", this);
    mainLayout->addWidget(sortTimeLabel);

    // Horizontal layout for buttons
    QHBoxLayout *buttonLayout = new QHBoxLayout();

    // Sort Button
    sortButton = new QPushButton("Start Sorting", this);
    buttonLayout->addWidget(sortButton);
    connect(sortButton, &QPushButton::clicked, this, &MainWindow::startSorting);

    // Randomize Button
    QPushButton *randomizeButton = new QPushButton("Randomize Data", this);
    buttonLayout->addWidget(randomizeButton);
    connect(randomizeButton, &QPushButton::clicked, this, &MainWindow::randomizeData);

    // Add button layout to main layout
    mainLayout->addLayout(buttonLayout);

    // Chart View
    chartView = new QChartView(this);
    chartView->setRenderHint(QPainter::Antialiasing);
    mainLayout->addWidget(chartView);

    // Timer setup
    sortTimer = new QTimer(this);
    connect(sortTimer, &QTimer::timeout, this, &MainWindow::performNextStep);

    // Generate initial random data
    generateRandomData();
}

MainWindow::~MainWindow() {}

void MainWindow::updateSortDelay() {
    sortTimer->setInterval(delaySpinBox->value());
}

void MainWindow::startSorting() {
    // Start timing
    elapsedTimer.start();

    // Create a copy of the data to preserve the original random array
    std::vector<int> sortData = data;

    QString algorithm = algorithmComboBox->currentText();
    bool ascending = ascendingRadioButton->isChecked();

    // Clear previous steps and reset visualization state
    sortSteps.clear();
    currentStep = 0;
    sortedIndices.clear();
    currentComparison = {-1, -1};
    previousComparison = {-1, -1};

    // Calculate sort steps on the copied data
    if (algorithm == "Bubble Sort") {
        SortingAlgorithms::bubbleSort(sortData, ascending, sortSteps);
    } else if (algorithm == "Selection Sort") {
        SortingAlgorithms::selectionSort(sortData, ascending, sortSteps);
    } else if (algorithm == "Insertion Sort") {
        SortingAlgorithms::insertionSort(sortData, ascending, sortSteps);
    } else if (algorithm == "Quick Sort") {
        SortingAlgorithms::quickSort(sortData, ascending, sortSteps);
    } else if (algorithm == "Merge Sort") {
        SortingAlgorithms::mergeSort(sortData, ascending, sortSteps);
    } else if (algorithm == "Heap Sort") {
        SortingAlgorithms::heapSort(sortData, ascending, sortSteps);
    } else if (algorithm == "Shell Sort") {
        SortingAlgorithms::shellSort(sortData, ascending, sortSteps);
    } else if (algorithm == "Counting Sort") {
        SortingAlgorithms::countingSort(sortData, ascending, sortSteps);
    } else if (algorithm == "Radix Sort") {
        SortingAlgorithms::radixSort(sortData, ascending, sortSteps);
    } else if (algorithm == "Bucket Sort") {
        SortingAlgorithms::bucketSort(sortData, ascending, sortSteps);
    }

    // Calculate and display sorting time
    qint64 elapsed = elapsedTimer.elapsed();
    sortTimeLabel->setText(QString("Sorting Time (%1): %2 ms")
                               .arg(algorithm)
                               .arg(elapsed));

    // Store the final sorted state
    sortedData = sortData;

    // Start the timer for visualization
    sortTimer->start(delaySpinBox->value());
}

void MainWindow::generateRandomData() {
    int size = sizeSpinBox->value();
    data.resize(size);
    for (int& value : data) {
        value = QRandomGenerator::global()->bounded(10, 1001);
    }
    displayChart();
}

void MainWindow::displayChart() {
    // Create a new bar series
    QBarSeries *series = new QBarSeries();

    // Create a chart
    QChart *chart = new QChart();
    chart->addSeries(series);

    // Color individual bars
    series->setBarWidth(0.95);
    for (int i = 0; i < data.size(); ++i) {
        QColor barColor = Qt::gray;
        if (sortedIndices.count(i)) {
            barColor = Qt::blue;
        }

        if ((currentComparison.first != -1 && i == currentComparison.first) ||
            (currentComparison.second != -1 && i == currentComparison.second)) {
            barColor = Qt::yellow;
        }

        QBarSet *individualBarSet = new QBarSet("");
        individualBarSet->append(data[i]);
        individualBarSet->setColor(barColor);
        chart->legend()->hide();

        series->append(individualBarSet);
    }

    chart->setTitle("Sorting Visualization");
    chart->setAnimationOptions(QChart::NoAnimation);
    chart->setMargins(QMargins(0, 0, 0, 0));

    chart->createDefaultAxes();
    chart->axes(Qt::Vertical).first()->setRange(0, 1000);
    chart->axes(Qt::Horizontal).first()->setRange(0, 1);

    chartView->setRenderHint(QPainter::Antialiasing);
    chartView->setChart(chart);
    chartView->setContentsMargins(100, 100, 100, 100);
}

void MainWindow::randomizeData() {
    // Stop any ongoing sorting
    sortTimer->stop();

    // Clear sorting-related states
    sortSteps.clear();
    currentStep = 0;
    sortedIndices.clear();
    currentComparison = {-1, -1};
    sortTimeLabel->setText("Sorting Time: ");  // Clear the sorting time

    // Generate new random data
    generateRandomData();
}

void MainWindow::calculateSortSteps(const QString& algorithm, bool ascending) {
    sortSteps.clear();
    std::vector<int> sortData = data;  // Create a copy to avoid modifying original data

    if (algorithm == "Bubble Sort") {
        SortingAlgorithms::bubbleSort(sortData, ascending, sortSteps);
    } else if (algorithm == "Selection Sort") {
        SortingAlgorithms::selectionSort(sortData, ascending, sortSteps);
    } else if (algorithm == "Insertion Sort") {
        SortingAlgorithms::insertionSort(sortData, ascending, sortSteps);
    } else if (algorithm == "Quick Sort") {
        SortingAlgorithms::quickSort(sortData, ascending, sortSteps);
    } else if (algorithm == "Merge Sort") {
        SortingAlgorithms::mergeSort(sortData, ascending, sortSteps);
    } else if (algorithm == "Heap Sort") {
        SortingAlgorithms::heapSort(sortData, ascending, sortSteps);
    } else if (algorithm == "Shell Sort") {
        SortingAlgorithms::shellSort(sortData, ascending, sortSteps);
    } else if (algorithm == "Counting Sort") {
        SortingAlgorithms::countingSort(sortData, ascending, sortSteps);
    } else if (algorithm == "Radix Sort") {
        SortingAlgorithms::radixSort(sortData, ascending, sortSteps);
    } else if (algorithm == "Bucket Sort") {
        SortingAlgorithms::bucketSort(sortData, ascending, sortSteps);
    }
}

void MainWindow::performNextStep() {
    if (currentStep < sortSteps.size()) {
        auto [i, j] = sortSteps[currentStep++];

        // Update previous comparison before current comparison
        previousComparison = currentComparison;
        currentComparison = {i, j};

        // Update the visualization data
        std::swap(data[i], data[j]);

        // Check if the current state matches the sorted state
        bool isSorted = true;
        for (size_t idx = 0; idx < data.size(); ++idx) {
            if (data[idx] != sortedData[idx]) {
                isSorted = false;
                break;
            }
        }

        // If the array is sorted, mark all indices as sorted
        if (isSorted) {
            sortedIndices.clear();
            for (size_t idx = 0; idx < data.size(); ++idx) {
                sortedIndices.insert(idx);
            }
        }

        displayChart();
    } else {
        // When sorting is complete
        data = sortedData;

        // Mark all indices as sorted
        sortedIndices.clear();
        for (size_t i = 0; i < data.size(); ++i) {
            sortedIndices.insert(i);
        }

        // Reset comparisons
        currentComparison = {-1, -1};
        previousComparison = {-1, -1};

        displayChart();
        sortTimer->stop();
    }
}

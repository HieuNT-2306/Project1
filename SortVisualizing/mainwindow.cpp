#include "mainwindow.h"
#include "sortingalgorithms.h"
#include <QRandomGenerator>
#include <QDebug>
#include <QBarCategoryAxis>
#include <QValueAxis>
#include <QTabWidget>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), currentStep(0), isCompareMode(false) {

    // Create central widget and main layout
    centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);
    mainLayout = new QVBoxLayout(centralWidget);

    // Create tab widget
    tabWidget = new QTabWidget(this);
    mainLayout->addWidget(tabWidget);

    // Create tabs for different modes
    setupVisualizationTab();
    setupComparisonTab();


    // Timer setup
    sortTimer = new QTimer(this);
    connect(sortTimer, &QTimer::timeout, this, &MainWindow::performNextStep);

    // Generate initial random data
    generateRandomData();

    // Connect tab change signal
    connect(tabWidget, &QTabWidget::currentChanged, this, &MainWindow::onTabChanged);
}

MainWindow::~MainWindow() {}

void MainWindow::setupVisualizationTab() {
    // Create visualization tab widget
    QWidget* visualizationTab = new QWidget();
    QVBoxLayout* visualizationLayout = new QVBoxLayout(visualizationTab);

    // Algorithm Selection
    algorithmComboBox = new QComboBox(this);
    algorithmComboBox->addItems(SortingAlgorithms::availableAlgorithms());
    visualizationLayout->addWidget(new QLabel("Select Sorting Algorithm:"));
    visualizationLayout->addWidget(algorithmComboBox);

    // Create horizontal layout for spinboxes
    QHBoxLayout *spinBoxLayout = new QHBoxLayout();

    // Size Selection Group
    sizeSpinBox = new QSpinBox(this);
    sizeSpinBox->setRange(5, 100);
    sizeSpinBox->setValue(20);
    sizeSpinBox->setWrapping(false);
    spinBoxLayout->addWidget(new QLabel("Number of Elements:"));
    spinBoxLayout->addWidget(sizeSpinBox);

    // Delay Selection Group
    delaySpinBox = new QSpinBox(this);
    delaySpinBox->setRange(10, 1000);
    delaySpinBox->setValue(100);
    delaySpinBox->setSingleStep(10);
    spinBoxLayout->addWidget(new QLabel("Sorting Delay (ms):"));
    spinBoxLayout->addWidget(delaySpinBox);

    visualizationLayout->addLayout(spinBoxLayout);

    // Sorting Order
    QHBoxLayout* orderLayout = new QHBoxLayout();
    ascendingRadioButton = new QRadioButton("Ascending", this);
    descendingRadioButton = new QRadioButton("Descending", this);
    ascendingRadioButton->setChecked(true);
    orderLayout->addWidget(new QLabel("Sort Order:"));
    orderLayout->addWidget(ascendingRadioButton);
    orderLayout->addWidget(descendingRadioButton);
    visualizationLayout->addLayout(orderLayout);

    // Buttons
    QHBoxLayout* buttonLayout = new QHBoxLayout();
    sortButton = new QPushButton("Start Sorting", this);
    randomizeButton = new QPushButton("Randomize Data", this);
    buttonLayout->addWidget(sortButton);
    buttonLayout->addWidget(randomizeButton);
    visualizationLayout->addLayout(buttonLayout);

    // Time Label
    sortTimeLabel = new QLabel("Sorting Time: ", this);
    visualizationLayout->addWidget(sortTimeLabel);

    // Chart View
    chartView = new QChartView(this);
    chartView->setRenderHint(QPainter::Antialiasing);
    visualizationLayout->addWidget(chartView);

    // Connect signals
    connect(sortButton, &QPushButton::clicked, this, &MainWindow::startSorting);
    connect(randomizeButton, &QPushButton::clicked, this, &MainWindow::randomizeData);
    connect(delaySpinBox, QOverload<int>::of(&QSpinBox::valueChanged),
            this, &MainWindow::updateSortDelay);

    tabWidget->addTab(visualizationTab, "Visualization");
}

void MainWindow::setupComparisonTab() {
    // Create comparison tab widget
    QWidget* comparisonTab = new QWidget();
    QVBoxLayout* comparisonLayout = new QVBoxLayout(comparisonTab);

    // Algorithm Selection
    QHBoxLayout* algorithmsLayout = new QHBoxLayout();

    // First Algorithm
    QVBoxLayout* firstAlgoLayout = new QVBoxLayout();
    algorithm1ComboBox = new QComboBox(this);
    algorithm1ComboBox->addItems(SortingAlgorithms::availableAlgorithms());
    firstAlgoLayout->addWidget(new QLabel("First Algorithm:"));
    firstAlgoLayout->addWidget(algorithm1ComboBox);
    algorithmsLayout->addLayout(firstAlgoLayout);

    // Second Algorithm
    QVBoxLayout* secondAlgoLayout = new QVBoxLayout();
    algorithm2ComboBox = new QComboBox(this);
    algorithm2ComboBox->addItems(SortingAlgorithms::availableAlgorithms());
    secondAlgoLayout->addWidget(new QLabel("Second Algorithm:"));
    secondAlgoLayout->addWidget(algorithm2ComboBox);
    algorithmsLayout->addLayout(secondAlgoLayout);

    comparisonLayout->addLayout(algorithmsLayout);

    // Controls layout
    QHBoxLayout* controlsLayout = new QHBoxLayout();

    // Size Selection
    QVBoxLayout* sizeLayout = new QVBoxLayout();
    compareSpinBox = new QSpinBox(this);
    compareSpinBox->setRange(5, 100);
    compareSpinBox->setValue(20);
    compareSpinBox->setWrapping(false);
    sizeLayout->addWidget(new QLabel("Number of Elements:"));
    sizeLayout->addWidget(compareSpinBox);
    controlsLayout->addLayout(sizeLayout);

    // Delay Selection
    QVBoxLayout* delayLayout = new QVBoxLayout();
    compareDelaySpinBox = new QSpinBox(this);
    compareDelaySpinBox->setRange(10, 1000);
    compareDelaySpinBox->setValue(100);
    compareDelaySpinBox->setSingleStep(10);
    delayLayout->addWidget(new QLabel("Sorting Delay (ms):"));
    delayLayout->addWidget(compareDelaySpinBox);
    controlsLayout->addLayout(delayLayout);
    comparisonLayout->addLayout(controlsLayout);

    // Buttons
    QHBoxLayout* buttonLayout = new QHBoxLayout();
    QPushButton* compareButton = new QPushButton("Start Comparison", this);
    QPushButton* compareRandomizeButton = new QPushButton("Randomize Data", this);
    buttonLayout->addWidget(compareButton);
    buttonLayout->addWidget(compareRandomizeButton);
    comparisonLayout->addLayout(buttonLayout);

    // Charts Layout
    QHBoxLayout* chartsLayout = new QHBoxLayout();

    // Left Chart
    leftChartView = new QChartView(this);
    leftChartView->setRenderHint(QPainter::Antialiasing);
    chartsLayout->addWidget(leftChartView);

    // Right Chart
    rightChartView = new QChartView(this);
    rightChartView->setRenderHint(QPainter::Antialiasing);
    chartsLayout->addWidget(rightChartView);

    comparisonLayout->addLayout(chartsLayout);

    // Results Label
    compareTimeLabel = new QLabel("Comparison Results: ", this);
    comparisonLayout->addWidget(compareTimeLabel);

    // Connect signals
    connect(compareButton, &QPushButton::clicked, this, &MainWindow::startComparison);
    connect(compareRandomizeButton, &QPushButton::clicked, this, &MainWindow::randomizeData);
    connect(compareSpinBox, QOverload<int>::of(&QSpinBox::valueChanged),
            [this](int value) {
                sortTimer->setInterval(value);
            });

    tabWidget->addTab(comparisonTab, "Comparison");

    // Hide the main chartView when in comparison mode
    connect(tabWidget, &QTabWidget::currentChanged, [this](int index) {
        chartView->setVisible(index == 0);  // Only show in visualization mode
    });
}


void MainWindow::onTabChanged(int index) {
    // Stop any ongoing sorting
    sortTimer->stop();

    // Disconnect all timer connections
    disconnect(sortTimer, &QTimer::timeout, this, &MainWindow::performNextStep);
    disconnect(sortTimer, &QTimer::timeout, this, &MainWindow::performNextComparisonStep);

    // Update isCompareMode flag
    isCompareMode = (index == 1);

    // Connect appropriate timer signal based on current tab
    if (index == 0) {  // Visualization tab
        connect(sortTimer, &QTimer::timeout, this, &MainWindow::performNextStep);
        sortTimeLabel->setText("Sorting Time: ");
    } else {  // Comparison tab
        connect(sortTimer, &QTimer::timeout, this, &MainWindow::performNextComparisonStep);
        compareTimeLabel->setText("Comparison Results: ");
    }

    // Reset chart and states
    sortedIndices.clear();
    currentComparison = {-1, -1};
    previousComparison = {-1, -1};
    currentStep = 0;
    sortSteps.clear();

    // Reset chart
    generateRandomData();
    if (index == 0) {
        displayChart();
    } else {
        displayComparisonCharts();
    }
}

void MainWindow::updateSortDelay() {
    sortTimer->setInterval(delaySpinBox->value());
}

void MainWindow::startSorting() {
    // Start timing
    elapsedTimer.start();

    // Create a copy of the data to preserve the original random array
    std::vector<int> sortData = data;
    SortStats stats;

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
        SortingAlgorithms::bubbleSort(sortData, ascending, sortSteps, stats.comparisons, stats.swaps);
    } else if (algorithm == "Selection Sort") {
        SortingAlgorithms::selectionSort(sortData, ascending, sortSteps, stats.comparisons, stats.swaps);
    } else if (algorithm == "Insertion Sort") {
        SortingAlgorithms::insertionSort(sortData, ascending, sortSteps, stats.comparisons, stats.swaps);
    } else if (algorithm == "Quick Sort") {
        SortingAlgorithms::quickSort(sortData, ascending, sortSteps, stats.comparisons, stats.swaps);
    } else if (algorithm == "Merge Sort") {
        SortingAlgorithms::mergeSort(sortData, ascending, sortSteps, stats.comparisons, stats.swaps);
    } else if (algorithm == "Heap Sort") {
        SortingAlgorithms::heapSort(sortData, ascending, sortSteps, stats.comparisons, stats.swaps);
    } else if (algorithm == "Shell Sort") {
        SortingAlgorithms::shellSort(sortData, ascending, sortSteps, stats.comparisons, stats.swaps);
    } else if (algorithm == "Counting Sort") {
        SortingAlgorithms::countingSort(sortData, ascending, sortSteps, stats.comparisons, stats.swaps);
    } else if (algorithm == "Radix Sort") {
        SortingAlgorithms::radixSort(sortData, ascending, sortSteps, stats.comparisons, stats.swaps);
    } else if (algorithm == "Bucket Sort") {
        SortingAlgorithms::bucketSort(sortData, ascending, sortSteps, stats.comparisons, stats.swaps);
    }

    // Calculate and display sorting time
    qint64 elapsed = elapsedTimer.elapsed();
    sortTimeLabel->setText(QString("%1: Comparisons: %3 - Swaps: %4 - Visualization Time: Running...")
                               .arg(algorithm)
                               .arg(stats.comparisons)
                               .arg(stats.swaps));

    // Store the final sorted state
    sortedData = sortData;
    elapsedTimer.start();

    // Start the timer for visualization
    sortTimer->start(delaySpinBox->value());
}

void MainWindow::startComparison() {
    // Disconnect any existing connections to prevent multiple connections
    disconnect(sortTimer, &QTimer::timeout, this, &MainWindow::performNextStep);
    disconnect(sortTimer, &QTimer::timeout, this, &MainWindow::performNextComparisonStep);

    // Reset states
    leftCurrentStep = rightCurrentStep = 0;
    leftSortedIndices.clear();
    rightSortedIndices.clear();
    leftComparison = rightComparison = {-1, -1};
    leftSteps.clear();
    rightSteps.clear();

    // Initialize data
    leftData = data;
    rightData = data;

    QString alg1 = algorithm1ComboBox->currentText();
    QString alg2 = algorithm2ComboBox->currentText();
    SortStats stats1, stats2;

    std::vector<int> leftSortData = leftData;
    std::vector<int> rightSortData = rightData;

    if (alg1 == "Bubble Sort")
        SortingAlgorithms::bubbleSort(leftSortData, true, leftSteps, stats1.comparisons, stats1.swaps);
    else if (alg1 == "Selection Sort")
        SortingAlgorithms::selectionSort(leftSortData, true, leftSteps, stats1.comparisons, stats1.swaps);
    else if (alg1 == "Insertion Sort")
        SortingAlgorithms::insertionSort(leftSortData, true, leftSteps, stats1.comparisons, stats1.swaps);
    else if (alg1 == "Quick Sort")
        SortingAlgorithms::quickSort(leftSortData, true, leftSteps, stats1.comparisons, stats1.swaps);
    else if (alg1 == "Merge Sort") {
        leftSteps.clear();
        SortingAlgorithms::mergeSort(leftSortData, true, leftSteps, stats1.comparisons, stats1.swaps);
    }
    else if (alg1 == "Heap Sort")
        SortingAlgorithms::heapSort(leftSortData, true, leftSteps, stats1.comparisons, stats1.swaps);
    else if (alg1 == "Shell Sort")
        SortingAlgorithms::shellSort(leftSortData, true, leftSteps, stats1.comparisons, stats1.swaps);
    else if (alg1 == "Counting Sort") {
        leftSteps.clear();
        SortingAlgorithms::countingSort(leftSortData, true, leftSteps, stats1.comparisons, stats1.swaps);
    }
    else if (alg1 == "Radix Sort")
        SortingAlgorithms::radixSort(leftSortData, true, leftSteps, stats1.comparisons, stats1.swaps);
    else if (alg1 == "Bucket Sort") {
        leftSteps.clear();
        SortingAlgorithms::bucketSort(leftSortData, true, leftSteps, stats1.comparisons, stats1.swaps);
    }

    if (alg2 == "Bubble Sort")
        SortingAlgorithms::bubbleSort(rightSortData, true, rightSteps, stats2.comparisons, stats2.swaps);
    else if (alg2 == "Selection Sort")
        SortingAlgorithms::selectionSort(rightSortData, true, rightSteps, stats2.comparisons, stats2.swaps);
    else if (alg2 == "Insertion Sort")
        SortingAlgorithms::insertionSort(rightSortData, true, rightSteps, stats2.comparisons, stats2.swaps);
    else if (alg2 == "Quick Sort")
        SortingAlgorithms::quickSort(rightSortData, true, rightSteps, stats2.comparisons, stats2.swaps);
    else if (alg2 == "Merge Sort") {
        rightSteps.clear();
        SortingAlgorithms::mergeSort(rightSortData, true, rightSteps, stats2.comparisons, stats2.swaps);
    }
    else if (alg2 == "Heap Sort")
        SortingAlgorithms::heapSort(rightSortData, true, rightSteps, stats2.comparisons, stats2.swaps);
    else if (alg2 == "Shell Sort")
        SortingAlgorithms::shellSort(rightSortData, true, rightSteps, stats2.comparisons, stats2.swaps);
    else if (alg2 == "Counting Sort") {
        rightSteps.clear();
        SortingAlgorithms::countingSort(rightSortData, true, rightSteps, stats2.comparisons, stats2.swaps);
    }
    else if (alg2 == "Radix Sort")
        SortingAlgorithms::radixSort(rightSortData, true, rightSteps, stats2.comparisons, stats2.swaps);
    else if (alg2 == "Bucket Sort") {
        rightSteps.clear();
        SortingAlgorithms::bucketSort(rightSortData, true, rightSteps, stats2.comparisons, stats2.swaps);
    }

    leftSortedData = leftSortData;
    rightSortedData = rightSortData;

    // Connect and start the timer for visualization
    elapsedTimer.start();
    compareTimeLabel->setText(QString("%1: Comparisons: %2 - Swaps: %3 - Visualization Time: Running...\n\n%4: Comparisons: %5 - Swaps: %6 - Visualization Time: Running...")
                                  .arg(alg1).arg(stats1.comparisons).arg(stats1.swaps)
                                  .arg(alg2).arg(stats2.comparisons).arg(stats2.swaps));
    sortTimer->start(compareSpinBox->value());
    disconnect(sortTimer, &QTimer::timeout, this, &MainWindow::performNextStep);
    connect(sortTimer, &QTimer::timeout, this, &MainWindow::performNextComparisonStep);

    // Initial display
    displayComparisonCharts();
}
void MainWindow::performNextComparisonStep() {
    bool leftDone = false, rightDone = false;

    // Left algorithm
    if (leftCurrentStep < leftSteps.size()) {
        auto [i, j] = leftSteps[leftCurrentStep++];
        leftComparison = {i, j};
        if (i < leftData.size() && j < leftData.size()) {
            std::swap(leftData[i], leftData[j]);
        }

        // Check if this algorithm is done with all steps
        if (leftCurrentStep >= leftSteps.size()) {
            leftDone = true;
            leftComparison = {-1, -1};
            // Set the final sorted state
            leftData = leftSortedData;
            for (size_t i = 0; i < leftData.size(); ++i) {
                leftSortedIndices.insert(i);
            }
            // Update time
            qint64 leftTime = elapsedTimer.elapsed();
            QString currentText = compareTimeLabel->text();
            if (currentText.contains("Running...")) {
                int runningIndex = currentText.indexOf("Running...");
                currentText.replace(runningIndex, 10, QString("%1 ms").arg(leftTime));
                compareTimeLabel->setText(currentText);
            }
        }
    } else {
        leftDone = true;
        leftComparison = {-1, -1};
    }

    // Right algorithm
    if (rightCurrentStep < rightSteps.size()) {
        auto [i, j] = rightSteps[rightCurrentStep++];
        rightComparison = {i, j};
        if (i < rightData.size() && j < rightData.size()) {
            std::swap(rightData[i], rightData[j]);
        }

        // Check if this algorithm is done with all steps
        if (rightCurrentStep >= rightSteps.size()) {
            rightDone = true;
            rightComparison = {-1, -1};
            // Set the final sorted state
            rightData = rightSortedData;
            for (size_t i = 0; i < rightData.size(); ++i) {
                rightSortedIndices.insert(i);
            }
            // Update time
            qint64 rightTime = elapsedTimer.elapsed();
            QString currentText = compareTimeLabel->text();
            if (currentText.contains("Running...")) {
                int runningIndex = currentText.lastIndexOf("Running...");
                currentText.replace(runningIndex, 10, QString("%1 ms").arg(rightTime));
                compareTimeLabel->setText(currentText);
            }
        }
    } else {
        rightDone = true;
        rightComparison = {-1, -1};
    }

    displayComparisonCharts();

    if (leftDone && rightDone) {
        sortTimer->stop();
    }
}
void MainWindow::displayComparisonCharts() {
    // Create and update left chart
    QBarSeries *leftSeries = new QBarSeries();
    QChart *leftChart = new QChart();
    leftChart->addSeries(leftSeries);

    // Left bars
    leftSeries->setBarWidth(0.95);
    for (int i = 0; i < leftData.size(); ++i) {
        QColor barColor = Qt::gray;
        if (leftSortedIndices.count(i)) {
            barColor = Qt::blue;
        }
        if (i == leftComparison.first || i == leftComparison.second) {
            barColor = Qt::yellow;
        }

        QBarSet *barSet = new QBarSet("");
        barSet->append(leftData[i]);
        barSet->setColor(barColor);
        leftSeries->append(barSet);
    }

    leftChart->setTitle(algorithm1ComboBox->currentText());
    leftChart->setAnimationOptions(QChart::NoAnimation);
    leftChart->createDefaultAxes();
    leftChart->axes(Qt::Vertical).first()->setRange(0, 1000);
    leftChart->axes(Qt::Horizontal).first()->setRange(0, 1);
    leftChart->legend()->hide();
    leftChartView->setChart(leftChart);

    // Create and update right chart (similar to left)
    QBarSeries *rightSeries = new QBarSeries();
    QChart *rightChart = new QChart();
    rightChart->addSeries(rightSeries);

    // Right bars
    rightSeries->setBarWidth(0.95);
    for (int i = 0; i < rightData.size(); ++i) {
        QColor barColor = Qt::gray;
        if (rightSortedIndices.count(i)) {
            barColor = Qt::blue;
        }
        if (i == rightComparison.first || i == rightComparison.second) {
            barColor = Qt::yellow;
        }

        QBarSet *barSet = new QBarSet("");
        barSet->append(rightData[i]);
        barSet->setColor(barColor);
        rightSeries->append(barSet);
    }

    rightChart->setTitle(algorithm2ComboBox->currentText());
    rightChart->setAnimationOptions(QChart::NoAnimation);
    rightChart->createDefaultAxes();
    rightChart->axes(Qt::Vertical).first()->setRange(0, 1000);
    rightChart->axes(Qt::Horizontal).first()->setRange(0, 1);
    rightChart->legend()->hide();
    rightChartView->setChart(rightChart);
}

void MainWindow::generateRandomData() {
    int size = sizeSpinBox->value();
    data.resize(size);
    for (int& value : data) {
        value = QRandomGenerator::global()->bounded(10, 1001);
    }
}

void MainWindow::displayChart() {
    QBarSeries *series = new QBarSeries();
    QChart *chart = new QChart();
    chart->addSeries(series);

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
    if (tabWidget->currentIndex() == 0) {
        // Visualization mode
        int size = sizeSpinBox->value();
        sortTimer->stop();

        sortSteps.clear();
        currentStep = 0;
        sortedIndices.clear();  // This will make bars grey
        currentComparison = {-1, -1};
        previousComparison = {-1, -1};

        data.resize(size);
        for (int& value : data) {
            value = QRandomGenerator::global()->bounded(10, 1001);
        }
        displayChart();
    } else {
        // Comparison mode
        int size = compareSpinBox->value();
        data.resize(size);
        for (int& value : data) {
            value = QRandomGenerator::global()->bounded(10, 1001);
        }
        leftData = rightData = data;
        leftSortedIndices.clear();
        rightSortedIndices.clear();
        displayComparisonCharts();
    }
}

void MainWindow::calculateSortSteps(const QString& algorithm, bool ascending) {
    sortSteps.clear();
    std::vector<int> sortData = data;  // Create a copy to avoid modifying original data
    int comparisons = 0;
    int swaps = 0;

    if (algorithm == "Bubble Sort") {
        SortingAlgorithms::bubbleSort(sortData, ascending, sortSteps, comparisons, swaps);
    } else if (algorithm == "Selection Sort") {
        SortingAlgorithms::selectionSort(sortData, ascending, sortSteps, comparisons, swaps);
    } else if (algorithm == "Insertion Sort") {
        SortingAlgorithms::insertionSort(sortData, ascending, sortSteps, comparisons, swaps);
    } else if (algorithm == "Quick Sort") {
        SortingAlgorithms::quickSort(sortData, ascending, sortSteps, comparisons, swaps);
    } else if (algorithm == "Merge Sort") {
        SortingAlgorithms::mergeSort(sortData, ascending, sortSteps, comparisons, swaps);
    } else if (algorithm == "Heap Sort") {
        SortingAlgorithms::heapSort(sortData, ascending, sortSteps, comparisons, swaps);
    } else if (algorithm == "Shell Sort") {
        SortingAlgorithms::shellSort(sortData, ascending, sortSteps, comparisons, swaps);
    } else if (algorithm == "Counting Sort") {
        SortingAlgorithms::countingSort(sortData, ascending, sortSteps, comparisons, swaps);
    } else if (algorithm == "Radix Sort") {
        SortingAlgorithms::radixSort(sortData, ascending, sortSteps, comparisons, swaps);
    } else if (algorithm == "Bucket Sort") {
        SortingAlgorithms::bucketSort(sortData, ascending, sortSteps, comparisons, swaps);
    }

    sortTimeLabel->setText(QString("%1:\nComparisons: %2\nSwaps: %3")
                               .arg(algorithm)
                               .arg(comparisons)
                               .arg(swaps));
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
        qint64 elapsed = elapsedTimer.elapsed();
        QString currentText = sortTimeLabel->text();
        QString updatedText = currentText.replace("Running...", QString("%1 ms").arg(elapsed));
        sortTimeLabel->setText(updatedText);
    }
}

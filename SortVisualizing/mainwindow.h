#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "qelapsedtimer.h"
#include <QMainWindow>
#include <QVBoxLayout>
#include <QComboBox>
#include <QSpinBox>
#include <QRadioButton>
#include <QPushButton>
#include <QLabel>
#include <QChartView>
#include <QBarSet>
#include <QBarSeries>
#include <QChart>
#include <QTimer>
#include <QHBoxLayout>
#include <vector>
#include <set>
#include <utility>

QT_USE_NAMESPACE

    class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void startSorting();
    void performNextStep();
    void updateSortDelay();

private:
    // UI Components
    QWidget *centralWidget;
    QVBoxLayout *mainLayout;
    QComboBox *algorithmComboBox;
    QSpinBox *sizeSpinBox;
    QRadioButton *ascendingRadioButton;
    QRadioButton *descendingRadioButton;
    QPushButton *sortButton;
    QChartView *chartView;
    QTimer *sortTimer;
    QSpinBox *delaySpinBox;  // For sorting delay
    QLabel *sortTimeLabel;   // To display sorting time
    QElapsedTimer elapsedTimer;


    // Sorting Data and State
    std::vector<int> data;
    std::vector<int> sortedData;
    std::vector<std::pair<int, int>> sortSteps;
    std::set<int> sortedIndices;
    std::pair<int, int> currentComparison;
    std::pair<int, int> previousComparison;
    int currentSelectedIndex;

    int currentStep;
    // Removed currentSelectedIndex as it's no longer needed

    // Helper Methods
    void generateRandomData();
    void displayChart();
    void calculateSortSteps(const QString& algorithm, bool ascending);
    void randomizeData();
};

#endif // MAINWINDOW_H

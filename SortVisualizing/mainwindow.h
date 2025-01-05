#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QVBoxLayout>
#include <QHBoxLayout>
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
#include <QElapsedTimer>
#include <QTabWidget>
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
    void startComparison();
    void onTabChanged(int index);

private:
    // UI Components
    QWidget *centralWidget;
    QVBoxLayout *mainLayout;
    QTabWidget *tabWidget;

    // Visualization tab widgets
    QComboBox *algorithmComboBox;
    QSpinBox *sizeSpinBox;
    QSpinBox *delaySpinBox;
    QRadioButton *ascendingRadioButton;
    QRadioButton *descendingRadioButton;
    QPushButton *sortButton;
    QPushButton *randomizeButton;
    QLabel *sortTimeLabel;

    // Comparison tab widgets
    QComboBox *algorithm1ComboBox;
    QComboBox *algorithm2ComboBox;
    QSpinBox *compareSpinBox;
    QSpinBox *compareDelaySpinBox;
    QLabel *compareTimeLabel;

    // Common components
    QChartView *chartView;
    QTimer *sortTimer;
    QElapsedTimer elapsedTimer;

    // Comparison graph component
    QChartView *leftChartView;
    QChartView *rightChartView;
    std::vector<int> leftData;
    std::vector<int> rightData;
    std::vector<int> leftSortedData;
    std::vector<int> rightSortedData;
    std::vector<std::pair<int, int>> leftSteps;
    std::vector<std::pair<int, int>> rightSteps;
    std::set<int> leftSortedIndices;
    std::set<int> rightSortedIndices;
    std::pair<int, int> leftComparison;
    std::pair<int, int> rightComparison;
    int leftCurrentStep;
    int rightCurrentStep;
    void displayComparisonCharts();
    void performNextComparisonStep();

    // Data and State
    std::vector<int> data;
    std::vector<int> sortedData;
    std::vector<std::pair<int, int>> sortSteps;
    std::set<int> sortedIndices;
    std::pair<int, int> currentComparison;
    std::pair<int, int> previousComparison;
    int currentStep;
    bool isCompareMode;

    // Helper Methods
    void setupVisualizationTab();
    void setupComparisonTab();
    void generateRandomData();
    void displayChart();
    void calculateSortSteps(const QString& algorithm, bool ascending);
    void randomizeData();
};

#endif // MAINWINDOW_H

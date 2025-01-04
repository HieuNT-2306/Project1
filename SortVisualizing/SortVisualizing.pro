TEMPLATE = app
TARGET = SortingVisualizer
CONFIG += c++17

QT += \
    core \
    gui \
    widgets \
    charts

SOURCES += \
    main.cpp \
    mainwindow.cpp \
    sortingalgorithms.cpp

HEADERS += \
    mainwindow.h \
    sortingalgorithms.h

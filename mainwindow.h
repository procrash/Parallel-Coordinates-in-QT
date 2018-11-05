#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include "parallelcoordinateswidget.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    ParallelCoordinatesWidget<WIDGET_DATA_TYPE>* parallelCoordinatesWidget;
    DataStore<WIDGET_DATA_TYPE> dataStore;
    void addDataCars();
    void addDataIris() {
        dataStore.loadDataset("/Users/wolfgangmeyerle/Documents/workspace/ParallelCoordinates/iris.csv",5,0,1, std::vector<size_t>{4} );

        this->parallelCoordinatesWidget->setDataStorePtr(&dataStore);
        this->parallelCoordinatesWidget->calcDataInBackground();
    }
};

#endif // MAINWINDOW_H

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
//#include <boost/thread.hpp>
#include <vector>
//#include "networkData.h"
//#include "network.h"
#include "parallelcoordinatesglobals.h"

#include "datastore.h"
#include "datastore.cpp"
#include "parallelcoordinateswidget.h"
#include "parallelcoordinateswidget.cpp"

#include "view3d.h"
#include "view3d.cpp"

using namespace std;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);

    ~MainWindow();

private slots:

    void on_actionQuit_triggered();

    void on_actionOpen_triggered();

    void on_actionCalc_triggered();

    void on_actionTest_triggered();

    void on_actionLoad_Data_from_File_triggered();

    void on_actionLoad_Default_Data_triggered();

    void on_actionGenerate_Random_Data_triggered();

    void on_actionDraw_data_out_of_range_changed();

    void on_pushButton_clicked();

protected:
    void resizeEvent(QResizeEvent* event) Q_DECL_OVERRIDE;
private:
    Ui::MainWindow *ui;
    DataStore<WIDGET_DATA_TYPE> dataStore;
    ParallelCoordinatesWidget<WIDGET_DATA_TYPE>* parallelCoordinatesPtr;
    View3D<WIDGET_DATA_TYPE>* view3dPtr;
    QTableWidget* tableWidget;

    void loadFileData(QString fileName);
};

#endif // MAINWINDOW_H

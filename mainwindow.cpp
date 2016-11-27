#include "mainwindow.h"
#include "ui_mainwindow.h"

//#include <boost/asio.hpp>
//#include <boost/bind.hpp>
//#include <boost/lexical_cast.hpp>
#include <QFileDialog>
#include <QSplitter>

#include <iostream>
#include <vector>
#include <iostream>

#include <iostream>
#include <fstream>
#include <sstream>
#include "datastore.h"
// #include "datastore.cpp"

#include "parallelcoordinateswidget.h"
#include "parallelcoordinatesglobals.h"


using namespace std;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{

    ui->setupUi(this);

    delete(this->ui->parallelCoordinatesPlace);
    delete(this->ui->view3dSpace);

    QSplitter *splitter = new QSplitter(Qt::Vertical, this->ui->centralWidget);


    parallelCoordinatesPtr = new ParallelCoordinatesWidget<WIDGET_DATA_TYPE>(this->ui->centralWidget);
    parallelCoordinatesPtr->setMinimumSize(QSize(300,150));
    parallelCoordinatesPtr->resize(QSize(300,300));

    splitter->addWidget(parallelCoordinatesPtr);

    tableWidget = new QTableWidget(this->ui->centralWidget);
    tableWidget->setShowGrid(false);
    tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);

    tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
    //tableWidget->setSelectionMode(QAbstractItemView::SingleSelection);
    tableWidget->setSelectionMode(QAbstractItemView::ExtendedSelection);

    tableWidget->setAlternatingRowColors(true);
    tableWidget->setStyleSheet("alternate-background-color: lightgrey;background-color: white;");

    //tableWidget->setsortingEnabled(true);
    /*
    tableWidget->setRowCount(0);
    QStringList tableHeader;
    tableHeader<<"#"<<"Name"<<"Text";
    tableWidget->setHorizontalHeaderLabels(tableHeader);
    */
    tableWidget->setVisible(false);
    splitter->addWidget(tableWidget);

    /*
    QTableView* tableView = new QTableView(this->ui->centralWidget);
    splitter->addWidget(tableView);

    tableView->setColu
    */

//    this->ui->verticalLayout->addWidget(parallelCoordinatesPtr);


    view3dPtr = new View3D<WIDGET_DATA_TYPE>(this->ui->centralWidget);
    //view3dPtr->setMinimumSize(300,300);

    view3dPtr->resize(QSize(300,300));

   // this->ui->verticalLayout->addWidget(view3dPtr);

     splitter->addWidget(view3dPtr);

     this->ui->verticalLayout->addWidget(splitter);



}

MainWindow::~MainWindow()
{    
    delete ui;

    delete parallelCoordinatesPtr;
    delete view3dPtr;
}



void MainWindow::on_actionQuit_triggered()
{
    qApp->quit();
}





void MainWindow::on_actionOpen_triggered()
{




}

void MainWindow::resizeEvent(QResizeEvent* event){
     QMainWindow::resizeEvent(event);

     // if (this->parallelCoordinatesPtr!=NULL) this->parallelCoordinatesPtr->recalculateDrawingLines();
}

void MainWindow::on_actionCalc_triggered()
{
    this->parallelCoordinatesPtr->calcDataInBackground();
}


void MainWindow::on_actionTest_triggered()
{

}


void MainWindow::loadFileData(QString fileName) {
    if (fileName!=NULL && fileName.length()>0) {
        ifstream is;
        is.open(fileName.toStdString(), ios::binary);

        this->dataStore.clearData();
//        this->parallelCoordinatesPtr->clearDataSet();

        std::string line;

        while (std::getline(is, line))
        {
            uint64_t data[17];

            istringstream iss(line);

            for (int i=0;i<17;i++)
                iss >> data[i];

            dataStore.addDataSet(17, data);
        }


        this->parallelCoordinatesPtr->setDataStorePtr(&dataStore);
        this->view3dPtr->setDataStorePtr(&dataStore);

        is.close();

        vector<DataSet<WIDGET_DATA_TYPE>>* dataSetPtr = dataStore.getDataSet();

        // Add data to tableWidget as well...
        tableWidget->setColumnCount(nrOfDimensions);
        tableWidget->setRowCount(dataSetPtr->size());

        uint64_t rowNr = 0;
        for (vector<DataSet<WIDGET_DATA_TYPE>>::iterator it = dataSetPtr->begin(); it!=dataSetPtr->end(); ++it) {
            for (int colNr=0; colNr<nrOfDimensions; colNr++) {
                DataSet<WIDGET_DATA_TYPE> data = *it;
                QTableWidgetItem* item = new QTableWidgetItem(0);
                item->setData(Qt::DisplayRole, data.dimVal[colNr]);

                tableWidget->setItem(rowNr, colNr, item);
            }
            rowNr++;
        }

        tableWidget->setSortingEnabled(true);
        tableWidget->setVisible(true);

    }

    /*
    QString path = QFileDialog::getExistingDirectory (this, tr("Directory"), directory.path());
    if ( path.isNull() == false )
    {
        directory.setPath(path);
    }*/

}

void MainWindow::on_actionLoad_Data_from_File_triggered()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"),"",tr("Files (*.*)"));
    // fileName = "/Users/wolfgangmeyerle/Downloads/of_v0.9.3_osx_release/apps/myApps/Salsa20/bin/Salsa20Debug.app/Contents/Resources/petyaData.dat";
    loadFileData(fileName);


}


void MainWindow::on_actionLoad_Default_Data_triggered()
{
    QString fileName = "/Users/wolfgangmeyerle/Downloads/of_v0.9.3_osx_release/apps/myApps/Salsa20/bin/Salsa20Debug.app/Contents/Resources/petyaData.dat";
    loadFileData(fileName);
}

void MainWindow::on_actionGenerate_Random_Data_triggered()
{
    dataStore.generateRandomDataSet(20);
    this->parallelCoordinatesPtr->setDataStorePtr(&dataStore);
    this->view3dPtr->setDataStorePtr(&dataStore);

    this->parallelCoordinatesPtr->calcDataInBackground();
}

void MainWindow::on_actionDraw_data_out_of_range_changed()
{
    bool drawNotInRange = this->ui->actionDraw_data_out_of_range->isChecked();
    this->parallelCoordinatesPtr->setDrawNotInRange(drawNotInRange);
}

void MainWindow::on_pushButton_clicked()
{
    this->view3dPtr->setWindowFlags(Qt::Window);
    this->view3dPtr->showFullScreen();
}

void MainWindow::keyPressEvent(QKeyEvent *keyEvent) {
    this->view3dPtr->keyPressedEventHandler(keyEvent);
}

void MainWindow::keyReleaseEvent(QKeyEvent * keyEvent) {
    this->view3dPtr->keyReleasedEventHandler(keyEvent);
}

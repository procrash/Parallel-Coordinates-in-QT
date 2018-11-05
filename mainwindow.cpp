#include "mainwindow.h"
#include "ui_mainwindow.h"



MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    parallelCoordinatesWidget =
            new ParallelCoordinatesWidget<WIDGET_DATA_TYPE>(this);

    this->setCentralWidget(parallelCoordinatesWidget);
//    this->ui->verticalLayout_2->addWidget(parallelCoordinatesWidget);

    addDataCars();

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::addDataCars() {


    dataStore.loadDataset("/Users/wolfgangmeyerle/Documents/workspace/ParallelCoordinates/carsDataset.csv",8,1 );

    this->parallelCoordinatesWidget->setDataStorePtr(&dataStore);
    this->parallelCoordinatesWidget->calcDataInBackground();

}

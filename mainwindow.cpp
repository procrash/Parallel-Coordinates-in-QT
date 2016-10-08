#include "mainwindow.h"
#include "ui_mainwindow.h"

//#include <boost/asio.hpp>
//#include <boost/bind.hpp>
//#include <boost/lexical_cast.hpp>
#include <QFileDialog>
#include <QQuickWidget>
#include <QSplitter>

#include <iostream>
#include <vector>
#include <iostream>

#include <iostream>
#include <fstream>
#include <sstream>
#include "datastore.h"

#include "parallelcoordinateswidget.h"
#include "surfacegraph.h"



using namespace std;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    delete(this->ui->parallelCoordinatesPlace);
    delete(this->ui->view3dSpace);

//    QSplitter *splitter = new QSplitter(parent);

    parallelCoordinatesPtr = new ParallelCoordinatesWidget<WIDGET_DATA_TYPE>(this->ui->centralWidget);
    parallelCoordinatesPtr->setMinimumSize(QSize(300,300));
    this->ui->verticalLayout->addWidget(parallelCoordinatesPtr);

    view3dPtr = new View3D<WIDGET_DATA_TYPE>(this->ui->centralWidget);
    view3dPtr->setMinimumSize(300,300);
    this->ui->verticalLayout->addWidget(view3dPtr);




    this->ui->quickWidget->setMinimumWidth(this->width());

//    this->ui->quickWidget->setResizeMode(QQuickWidget::SizeRootObjectToView);
//    this->ui->quickWidget->setSizePolicy(QSizePolicy::horizontalStretch());
    // this->ui->quickWidget->
    this->ui->quickWidget->setSource(QUrl("qrc:qml/Form.ui.qml"));


    Q3DSurface *graph = new Q3DSurface();

    QWidget *container = QWidget::createWindowContainer(graph);

    SurfaceGraph* surfaceGraph = new SurfaceGraph(graph);
    container->setMinimumWidth(300);
    container->setMinimumHeight(100);
    this->ui->verticalLayout->addWidget(container);

    surfaceGraph->enableSqrtSinModel(true);

    /*
    connect(ui->xSlider, SIGNAL(valueChanged(int)), ui->openGLWidget, SLOT(setXRotation(int)));
    connect(ui->openGLWidget, SIGNAL(xRotationChanged(int)), ui->xSlider, SLOT(setValue(int)));
    connect(ui->ySlider, SIGNAL(valueChanged(int)), ui->openGLWidget, SLOT(setYRotation(int)));
    connect(ui->openGLWidget, SIGNAL(yRotationChanged(int)), ui->ySlider, SLOT(setValue(int)));
    connect(ui->zSlider, SIGNAL(valueChanged(int)), ui->openGLWidget, SLOT(setZRotation(int)));
    connect(ui->openGLWidget, SIGNAL(zRotationChanged(int)), ui->zSlider, SLOT(setValue(int)));
    */

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
    this->parallelCoordinatesPtr->recalculateDrawingLines();
    this->update();
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

        this->parallelCoordinatesPtr->setMinMaxGUI();
        this->parallelCoordinatesPtr->recalculateDrawingLines();


        is.close();
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
    update();
}


void MainWindow::on_actionLoad_Default_Data_triggered()
{
    QString fileName = "/Users/wolfgangmeyerle/Downloads/of_v0.9.3_osx_release/apps/myApps/Salsa20/bin/Salsa20Debug.app/Contents/Resources/petyaData.dat";
    loadFileData(fileName);
    update();
}

void MainWindow::on_actionGenerate_Random_Data_triggered()
{
    dataStore.generateRandomDataSet(20);
    this->parallelCoordinatesPtr->setDataStorePtr(&dataStore);
    this->view3dPtr->setDataStorePtr(&dataStore);

    this->parallelCoordinatesPtr->setMinMaxGUI();
    this->parallelCoordinatesPtr->recalculateDrawingLines();

    update();
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

#include "mainwindow.h"
#include "ui_mainwindow.h"

//#include <boost/asio.hpp>
//#include <boost/bind.hpp>
//#include <boost/lexical_cast.hpp>
#include <iostream>
#include <vector>
//#include <boost/thread.hpp>
#include <iostream>
//#include "server.cpp"
//#include "client.cpp"
//#include <boost/lexical_cast.hpp>
//#include "network.h"
#include <QFileDialog>
#include <fstream>
#include <sstream>

#include "test.h"

using namespace std;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // networkData data1("Hello World", 7.5);
    // networkData data2("Data", 3.5);

    //this->dataToSend.push_back(data1);
    //this->dataToSend.push_back(data2);
    //ui->comboBoxServerSend->addItem("1", QVariant(1));
    //ui->comboBoxServerSend->addItem("2", QVariant(2));


    //this->ui->parallelCoordinates->generateRandomDataSet(200);


    // network.startDiscoveryServer(6666);
}

MainWindow::~MainWindow()
{
    delete ui;
}



void MainWindow::on_actionQuit_triggered()
{
    qApp->quit();
}

/*
void MainWindow::callback(vector<networkData> data) {
    cout << "Information retrieved "<< data.size() << endl;

    this->ui->comboBoxClientReceived->clear();
    this->dataReceived = data;
    for (int i=0; i<this->dataReceived.size(); i++) {
        this->ui->comboBoxClientReceived->addItem(boost::lexical_cast<string>(i+1).c_str(), QVariant(i+1));
    }

    if (this->dataReceived.size()>0) {

        this->ui->comboBoxClientReceived->setCurrentIndex(0);

        this->ui->lineEditClientTestString->setText(data[0].testString.c_str());
        this->ui->lineEditClientTestDouble->setText(boost::lexical_cast<string>(data[0].testDouble).c_str());

    }

}
*/
void MainWindow::on_pushButtonConnect_clicked()
{

    /*
    this->ui->comboBoxClientReceived->clear();


    QString QipAddress = ui->lineEditIPAddress->text();
    QString Qport = ui->lineEditPort->text();

    std::string ipAddressStr = QipAddress.toStdString();
    std::string portStr = Qport.toStdString();

    MainWindow* mw = this;

    /*
    clientThread = new boost::thread([=]{

        try
        {
            cout << "Starting Client Thread " << portStr << endl;
            boost::asio::io_service io_service;
            client client(io_service, ipAddressStr, portStr);

            client.mw = this;
            io_service.run();
        }
        catch (exception& e)
        {
          cerr << e.what() << endl;
        }
    });

*/
}


void MainWindow::on_pushButtonServer_clicked()
{
    /*
    QString buttonText = this->ui->pushButtonServer->text();

    if (buttonText == "start server") {
        cout << "Starting Server" << endl;
        QString QipAddress = ui->lineEditIPAddress->text();
        QString Qport = ui->lineEditPort->text();

        std::string ipAddressStr = QipAddress.toStdString();
        std::string portStr = Qport.toStdString();

        //unsigned short port = boost::lexical_cast<unsigned short>(portStr);

        /*
        serverThread = new boost::thread([=]{

            try
            {
                cout << "Starting Server Thread " << port << endl;


                server server(port);
                //server.setDataToTransmit(dataToSend);
                server.listen();
            }
            catch (exception& e)
            {
              cerr << e.what() << endl;
            }
        });
*/
    /*
        this->ui->pushButtonServer->setText("stop server");
    } else {
        cout << "Stopping Server Thread" << endl;

        //serverThread->interrupt();
        this->ui->pushButtonServer->setText("start server");
    }
*/
}

void MainWindow::on_pushButtonAddData_clicked()
{
    /*

    int i = this->ui->comboBoxServerSend->count()+1;

    //string strI = boost::lexical_cast<std::string>(i);

    //QString qstr(strI.c_str());

    string strTestString = this->ui->lineEdittestString->text().toStdString();

    double testDouble = this->ui->lineEditTestDouble->text().toDouble();

    //networkData aData = networkData(strTestString, testDouble);

    QVariant variant(i);

    //dataToSend.push_back(aData);

    //this->ui->comboBoxServerSend->addItem(qstr, variant);
*/
}



void MainWindow::on_comboBoxClientReceived_currentIndexChanged(const QString &arg1)
{
    /*
    int idx = this->ui->comboBoxClientReceived->currentIndex();

    if (idx<dataReceived.size()) {
        networkData selectedData = dataReceived[idx];
        this->ui->lineEditClientTestString->setText(selectedData.testString.c_str());
        this->ui->lineEditClientTestDouble->setText(boost::lexical_cast<string>(selectedData.testDouble).c_str());
    }*/
}

void MainWindow::on_comboBoxServerSend_currentIndexChanged(int index)
{
    /*
    int idx = this->ui->comboBoxServerSend->currentIndex();

    networkData selectedData = dataToSend[idx];
    this->ui->lineEdittestString->setText(selectedData.testString.c_str());
    this->ui->lineEditTestDouble->setText(boost::lexical_cast<string>(selectedData.testDouble).c_str());
*/
}

void MainWindow::on_actionDiscover_Network_triggered()
{



    cout << "Starting Network Discovery" << endl;
    // Network network;
    //network.discover("6666", 24);
}

void MainWindow::on_actionStart_Discovery_Server_triggered()
{
    cout << "Starting Network Discovery Server" << endl;
    //network.startDiscoveryServer(6666);
}

void MainWindow::on_actionTest_triggered()
{
    // Test test;
    // test.serverTest();

    this->ui->parallelCoordinates->generateRandomDataSet(20);
    this->ui->parallelCoordinates->setMinMaxGUI();

}

void MainWindow::on_actionOpen_triggered()
{


    QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"),"",tr("Files (*.*)"));
    fileName = "/Users/wolfgangmeyerle/Downloads/of_v0.9.3_osx_release/apps/myApps/Salsa20/bin/Salsa20Debug.app/Contents/Resources/petyaData.dat";

    if (fileName!=NULL && fileName.length()>0) {
        ifstream is;
        is.open(fileName.toStdString(), ios::binary);

        this->ui->parallelCoordinates->clearDataSet();

        std::string line;

        while (std::getline(is, line))
        {
            uint64_t data[17];

            istringstream iss(line);

            for (int i=0;i<17;i++)
                iss >> data[i];

            this->ui->parallelCoordinates->addDataSet(17, data);
        }


        this->ui->parallelCoordinates->setMinMaxGUI();
        this->ui->parallelCoordinates->recalculateDrawingLines();


        is.close();
    }
    /*
    QString path = QFileDialog::getExistingDirectory (this, tr("Directory"), directory.path());
    if ( path.isNull() == false )
    {
        directory.setPath(path);
    }*/

}

void MainWindow::on_actionCalc_triggered()
{
    this->ui->parallelCoordinates->recalculateDrawingLines();
}


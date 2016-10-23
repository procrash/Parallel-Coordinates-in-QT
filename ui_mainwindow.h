/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.7.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QFormLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QOpenGLWidget>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QAction *actionQuit;
    QAction *actionOpen;
    QAction *actionCalc;
    QAction *actionTest;
    QAction *actionDraw_data_out_of_range;
    QAction *actionGenerate_Random_Data;
    QAction *actionLoad_Default_Data;
    QAction *actionLoad_Data_from_File;
    QWidget *centralWidget;
    QFormLayout *formLayout;
    QWidget *parallelCoordinatesPlace;
    QVBoxLayout *verticalLayout;
    QOpenGLWidget *view3dSpace;
    QPushButton *pushButton;
    QMenuBar *menuBar;
    QMenu *menuFile;
    QMenu *menuData;
    QMenu *menuOptions;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QStringLiteral("MainWindow"));
        MainWindow->resize(589, 546);
        actionQuit = new QAction(MainWindow);
        actionQuit->setObjectName(QStringLiteral("actionQuit"));
        actionOpen = new QAction(MainWindow);
        actionOpen->setObjectName(QStringLiteral("actionOpen"));
        actionCalc = new QAction(MainWindow);
        actionCalc->setObjectName(QStringLiteral("actionCalc"));
        actionTest = new QAction(MainWindow);
        actionTest->setObjectName(QStringLiteral("actionTest"));
        actionDraw_data_out_of_range = new QAction(MainWindow);
        actionDraw_data_out_of_range->setObjectName(QStringLiteral("actionDraw_data_out_of_range"));
        actionDraw_data_out_of_range->setCheckable(true);
        actionGenerate_Random_Data = new QAction(MainWindow);
        actionGenerate_Random_Data->setObjectName(QStringLiteral("actionGenerate_Random_Data"));
        actionLoad_Default_Data = new QAction(MainWindow);
        actionLoad_Default_Data->setObjectName(QStringLiteral("actionLoad_Default_Data"));
        actionLoad_Data_from_File = new QAction(MainWindow);
        actionLoad_Data_from_File->setObjectName(QStringLiteral("actionLoad_Data_from_File"));
        centralWidget = new QWidget(MainWindow);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        formLayout = new QFormLayout(centralWidget);
        formLayout->setSpacing(6);
        formLayout->setContentsMargins(11, 11, 11, 11);
        formLayout->setObjectName(QStringLiteral("formLayout"));
        parallelCoordinatesPlace = new QWidget(centralWidget);
        parallelCoordinatesPlace->setObjectName(QStringLiteral("parallelCoordinatesPlace"));
        parallelCoordinatesPlace->setMinimumSize(QSize(300, 300));

        formLayout->setWidget(1, QFormLayout::FieldRole, parallelCoordinatesPlace);

        verticalLayout = new QVBoxLayout();
        verticalLayout->setSpacing(6);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));

        formLayout->setLayout(2, QFormLayout::FieldRole, verticalLayout);

        view3dSpace = new QOpenGLWidget(centralWidget);
        view3dSpace->setObjectName(QStringLiteral("view3dSpace"));
        QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(view3dSpace->sizePolicy().hasHeightForWidth());
        view3dSpace->setSizePolicy(sizePolicy);
        view3dSpace->setMinimumSize(QSize(100, 100));

        formLayout->setWidget(3, QFormLayout::FieldRole, view3dSpace);

        pushButton = new QPushButton(centralWidget);
        pushButton->setObjectName(QStringLiteral("pushButton"));
        pushButton->setMaximumSize(QSize(100, 16777215));

        formLayout->setWidget(4, QFormLayout::FieldRole, pushButton);

        MainWindow->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(MainWindow);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 589, 22));
        menuFile = new QMenu(menuBar);
        menuFile->setObjectName(QStringLiteral("menuFile"));
        menuData = new QMenu(menuFile);
        menuData->setObjectName(QStringLiteral("menuData"));
        menuOptions = new QMenu(menuBar);
        menuOptions->setObjectName(QStringLiteral("menuOptions"));
        MainWindow->setMenuBar(menuBar);
        mainToolBar = new QToolBar(MainWindow);
        mainToolBar->setObjectName(QStringLiteral("mainToolBar"));
        MainWindow->addToolBar(Qt::TopToolBarArea, mainToolBar);
        statusBar = new QStatusBar(MainWindow);
        statusBar->setObjectName(QStringLiteral("statusBar"));
        MainWindow->setStatusBar(statusBar);

        menuBar->addAction(menuFile->menuAction());
        menuBar->addAction(menuOptions->menuAction());
        menuFile->addAction(actionQuit);
        menuFile->addSeparator();
        menuFile->addAction(actionOpen);
        menuFile->addAction(actionCalc);
        menuFile->addAction(actionTest);
        menuFile->addAction(menuData->menuAction());
        menuData->addAction(actionGenerate_Random_Data);
        menuData->addAction(actionLoad_Default_Data);
        menuData->addAction(actionLoad_Data_from_File);
        menuOptions->addAction(actionDraw_data_out_of_range);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "MainWindow", 0));
        actionQuit->setText(QApplication::translate("MainWindow", "Quit", 0));
        actionOpen->setText(QApplication::translate("MainWindow", "Open", 0));
        actionCalc->setText(QApplication::translate("MainWindow", "Calc", 0));
        actionTest->setText(QApplication::translate("MainWindow", "test", 0));
        actionDraw_data_out_of_range->setText(QApplication::translate("MainWindow", "draw data out of range", 0));
        actionGenerate_Random_Data->setText(QApplication::translate("MainWindow", "Generate Random Data", 0));
        actionLoad_Default_Data->setText(QApplication::translate("MainWindow", "Load Default Data", 0));
        actionLoad_Data_from_File->setText(QApplication::translate("MainWindow", "Load Data from File", 0));
        pushButton->setText(QApplication::translate("MainWindow", "fullscreen", 0));
        menuFile->setTitle(QApplication::translate("MainWindow", "File", 0));
        menuData->setTitle(QApplication::translate("MainWindow", "Data", 0));
        menuOptions->setTitle(QApplication::translate("MainWindow", "Options", 0));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H

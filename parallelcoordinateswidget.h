#pragma once
#include <QWidget>
#include <vector>
#include <QDebug>
#include <iostream>

#include "qrangeslider.h"
#include "parallelcoordinatesglobals.h"
#include "datastore.h"

#include "qrangesliderobserver.h"

#include "parallelcoordinatesworker.h"
#include "parallelcoordinatesworker.cpp" // <- As this is a template class
#include "parallelcoordinatesworkerobserver.h"

using namespace std;

template<class T>
class ParallelCoordinatesWidget : public QWidget,
                                  public QRangeSliderObserver,
                                  public ParallelCoordinatesWorkerObserver
{
private:

    ParallelCoordinatesWorker<T> worker;

    vector<QRangeSlider*> slidersUnordered;




    DataStore<T>* dataStorePtr=NULL;
    vector<int> dataOrder;

    T* minValPtr;
    T* maxValPtr;

    /*
    vector<tuple<vector<QLineF>, QColor, vector<T>>> drawingLinesIn;
    vector<tuple<vector<QLineF>, QColor, vector<T>>> drawingLinesOut;

    vector<tuple<vector<QLineF>, QColor, vector<T>>> drawingLinesInNecessary;
    vector<tuple<vector<QLineF>, QColor, vector<T>>> drawingLinesOutNecessary;
    */



    bool minMaxValInitialized = false;
    bool minMaxGUISet = false;

    vector<QRangeSlider*> sliders;

    QPixmap pm;

    void reorderSliders();
    // void reduceDrawingDataSet();
    // void renderLines();

protected:
    void paintEvent(QPaintEvent *event) Q_DECL_OVERRIDE;
    void mouseMoveEvent(QMouseEvent *event) Q_DECL_OVERRIDE;
    void resizeEvent(QResizeEvent* event) Q_DECL_OVERRIDE;
    void parallelCoordinatesWorkerCalculationDone() Q_DECL_OVERRIDE;

public:
    ParallelCoordinatesWidget(QWidget *parent = 0);

    void clearDataSet();
    void recalculateDrawingLines();
    void setMinMaxGUI();
    void setDataStorePtr(DataStore<T>* dataStore);

    void setDrawNotInRange(bool drawNotInRange);
    void calcDataInBackground();


    void qrangeSliderTextboxFocused(QRangeSlider* source);
    void qrangeSliderMinMaxValChanged(QRangeSlider* source);



};


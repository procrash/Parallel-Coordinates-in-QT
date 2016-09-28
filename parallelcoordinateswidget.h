#pragma once
#include <QWidget>
#include <vector>
#include <QDebug>
#include <iostream>

#include "qrangeslider.h"
#include "parallelcoordinatesglobals.h"
#include "datastore.h"

using namespace std;

template<class T>
class ParallelCoordinatesWidget : public QWidget
{
private:

    vector<QRangeSlider*> slidersUnordered;

    bool drawNotInRange = false;

    DataStore<T>* dataStorePtr=NULL;

    T* minValPtr;
    T* maxValPtr;

    vector<tuple<vector<QLineF>, QColor, vector<T>>> drawingLinesIn;
    vector<tuple<vector<QLineF>, QColor, vector<T>>> drawingLinesOut;

    vector<tuple<vector<QLineF>, QColor, vector<T>>> drawingLinesInNecessary;
    vector<tuple<vector<QLineF>, QColor, vector<T>>> drawingLinesOutNecessary;

    bool minMaxValInitialized = false;
    bool minMaxGUISet = false;

    vector<QRangeSlider*> sliders;

    void reorderSliders();
    void reduceDrawingDataSet();

protected:
    void paintEvent(QPaintEvent *event) Q_DECL_OVERRIDE;
    void mouseMoveEvent(QMouseEvent *event) Q_DECL_OVERRIDE;
public:
    ParallelCoordinatesWidget(QWidget *parent = 0);

    void clearDataSet();
    void recalculateDrawingLines();
    void setMinMaxGUI();
    void setDataStorePtr(DataStore<T>* dataStore);

    void setDrawNotInRange(bool drawNotInRange);
};


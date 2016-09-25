#pragma once
#include <QWidget>
#include "qrangeslider.h"
#include <vector>
#include <QDebug>
#include <iostream>
#include "parallelcoordinatesglobals.h"

using namespace std;

template<class T>
struct DataSet{
    T dimVal[nrOfDimensions];
};

class ParallelCoordinatesWidget : public QWidget
{
private:
  //  vector<QRangeSlider> sliders;
    vector<DataSet<WIDGET_DATA_TYPE>> dataSets;

    vector<tuple<vector<QLineF>, QColor, vector<WIDGET_DATA_TYPE>>> drawingLinesIn;
    vector<tuple<vector<QLineF>, QColor, vector<WIDGET_DATA_TYPE>>> drawingLinesOut;

    vector<tuple<vector<QLineF>, QColor, vector<WIDGET_DATA_TYPE>>> drawingLinesInNecessary;
    vector<tuple<vector<QLineF>, QColor, vector<WIDGET_DATA_TYPE>>> drawingLinesOutNecessary;


    double minVal[nrOfDimensions];
    double maxVal[nrOfDimensions];

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

    void generateRandomDataSet(int nrOfValues);
    void recalculateDrawingLines();

    template<class T>
    void addDataSet(int dimenions, T*data);
    void setMinMaxGUI();



};


#pragma once
//  ^- As this is a template class
#include "parallelcoordinatesworker.h"
#include <vector>
#include <QColor>
#include <QPointF>
#include <QBrush>
#include <QPalette>
#include <QPen>
#include <QPainter>
#include <QImage>
#include <iostream>

#include "parallelcoordinatesworkerobserver.h"

using namespace std;

template<class T>
ParallelCoordinatesWorker<T>::ParallelCoordinatesWorker()
{

}

template<class T>
void ParallelCoordinatesWorker<T>::initialize(int xPositionDimensions[nrOfDimensions], int width, int height, const QPalette* palette, QPalette::ColorRole backgroundRole) {
    this->palPtr = palette;
    this->bg = backgroundRole;
    this->brush = palPtr->brush(bg);
    this->width = width;
    this->height = height;

    this->valuesHeight = height;

    this->dataOrder.clear();

    for (int i=0; i<nrOfDimensions; i++) {
        this->xPositionDimensions[i] = xPositionDimensions[i];
        this->dataOrder.push_back(i);
    }



}

template<class T>
void ParallelCoordinatesWorker<T>::registerParallelCoordinatesWorkerObserver(ParallelCoordinatesWorkerObserver* observer){
    if (std::find(observers.begin(), observers.end(), observer)==observers.end()) {
        observers.push_back(observer);
    }
}

template<class T>
void ParallelCoordinatesWorker<T>::unregisterParallelCoordinatesWorkerObserver(ParallelCoordinatesWorkerObserver* observer){
    if (std::find(observers.begin(), observers.end(), observer)!=observers.end()) {
        observers.erase(remove(observers.begin(), observers.end(), observer), observers.end());
    }
}


template<class T>
void ParallelCoordinatesWorker<T>::resize(int width, int height) {
    this->width = width;
    this->height = height;
    this->valuesHeight = height;
}

template <class T>
uint64_t ParallelCoordinatesWorker<T>::getYPositionForVal(T val, int dimensionNr) {

    T* minVals = dataStorePtr->getMinValPtr();

    T range = dataStorePtr->getDataRange(dimensionNr);

    uint64_t retVal = posStartY;

    if (range!=0) {
        uint64_t ret2 = ((T)(this->valuesHeight))*(val-minVals[dimensionNr])/range;

        if (minValDisplayedOnTop[dimensionNr]) {
            retVal += ret2;
        }
        else {
            retVal += valuesHeight-ret2;
        }
    }

    return retVal;
}


template <class T>
void ParallelCoordinatesWorker<T>::setDataStorePtr(DataStore<T>* dataStore) {
    this->dataStorePtr = dataStore;

    vector<DataSet<T>> *dataSetPtr = dataStorePtr->getDataSet();

    // Initialize Colors...
    dataColorIsPart.clear();
    for (vector<DataSet<WIDGET_DATA_TYPE>>::iterator it = dataSetPtr->begin(); it != dataSetPtr->end(); ++it) {
         QColor colorLineIsPart(rand()%255,rand()%255, rand()%255);
         dataColorIsPart.push_back(colorLineIsPart);
    }

    // Initialize Min/Max Vals for Drawing Scope
    for (int i=0; i<nrOfDimensions; i++) {
        this->currentSetMinVal[i] = dataStorePtr->getMinValPtr()[i];
        this->currentSetMaxVal[i] = dataStorePtr->getMaxValPtr()[i];
    }

    // Initialize Dimension Data Order...
    for (int i=0; i<nrOfDimensions; i++) {
        dataOrder.push_back(i);
    }

}

template<class T>
void ParallelCoordinatesWorker<T>::recalculateDrawingLines() {

    drawingLinesIn.clear();
    drawingLinesOut.clear();

    // Iterate through data and test if data is part of the users selection or not
    // if not, then paint dependecy grey, if paint dependency black
    vector<DataSet<T>> *dataSetPtr = dataStorePtr->getDataSet();

    if (dataSetPtr==NULL) return;

    uint64_t colorIdx = 0;

    for (vector<DataSet<WIDGET_DATA_TYPE>>::iterator it = dataSetPtr->begin(); it != dataSetPtr->end(); ++it) {
            DataSet<T> ds = *it;

        vector<QLineF> lines;

        bool lastPointInitialized = false;
        QPointF lastPoint;

        QColor color;
        QColor colorLineIsPart = dataColorIsPart.at(colorIdx);

        colorIdx++;

        QColor colorLineIsNotPart(LINE_IS_NOT_PART_COLOR);

        bool lineIsPart = true;

        vector<T> values;

        color = colorLineIsPart;
        for (int dim=0; dim<nrOfDimensions; dim++) {

            int idxPos = dataOrder.at(dim);

            int xPos = xPositionDimensions[dataOrder.at(dim)];
            // cout << "XPos is " << xPos << endl;
            int yPos = getYPositionForVal(ds.dimVal[idxPos], dataOrder.at(dim));

            if (currentSetMinVal[dataOrder.at(dim)]>ds.dimVal[idxPos] ||
                currentSetMaxVal[dataOrder.at(dim)]<ds.dimVal[idxPos]) {
                // Data is out of selection scope, paint it grey
                lineIsPart = false;
                color = colorLineIsNotPart;
            }

            QPointF point((double) xPos, (double) yPos);

            if (lastPointInitialized) {
                QLineF line(lastPoint, point);
                lines.push_back(line);
            }

            lastPointInitialized = true;
            lastPoint = point;
        }


        tuple<vector<QLineF>, QColor, vector<T>> entry(lines,color, values);


        if (lineIsPart==true) {
            drawingLinesIn.push_back(entry);
        } else {
           drawingLinesOut.push_back(entry);
        }
    }

    reduceDrawingDataSet();

    drawingLinesIn = drawingLinesInNecessary;
    drawingLinesOut = drawingLinesOutNecessary;

    renderLines(width, height);

    updateGui();
}


template<class T>
void ParallelCoordinatesWorker<T>::reduceDrawingDataSet() {

    drawingLinesInNecessary.clear();

    map<tuple<qreal, qreal, qreal, qreal>, bool> linesMap;

    // TODO: Remove, next is for debugging only...
    uint64_t nrOfLines = 0;
    uint64_t nrOfLinesReduced = 0;


    for (int i=0; i<drawingLinesIn.size(); i++) {
        tuple<vector<QLineF>, QColor, vector<T>> entry = drawingLinesIn.at(i);

        vector<QLineF> lines = get<0>(entry);
        vector<QLineF> linesReduced;



        for (int j=0; j<lines.size(); j++) {

            qreal x1 = lines.at(j).x1();
            qreal x2 = lines.at(j).x2();

            qreal y1 = lines.at(j).y1();
            qreal y2 = lines.at(j).y2();

            // cout << "Points:" << x1 << " " << y1 << " " << x1 << " " << y2 << endl;
            tuple<qreal, qreal, qreal, qreal> key(x1,y1,x2, y2);
            if (linesMap.find(key) == linesMap.end()) {
                linesMap[key] = true;
                linesReduced.push_back(lines.at(j));
            }
        }

        nrOfLines += lines.size();
        nrOfLinesReduced+=linesReduced.size();

        tuple<vector<QLineF>, QColor, vector<T>> newEntry(linesReduced, get<1>(entry), get<2>(entry));
        drawingLinesInNecessary.push_back(newEntry);
    }
    cout << "Original amount of Data " << nrOfLines << " reduced to " << nrOfLinesReduced << endl;


    if (this->drawNotInRange) {
        drawingLinesOutNecessary.clear();
        map<tuple<qreal, qreal, qreal, qreal>, bool> linesMapOut;

        for (int i=0; i<drawingLinesOut.size(); i++) {
            tuple<vector<QLineF>, QColor, vector<T>> entry = drawingLinesOut.at(i);

            vector<QLineF> lines = get<0>(entry);
            vector<QLineF> linesReduced;

            for (int j=0; j<lines.size(); j++) {

                qreal x1 = lines.at(j).x1();
                qreal x2 = lines.at(j).x2();

                qreal y1 = lines.at(j).y1();
                qreal y2 = lines.at(j).y2();

                tuple<qreal, qreal, qreal, qreal> key(x1, y1,x2, y2);
                if (linesMapOut.find(key) == linesMapOut.end()) {
                    linesMapOut[key] = true;
                    linesReduced.push_back(lines.at(j));
                }
            }

            tuple<vector<QLineF>, QColor, vector<T>> newEntry(linesReduced, get<1>(entry), get<2>(entry));
            drawingLinesOutNecessary.push_back(newEntry);
        }
    }
}

template<class T>
void ParallelCoordinatesWorker<T>::renderLines(int width, int height) {

    if (width==0 || height==0) return;

    QImage result(width, height, QImage::Format_ARGB32);


    QPainter painter;

    painter.begin(&result);

    painter.setRenderHint(QPainter::Antialiasing);

    painter.fillRect(0,0, width, height, brush);

    if (this->drawNotInRange) {
        if (drawingLinesOut.size()>1)
        {
            for (int i=0; i<drawingLinesOut.size(); i++) {

                 tuple<vector<QLineF>, QColor, vector<T>> entry = drawingLinesOut.at(i);

                 vector<QLineF> lines = get<0>(entry);
                 QColor color = get<1>(entry);

                 QPen pen(color, LINE_THICKNESS , Qt::SolidLine, Qt::FlatCap, Qt::RoundJoin);
                 painter.setPen(pen);

                 for (int k=0; k<lines.size(); k++) {
                     QLineF line = lines[k];
                     painter.drawLine(line);
                 }
            }
        }
    }

        if (drawingLinesIn.size()>1)
        {
            for (int i=0; i<drawingLinesIn.size(); i++) {

                 tuple<vector<QLineF>, QColor, vector<T>> entry = drawingLinesIn.at(i);

                 vector<QLineF> lines = get<0>(entry);
                 QColor color = get<1>(entry);

                 QPen pen(color, LINE_THICKNESS , Qt::SolidLine, Qt::FlatCap, Qt::RoundJoin);
                 painter.setPen(pen);

                 for (int k=0; k<lines.size(); k++) {
                     QLineF line = lines[k];
                     painter.drawLine(line);
                 }
            }
        }

        painter.end();

        this->pm = result;
}

template<class T>
void ParallelCoordinatesWorker<T>::updateGui() {
    for (vector<ParallelCoordinatesWorkerObserver*>::iterator it=observers.begin(); it!=observers.end(); ++it) {
        (*it)->parallelCoordinatesWorkerCalculationDone();
    }
}

template<class T>
QImage ParallelCoordinatesWorker<T>::getImage() {
    return pm;
}

template<class T>
void ParallelCoordinatesWorker<T>::setXPositionDimensions(int* xPositionDimensions) {
    for (int i=0; i<nrOfDimensions;i++) {
        this->xPositionDimensions[i] = xPositionDimensions[i];
    }
}


template<class T>
void ParallelCoordinatesWorker<T>::setMinMaxValScope(T minValScope[nrOfDimensions], T maxValScope[nrOfDimensions]) {
    for (int i=0; i<nrOfDimensions; i++) {
        this->currentSetMinVal[i] = minValScope[i];
        this->currentSetMaxVal[i] = maxValScope[i];
    }
}

template<class T>
void ParallelCoordinatesWorker<T>::setMinValDisplayedOnTop(bool minValDisplayedOnTop[nrOfDimensions]) {
    for (int i=0; i<nrOfDimensions; i++) {
        this->minValDisplayedOnTop[i] = minValDisplayedOnTop[i];
    }
}

template<class T>
void ParallelCoordinatesWorker<T>::run() {
    cout << "Starting calculation" << endl;

    this->recalculateDrawingLines();
    this->reduceDrawingDataSet();
    this->renderLines(width, height);
    cout << "Done" << endl;

    pm.save("image.png", "PNG");
    updateGui();
}

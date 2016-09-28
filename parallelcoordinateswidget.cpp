#include "parallelcoordinateswidget.h"
#include "qrangeslider.h"
#include <QWidget>
#include <QLayout>
#include <QPainter>
#include <QDebug>
#include <vector>
#include <QRect>
#include <QPaintEvent>

#include <map>
#include <iostream>
#include <cstdlib>

#include <tuple>

#include <QTableWidget>
#include <QHBoxLayout>
#include <QVBoxLayout>


#include "datastore.h"

using namespace std;

template <class T>
ParallelCoordinatesWidget<T>::ParallelCoordinatesWidget(QWidget *parent): QWidget(parent)
{
    srand (time(NULL));

    this->setMouseTracking(true);
    //this->setParent(parent);

    QVBoxLayout* vLayout = new QVBoxLayout;

    QHBoxLayout* hLayout = new QHBoxLayout;

    for (int i=0; i<nrOfDimensions; i++) {
        QRangeSlider* aSlider = new QRangeSlider(this);
        this->slidersUnordered.push_back(aSlider);
        hLayout->addWidget(aSlider);
    }

    vLayout->addLayout(hLayout);
    this->setLayout(vLayout);


}

template <class T>
void ParallelCoordinatesWidget<T>::setDataStorePtr(DataStore<T>* dataStore) {
    this->dataStorePtr = dataStore;

    minValPtr = dataStore->getMinValPtr();
    maxValPtr = dataStore->getMaxValPtr();
}

template <class T>
void ParallelCoordinatesWidget<T>::clearDataSet() {
    dataStorePtr->clearData();
    this->update();
}




template <class T>
void ParallelCoordinatesWidget<T>::setMinMaxGUI() {

    reorderSliders();

    for (int i=0; i<this->sliders.size(); i++) {
        sliders[i]->setMinVal(minValPtr[i]);
        sliders[i]->setMaxVal(maxValPtr[i]);
    }

    minMaxGUISet = true;
}

template <class T>
void ParallelCoordinatesWidget<T>::mouseMoveEvent(QMouseEvent *event) {
    int mouseX =  event->pos().x();
    int mouseY; //  = event->pos().y();

    /*
    // Unset highlight as events are not always fired in control...
    foreach (QRangeSlider* slider, findChildren<QRangeSlider*>()) {
        if (mouseX > slider->pos().x()+ slider->width() ||
            mouseX < slider->pos().x() ||
            mouseY < slider->pos().y() ||
            mouseY > slider->pos().y() + slider->height()) {
            slider->unsetHighlights();
        }
    }*/
}

template <class T>
void ParallelCoordinatesWidget<T>::reduceDrawingDataSet() {

    drawingLinesInNecessary.clear();

    map<tuple<qreal, qreal, qreal, qreal>, bool> linesMap;

    // TODO: Remove, next is for debugging only...
    uint64_t nrOfLines = 0;
    uint64_t nrOfLinesReduced = 0;


    for (int i=0; i<drawingLinesIn.size(); i++) {
        tuple<vector<QLineF>, QColor, vector<WIDGET_DATA_TYPE>> entry = drawingLinesIn.at(i);

        vector<QLineF> lines = get<0>(entry);
        vector<QLineF> linesReduced;



        for (int j=0; j<lines.size(); j++) {

            qreal x1 = lines.at(j).x1();
            qreal x2 = lines.at(j).x2();

            qreal y1 = lines.at(j).y1();
            qreal y2 = lines.at(j).y2();

            tuple<qreal, qreal, qreal, qreal> key(x1,y1,x2, y2);
            if (linesMap.find(key) == linesMap.end()) {
                linesMap[key] = true;
                linesReduced.push_back(lines.at(j));
            }
        }

        nrOfLines += lines.size();
        nrOfLinesReduced+=linesReduced.size();

        tuple<vector<QLineF>, QColor, vector<WIDGET_DATA_TYPE>> newEntry(linesReduced, get<1>(entry), get<2>(entry));
        drawingLinesInNecessary.push_back(newEntry);
    }
    cout << "Original amount of Data " << nrOfLines << " reduced to " << nrOfLinesReduced << endl;

    if (this->drawNotInRange) {
        drawingLinesOutNecessary.clear();
        map<pair<qreal, qreal>, bool> linesMapOut;

        for (int i=0; i<drawingLinesOut.size(); i++) {
            tuple<vector<QLineF>, QColor, vector<WIDGET_DATA_TYPE>> entry = drawingLinesOut.at(i);

            vector<QLineF> lines = get<0>(entry);
            vector<QLineF> linesReduced;

            for (int j=0; j<lines.size(); j++) {

                qreal y1 = lines.at(j).y1();
                qreal y2 = lines.at(j).y2();

                pair<qreal, qreal> key(y1,y2);
                if (linesMapOut.find(key) == linesMapOut.end()) {
                    linesMapOut[key] = true;
                    linesReduced.push_back(lines.at(j));
                }
            }

            tuple<vector<QLineF>, QColor, vector<WIDGET_DATA_TYPE>> newEntry(linesReduced, get<1>(entry), get<2>(entry));
            drawingLinesInNecessary.push_back(newEntry);
        }
    }



}

template <class T>
void ParallelCoordinatesWidget<T>::recalculateDrawingLines() {

    // this->minMaxGUISet = false;
    // setMinMaxGUI();

    drawingLinesIn.clear();
    drawingLinesOut.clear();

    // Iterate through data and test if data is part of the users selection or not
    // if not, then paint dependecy grey, if paint dependency black
    vector<DataSet<WIDGET_DATA_TYPE>> *dataSetPtr = dataStorePtr->getDataSet();

    for (vector<DataSet<WIDGET_DATA_TYPE>>::iterator it = dataSetPtr->begin(); it != dataSetPtr->end(); ++it) {
        DataSet<WIDGET_DATA_TYPE> ds = *it;

        // vector<QPointF> linePoints;
        vector<QLineF> lines;

        bool lastPointInitialized = false;
        QPointF lastPoint;

        QColor color;
        // QColor colorLineIsPart("#000000");
        QColor colorLineIsPart(rand()%255,rand()%255, rand()%255);
        QColor colorLineIsNotPart("#AAAAAA");

        bool lineIsPart = true;
        int i = 0;
        vector<WIDGET_DATA_TYPE> values;

        color = colorLineIsPart;

        foreach (QRangeSlider* slider, this->sliders) {

            int xPos = slider->pos().x()+slider->getXPositionBar()+slider->getSliderWidth()/2;
            int yPos = slider->getYPositionForVal(ds.dimVal[i]);

            if (slider->getCurrentSetMinVal()>ds.dimVal[i] || slider->getCurrentSetMaxVal()<ds.dimVal[i]) {
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
            i++;
        }


        tuple<vector<QLineF>, QColor, vector<WIDGET_DATA_TYPE>> entry(lines,color, values);

        if (lineIsPart) {
            drawingLinesIn.push_back(entry);
        } else {
            drawingLinesOut.push_back(entry);
        }
    }

    reduceDrawingDataSet();

    drawingLinesIn = drawingLinesInNecessary;
    drawingLinesOut = drawingLinesOutNecessary;

    update();
}

template <class T>
void ParallelCoordinatesWidget<T>::paintEvent(QPaintEvent *evt) {

    // QRect rct = evt->rect();

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    if (this->drawNotInRange) {
        if (drawingLinesOut.size()>1)
        {
            for (int i=0; i<drawingLinesOut.size(); i++) {

                 tuple<vector<QLineF>, QColor, vector<WIDGET_DATA_TYPE>> entry = drawingLinesOut.at(i);

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

                 tuple<vector<QLineF>, QColor, vector<WIDGET_DATA_TYPE>> entry = drawingLinesIn.at(i);

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

template <class T>
void ParallelCoordinatesWidget<T>::reorderSliders() {
	

    // Find QRange sliders in children and sort them according to their Xorder in a vector
    multimap<int, QRangeSlider*> rangeSlidersOrdered;

    for (vector<QRangeSlider*>::iterator it = slidersUnordered.begin(); it!=slidersUnordered.end(); ++it) {
        QRangeSlider* slider = *it;
        rangeSlidersOrdered.insert(pair<int, QRangeSlider*>(slider->pos().x(), slider));
    }

    /*
    foreach (QRangeSlider* slider, findChildren<QRangeSlider*>()) {
        QRangeSlider* obj =  dynamic_cast<QRangeSlider*>(slider);
        if (obj!=NULL) {
            rangeSlidersOrdered.insert(pair<int, QRangeSlider*>(slider->pos().x(), slider));
        }
    }
    */

    this->sliders.clear();

    for (multimap<int, QRangeSlider*>::iterator it = rangeSlidersOrdered.begin(); it!=rangeSlidersOrdered.end(); ++it) {
         QRangeSlider* slider = (*it).second;

		this->sliders.push_back(slider);
    }

}

template<class T>
void ParallelCoordinatesWidget<T>::setDrawNotInRange(bool drawNotInRange) {
    this->drawNotInRange = drawNotInRange;
}



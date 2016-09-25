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
#include <time.h>

#include <tuple>

#include <QTableWidget>
#include <QHBoxLayout>
#include <QVBoxLayout>

using namespace std;

ParallelCoordinatesWidget::ParallelCoordinatesWidget(QWidget *parent)
{
    this->setMouseTracking(true);
    //this->setParent(parent);

    QVBoxLayout* vLayout = new QVBoxLayout;

    QHBoxLayout* hLayout = new QHBoxLayout;

    for (int i=0; i<nrOfDimensions; i++)
        hLayout->addWidget(new QRangeSlider(this));



   // QTableWidget* table = new QTableWidget(this);

   // vLayout->addWidget(table);

    vLayout->addLayout(hLayout);

    this->setLayout(vLayout);

    /*
    this->reorderSliders();
    this->setMinMaxGUI();
    */
}

void ParallelCoordinatesWidget::clearDataSet() {
    this->dataSets.clear();
    this->minMaxValInitialized = false;
    this->update();
}

void ParallelCoordinatesWidget::generateRandomDataSet(int nrOfValues) {
    clearDataSet();
    srand(time(NULL));

    WIDGET_DATA_TYPE vals[nrOfDimensions];

    for (int count = 0; count < nrOfValues; count++) {

        for (int i=0; i<nrOfDimensions; i++) {
            vals[i] = (WIDGET_DATA_TYPE)(rand()%1000)/ (WIDGET_DATA_TYPE)(rand()%1000+1);
        }

        addDataSet(nrOfDimensions, vals);
    }

    this->update();
}




template<class T>
void ParallelCoordinatesWidget::addDataSet(int dimensions, T*data){
    DataSet<T> d;
    for (int i=0; i<dimensions; i++) {
        d.dimVal[i] = data[i];
    }
    dataSets.push_back(d);

    if (minMaxValInitialized) {
        for (int i=0; i<dimensions; i++) {
            if (minVal[i]>d.dimVal[i]) minVal[i] = d.dimVal[i];
            if (maxVal[i]<d.dimVal[i]) maxVal[i] = d.dimVal[i];
        }
    } else
    {
        for (int i=0; i<dimensions; i++) {
            minVal[i] = maxVal[i] = d.dimVal[i];
        }
        minMaxValInitialized = true;
    }
}


void ParallelCoordinatesWidget::setMinMaxGUI() {
    reorderSliders();

    for (int i=0; i<this->sliders.size(); i++) {
        sliders[i]->setMinVal(minVal[i]);
        sliders[i]->setMaxVal(maxVal[i]);

        cout << i << ": Minval " << minVal[i] << " MaxVal " << maxVal[i] << endl;
    }

    minMaxGUISet = true;
}


void ParallelCoordinatesWidget::mouseMoveEvent(QMouseEvent *event) {
    // int mouseX =  event->pos().x();
    // int mouseY; //  = event->pos().y();

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

void ParallelCoordinatesWidget::reduceDrawingDataSet() {

    cout << "Reducing..." << endl;
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


    #ifdef DRAW_NOT_IN_RANGE
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
    #endif



}

void ParallelCoordinatesWidget::recalculateDrawingLines() {

    // this->minMaxGUISet = false;
    // setMinMaxGUI();

    drawingLinesIn.clear();
    drawingLinesOut.clear();

    // Iterate through data and test if data is part of the users selection or not
    // if not, then paint dependecy grey, if paint dependency black
    for (vector<DataSet<WIDGET_DATA_TYPE>>::iterator it = dataSets.begin(); it != dataSets.end(); ++it) {
        DataSet<WIDGET_DATA_TYPE> ds = *it;

        // vector<QPointF> linePoints;
        vector<QLineF> lines;

        bool lastPointInitialized = false;
        QPointF lastPoint;

        QColor color;
        QColor colorLineIsPart("#000000");
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

void ParallelCoordinatesWidget::paintEvent(QPaintEvent *evt) {

    QRect rct = evt->rect();

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    #ifdef DRAW_NOT_IN_RANGE
    if (drawingLinesOut.size()>1)
    {
        for (int i=0; i<drawingLinesOut.size(); i++) {

             tuple<vector<QLineF>, QColor, vector<WIDGET_DATA_TYPE>> entry = drawingLinesOut.at(i);

             vector<QLineF> lines = get<0>(entry);
             QColor color = get<1>(entry);

             QPen pen(color, 2 , Qt::SolidLine, Qt::FlatCap, Qt::RoundJoin);
             painter.setPen(pen);

             for (int k=0; k<lines.size(); k++) {
                 QLineF line = lines[k];
                 painter.drawLine(line);
             }
        }
    }
    #endif

        if (drawingLinesIn.size()>1)
        {
            for (int i=0; i<drawingLinesIn.size(); i++) {

                 tuple<vector<QLineF>, QColor, vector<WIDGET_DATA_TYPE>> entry = drawingLinesIn.at(i);

                 vector<QLineF> lines = get<0>(entry);
                 QColor color = get<1>(entry);

                 QPen pen(color, 2 , Qt::SolidLine, Qt::FlatCap, Qt::RoundJoin);
                 painter.setPen(pen);

                 for (int k=0; k<lines.size(); k++) {
                     QLineF line = lines[k];
                     painter.drawLine(line);
                 }
            }
        }


}

void ParallelCoordinatesWidget::reorderSliders() {
	

    // Find QRange sliders in children and sort them according to their Xorder in a vector
    multimap<int, QRangeSlider*> rangeSlidersOrdered;

    foreach (QRangeSlider* slider, findChildren<QRangeSlider*>()) {
        rangeSlidersOrdered.insert(pair<int, QRangeSlider*>(slider->pos().x(), slider));
    }

    this->sliders.clear();

    for (multimap<int, QRangeSlider*>::iterator it = rangeSlidersOrdered.begin(); it!=rangeSlidersOrdered.end(); ++it) {
         QRangeSlider* slider = (*it).second;

		this->sliders.push_back(slider);
    }

}

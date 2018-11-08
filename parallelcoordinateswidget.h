#pragma once
#include <QWidget>
#include <vector>
#include <QDebug>
#include <iostream>


// QT Includes
#include <QWidget>
#include <QLayout>
#include <QPainter>
#include <QDebug>
#include <QRect>
#include <QPaintEvent>
#include <QTableWidget>
#include <QHBoxLayout>
#include <QVBoxLayout>

// Std Includes
#include <map>
#include <vector>
#include <tuple>

#include <iostream>
#include <cstdlib>

// project includes
#include "datastore.h"
#include "qrangeslider.h"

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
    const int marginBottom = 50;

    std::vector<double> sliderRangeDiffs;

    ParallelCoordinatesWorker<T> worker;

    vector<QRangeSlider*> slidersUnordered;




    DataStore<T>* dataStorePtr=NULL;
    vector<int> dataOrder;

    T* minValPtr;
    T* maxValPtr;

    bool minMaxValInitialized = false;
    bool minMaxGUISet = false;

    vector<QRangeSlider*> sliders;
    vector<std::size_t> sliderOrdersNrs;

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
    ParallelCoordinatesWidget(QWidget *parent = nullptr);

    void clearDataSet();
    void recalculateDrawingLines();
    void setMinMaxGUI();
    void setDataStorePtr(DataStore<T>* dataStore);

    void setDrawNotInRange(bool drawNotInRange);
    void calcDataInBackground();


    void qrangeSliderTextboxFocused(QRangeSlider* source);
    void qrangeSliderMinMaxValChanged(QRangeSlider* source);



};








template <class T>
ParallelCoordinatesWidget<T>::ParallelCoordinatesWidget(QWidget *parent): QWidget(parent)
{
    srand (time(NULL));



    this->setMouseTracking(true);



    int singleWidth = (parent->width()/nrOfDimensions);

    for (int i=0; i<nrOfDimensions; i++) {
        QRangeSlider* aSlider = new QRangeSlider(this);
        this->slidersUnordered.push_back(aSlider);


        aSlider->setGeometry(QRect(0+ singleWidth*i,0,singleWidth,parent->height()-marginBottom));

        //hLayout->addWidget(aSlider);

        // define data order
        dataOrder.push_back(i);
    }

    for (size_t j=0; j<this->slidersUnordered.size();j++) {
        QRangeSlider* aSlider = this->slidersUnordered.at(j);
        aSlider->registerQRangeSliderObserver(this);
    }

    //vLayout->addLayout(hLayout);
    //this->setLayout(vLayout);



    this->reorderSliders();

    int xPositionSliders[nrOfDimensions];
    for (size_t i=0; i<sliders.size(); i++) {
        QRangeSlider* slider = sliders.at(i);
        xPositionSliders[i] = slider->x()+
                              slider->getXPositionBar()+
                              slider->getSliderWidth()/2;
    }


    worker.initialize(xPositionSliders, this->width(), this->height(), &this->palette(), this->backgroundRole());
    worker.registerParallelCoordinatesWorkerObserver(this);
}



template <class T>
void ParallelCoordinatesWidget<T>::setDataStorePtr(DataStore<T>* dataStore) {
    this->dataStorePtr = dataStore;

    // Set Label Texts
    std::vector<std::string> labelTexts = dataStore->getLabelTexts();

    for (std::size_t idx=0; idx<labelTexts.size(); idx++) {
      sliders[idx]->setLabel(labelTexts[idx]);
    }


    minValPtr = dataStore->getMinValPtr();
    maxValPtr = dataStore->getMaxValPtr();


    this->setMinMaxGUI();

    // Set dataSets also in worker thread...
    worker.setDataStorePtr(dataStore);
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

    calcDataInBackground();
}

template <class T>
void ParallelCoordinatesWidget<T>::mouseMoveEvent(QMouseEvent *event) {
    //if (!moveSlider) return;

    int mouseX =  event->pos().x();
    int mouseY  = event->pos().y();



    // Unset highlight as events are not always fired in control...

    foreach (QRangeSlider* slider, findChildren<QRangeSlider*>()) {
        if (slider->shallMove()) {
            std::cout << "ShallMove" << std::endl;
/*        if (
            (mouseX >= slider->pos().x() &&
             mouseX <  slider->pos().x() + slider->width()) &&
            (mouseY >= slider->pos().y() &&
             mouseY < slider->pos().y() + slider->height())
            ) {
*/
            slider->move(mouseX, mouseY);
            this->update();
            //slider->unsetHighlights();
        }
    }

    // Remember diffs

    sliderRangeDiffs.clear();
    for (int i=0; i<nrOfDimensions; i++) {
        sliderOrdersNrs[i];
        sliderRangeDiffs.push_back(static_cast<double>(sliders[sliderOrdersNrs[i]]->pos().x())/static_cast<double>(this->parentWidget()->width()));
    }

}


template <class T>
void ParallelCoordinatesWidget<T>::qrangeSliderTextboxFocused(QRangeSlider* source) {
    for (int i=0; i<this->slidersUnordered.size();i++) {
        QRangeSlider* aSlider = this->slidersUnordered.at(i);
        if (source!=aSlider) aSlider->deselect();
    }
}

template <class T>
void ParallelCoordinatesWidget<T>::qrangeSliderMinMaxValChanged(QRangeSlider* source) {
    calcDataInBackground();
}


template <class T>
void ParallelCoordinatesWidget<T>::paintEvent(QPaintEvent *evt) {

    if (this->sliders.size()==0) return;
    QRangeSlider* first = sliders.at(0);
    QRangeSlider* last = sliders.at(sliders.size()-1);

    int startX =  first->x()+first->getXPositionBar()+first->getSliderWidth()/2;
    int endX = last->x()+last->getXPositionBar()+last->getSliderWidth()/2;

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    //std::cout << "Drawing from " << startX << " " << first->getSlideBarStartY() << " to:" << endX-startX << " " <<first->getSlideBarHeight() << std::endl;

    painter.drawPixmap(startX,first->getSlideBarStartY(),endX-startX, first->getSlideBarHeight(), pm);
    //pm.save("/home/wolfgang/test.png");
}

template <class T>
void ParallelCoordinatesWidget<T>::reorderSliders() {


    // Find QRange sliders in children and sort them according to their Xorder in a vector
    multimap<int, QRangeSlider*> rangeSlidersOrdered;

    std::cout << "Slider Unordered Positions:";

    for (vector<QRangeSlider*>::iterator it = slidersUnordered.begin(); it!=slidersUnordered.end(); ++it) {
        QRangeSlider* slider = *it;
        rangeSlidersOrdered.insert(pair<int, QRangeSlider*>(slider->pos().x(), slider));
        std::cout << slider->pos().x() << " ";
    }

    std::cout << std::endl;



    this->sliders.clear();
    this->sliderOrdersNrs.clear();

    for (multimap<int, QRangeSlider*>::iterator it = rangeSlidersOrdered.begin(); it!=rangeSlidersOrdered.end(); ++it) {
         QRangeSlider* slider = (*it).second;

        this->sliders.push_back(slider);

         for (int idx=0; idx<slidersUnordered.size(); ++idx) {
             if (slidersUnordered[idx]->pos().x()==(*it).first) {
                 sliderOrdersNrs.push_back(idx);
                 break;
             }
         }
    }


    /*
    // Replace widget order so that a window resize event will trigger the layout mechanism to work in correct order

    QHBoxLayout* hBoxLayout = (QHBoxLayout*) this->layout()->children().at(0);

    for (QRangeSlider* slider : slidersUnordered) {
        hBoxLayout->removeWidget(slider);
    }

    for (std::size_t idx = 0; idx<sliderOrdersNrs.size(); ++idx) {
        hBoxLayout->addWidget(slidersUnordered[sliderOrdersNrs[idx]]);
    }
    */


}

template<class T>
void ParallelCoordinatesWidget<T>::setDrawNotInRange(bool drawNotInRange) {
    worker.drawNotInRange = drawNotInRange;
}

template<class T>
void ParallelCoordinatesWidget<T>::calcDataInBackground() {
    // cout << "Widget Size: " << this->width() << " " << this->height() << endl;


    // Order slider along x-Axis
    this->reorderSliders();

    if (this->sliders.size()==0) return;

    // Determine dimensions of Image to calculate...
    int imageHeight = this->sliders.at(0)->getSlideBarHeight();

    std::cout << "Image Height:" << imageHeight << std::endl;

    QRangeSlider* first = this->sliders.at(0);
    QRangeSlider* last = this->sliders.at(this->sliders.size()-1);

    int startX = first->x()+
                 first->getXPositionBar()+
                 first->getSliderWidth()/2;
    int endX = last->x()+
            last->getXPositionBar()+
            last->getSliderWidth()/2;

    int imageWidth = endX-startX;


    worker.resize(imageWidth, imageHeight);


    // Determine positions of dimensions...
    int xPositionSliders[nrOfDimensions];

    for (size_t i=0; i<sliders.size(); i++) {

        QRangeSlider* slider = sliders.at(i);
        xPositionSliders[i] = slider->x()+
                              slider->getXPositionBar()+
                              slider->getSliderWidth()/2 -startX;

       // cout << "XPos set is " <<  xPositionSliders[i]  << endl;
    }

    worker.setXPositionDimensions(xPositionSliders);

    T minValSliders[nrOfDimensions];
    T maxValSliders[nrOfDimensions];

    for (size_t i=0; i<sliders.size(); i++) {
        QRangeSlider* slider = sliders.at(i);
        minValSliders[i] = slider->getCurrentSetMinVal();
        maxValSliders[i] = slider->getCurrentSetMaxVal();
    }
    worker.setMinMaxValScope(minValSliders, maxValSliders);

    bool minValDisplayedOnTop[nrOfDimensions];
    for (size_t i=0; i<sliders.size(); i++) {
        QRangeSlider* slider = sliders.at(i);
        minValDisplayedOnTop[sliderOrdersNrs[i]] = slider->getMinValDisplayedOnTop();
    }

    worker.setMinValDisplayedOnTop(minValDisplayedOnTop);

    /*
    if (worker.isRunning()) {
        worker.terminate();
    }*/


    std::cout << std::endl;

    worker.setSliderOrders(sliderOrdersNrs);

    worker.start();
}

template<class T>
void ParallelCoordinatesWidget<T>::parallelCoordinatesWorkerCalculationDone() {
    cout << "Calculation done" << endl;
    pm = QPixmap::fromImage(worker.getImage());
    update();

    for (QRangeSlider* slider : sliders) {
        slider->recalcPositions();
        slider->update();
    }


}

template<class T>
void ParallelCoordinatesWidget<T>::resizeEvent(QResizeEvent* event) {

//    QHBoxLayout* hBoxLayout = (QHBoxLayout*)this->layout()->children().at(0);

    /*
    // Redorder QRange Sliders as after a resize the order seems to be restored from the sequence....
    QHBoxLayout* hBoxLayout = (QHBoxLayout*)this->layout()->children().at(0);

    for (QRangeSlider* slider : slidersUnordered) {
        hBoxLayout->removeWidget(slider);
    }

    for (std::size_t idx = 0; idx<sliderOrdersNrs.size(); ++idx) {
        hBoxLayout->addWidget(slidersUnordered[sliderOrdersNrs[idx]]);
    }*/



    int singleWidth = this->parentWidget()->width() / nrOfDimensions;


    std::vector<double> xVals;
    std::vector<double> diffs;



    if (sliderRangeDiffs.size()==nrOfDimensions)
    for (int i=0; i<nrOfDimensions; i++) {
        this->slidersUnordered[i]->setGeometry(QRect(sliderRangeDiffs[i]*this->parentWidget()->width(),0,singleWidth,parentWidget()->height()-marginBottom));

    }

    calcDataInBackground();
}

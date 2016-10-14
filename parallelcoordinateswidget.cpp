#include "parallelcoordinateswidget.h"

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

        // define data order
        dataOrder.push_back(i);
    }

    for (size_t j=0; j<this->slidersUnordered.size();j++) {
        QRangeSlider* aSlider = this->slidersUnordered.at(j);
        aSlider->registerQRangeSliderObserver(this);
    }

    vLayout->addLayout(hLayout);
    this->setLayout(vLayout);

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
    //  int mouseX =  event->pos().x();
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

    painter.drawPixmap(startX,first->getSlideBarStartY(),endX-startX, first->getSlideBarHeight(), pm);
}

template <class T>
void ParallelCoordinatesWidget<T>::reorderSliders() {
	

    // Find QRange sliders in children and sort them according to their Xorder in a vector
    multimap<int, QRangeSlider*> rangeSlidersOrdered;

    for (vector<QRangeSlider*>::iterator it = slidersUnordered.begin(); it!=slidersUnordered.end(); ++it) {
        QRangeSlider* slider = *it;
        rangeSlidersOrdered.insert(pair<int, QRangeSlider*>(slider->pos().x(), slider));
    }

    this->sliders.clear();

    for (multimap<int, QRangeSlider*>::iterator it = rangeSlidersOrdered.begin(); it!=rangeSlidersOrdered.end(); ++it) {
         QRangeSlider* slider = (*it).second;

		this->sliders.push_back(slider);
    }
}

template<class T>
void ParallelCoordinatesWidget<T>::setDrawNotInRange(bool drawNotInRange) {
    worker.drawNotInRange = drawNotInRange;
}

template<class T>
void ParallelCoordinatesWidget<T>::calcDataInBackground() {
    cout << "Widget Size: " << this->width() << " " << this->height() << endl;


    // Order slider along x-Axis
    this->reorderSliders();

    if (this->sliders.size()==0) return;

    // Determine dimensions of Image to calculate...
    int imageHeight = this->sliders.at(0)->getSlideBarHeight();

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
        minValDisplayedOnTop[i] = slider->getMinValDisplayedOnTop();
    }

    worker.setMinValDisplayedOnTop(minValDisplayedOnTop);

    worker.start();
}

template<class T>
void ParallelCoordinatesWidget<T>::parallelCoordinatesWorkerCalculationDone() {
    cout << "Calculation done" << endl;
    pm = QPixmap::fromImage(worker.getImage());
    update();
}

template<class T>
void ParallelCoordinatesWidget<T>::resizeEvent(QResizeEvent* event) {

    calcDataInBackground();
}

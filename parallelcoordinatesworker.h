#pragma once

#include <QThread>>
#include <vector>
#include "parallelcoordinatesglobals.h"
// #include "dataset.h"
#include "datastore.h"
#include <QImage>
#include <QPalette>

#include "parallelcoordinatesworkerobserver.h"


using namespace std;

template<class T>
class ParallelCoordinatesWorker: public QThread
{
private:
    int posStartY{0};
    int valuesHeight;

    int width;
    int height;

    DataStore<T>* dataStorePtr=NULL;
    vector<std::size_t> dataOrder;

    bool minValDisplayedOnTop[nrOfDimensions];
    int xPositionDimensions[nrOfDimensions];

    T currentSetMinVal[nrOfDimensions];
    T currentSetMaxVal[nrOfDimensions];


    const QPalette* palPtr;
    QPalette::ColorRole bg;
    QBrush brush;

    vector<tuple<vector<QLineF>, QColor, vector<T>>> drawingLinesIn;
    vector<tuple<vector<QLineF>, QColor, vector<T>>> drawingLinesOut;

    vector<tuple<vector<QLineF>, QColor, vector<T>>> drawingLinesInNecessary;
    vector<tuple<vector<QLineF>, QColor, vector<T>>> drawingLinesOutNecessary;

    vector<QColor> dataColorIsPart;

    vector<ParallelCoordinatesWorkerObserver*> observers;

    QImage pm;

    uint64_t getYPositionForVal(T val, int dimensionNr);

    void recalculateDrawingLines();
    void reduceDrawingDataSet();
    void renderLines(int width, int height);

    void updateGui();

    void run();
public:
    bool drawNotInRange = false;

    ParallelCoordinatesWorker();
    void setDataStorePtr(DataStore<T>* dataStore);
    void initialize(int xPositionDimensions[nrOfDimensions], int width, int height, const QPalette* palette, QPalette::ColorRole backgroundRole);
    void setXPositionDimensions(int* xPositionDimensions);
    void setMinMaxValScope(T minValScope[nrOfDimensions], T maxValScope[nrOfDimensions]);
    void setMinValDisplayedOnTop(bool minValDisplayedOnTop[nrOfDimensions]);
    void resize(int width, int height);
    QImage getImage();

    void registerParallelCoordinatesWorkerObserver(ParallelCoordinatesWorkerObserver* observer);
    void unregisterParallelCoordinatesWorkerObserver(ParallelCoordinatesWorkerObserver* observer);

    void setSliderOrders(std::vector<size_t> orders);

protected:

};

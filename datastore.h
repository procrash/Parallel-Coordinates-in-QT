#pragma once


#include <vector>

#include "parallelcoordinatesglobals.h"
#include "dataset.h"

using namespace std;

template<class T>
class DataStore
{
private:
    vector<DataSet<T>> dataSets;
    T minVal[nrOfDimensions];
    T maxVal[nrOfDimensions];
    bool minMaxValInitialized;

public:
    DataStore();

    void clearData();

    void addDataSet(int dimenions, T*data);
    vector<DataSet<T>>* getDataSetPtr();
//    WIDGET_DATA_TYPE getMinValDimension(int dimensionNr);
//    WIDGET_DATA_TYPE getMaxValDimension(int dimensionNr);
    void generateRandomDataSet(int nrOfValues);

    T* getMinValPtr();
    T* getMaxValPtr();
    vector<DataSet<T>>* getDataSet();
};


#pragma once
#include <time.h>
#include <stdlib.h>

#include "datastore.h"


template<class T>
DataStore<T>::DataStore()
{
   minMaxValInitialized = false;
}

template<class T>
void DataStore<T>::clearData() {
    dataSets.clear();
    minMaxValInitialized = false;
}


template<class T>
void DataStore<T>::addDataSet(int dimensions, T*data){
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

template<class T>
void DataStore<T>::generateRandomDataSet(int nrOfValues) {
    clearData();
    srand(time(NULL));



    T vals[nrOfDimensions];

    for (int count = 0; count < nrOfValues; count++) {

        for (int i=0; i<nrOfDimensions; i++) {
            vals[i] = (T)(rand()%1000)/ (T)(rand()%1000+1);
        }

        DataStore::addDataSet(nrOfDimensions, vals);
    }

}

template<class T>
T* DataStore<T>::getMinValPtr() {
    return minVal;
}

template<class T>
T* DataStore<T>::getMaxValPtr() {
    return maxVal;
}

template<class T>
vector<DataSet<T>>* DataStore<T>::getDataSet() {
    return &dataSets;
}



/*
WIDGET_DATA_TYPE DataStore::getMinValDimension(int dimensionNr) {
    return minVal[dimensionNr];
}

WIDGET_DATA_TYPE DataStore::getMaxValDimension(int dimensionNr) {
    return maxVal[dimensionNr];

}
*/

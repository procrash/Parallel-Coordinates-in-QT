#pragma once

#include <vector>
#include <ctime>
#include <stdlib.h>

#include <fstream>
#include <string>
#include <sstream>
#include <boost/algorithm/string.hpp>
#include <boost/lexical_cast.hpp>

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

    T getDataRange(int dimensionNr) {
        if (dimensionNr>nrOfDimensions) return 0;

        T dimMaxVal = maxVal[dimensionNr];
        T dimMinVal = minVal[dimensionNr];

        return (dimMaxVal>=dimMinVal) ? dimMaxVal-dimMinVal : dimMinVal-dimMaxVal;
    }

    T* getMinValPtr();
    T* getMaxValPtr();
    vector<DataSet<T>>* getDataSet();

    std::vector<std::string> getLabelTexts() {
        std::vector<std::string> result = {"mpg","cyl","dsp","hp","lbs","acc","year","origin"};
        return result;
    }

    void loadDataset(std::string fileName, std::size_t dimensions, std::size_t startIdx=0, std::uint64_t skipLines=0, std::vector<size_t> skipCols = std::vector<size_t>()) {

        this->clearData();

        if (fileName.length()>0) {
            ifstream is;
            is.open(fileName, ios::binary);

            this->clearData();

            std::string line;

            while (skipLines>0 && std::getline(is, line)) {
                skipLines--;
            }



            while (std::getline(is, line))
            {
                WIDGET_DATA_TYPE data[nrOfDimensions];

                vector<string> strs;
                boost::split(strs,line,boost::is_any_of(","));

                istringstream iss(line);

                std::vector<T> vals;

                for (std::size_t i=startIdx;i<startIdx+dimensions;i++) {
                    // iss >> data[i];
                    try {
                        boost::trim(strs[i]);

                        for (auto skipCol : skipCols) {
                            if (skipCol==i) continue;
                        }
                        vals.push_back(boost::lexical_cast<double>(strs[i]));
                    }
                    catch (boost::bad_lexical_cast ex) {

                    }
                }


                if (vals.size()==nrOfDimensions)
                for (int i=0;i<nrOfDimensions;i++) {
                    data[i] = boost::lexical_cast<double>(vals[i]);
                }


                addDataSet(nrOfDimensions, data);
            }


            is.close();


        }


    }
};


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




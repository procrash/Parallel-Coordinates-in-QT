#pragma once

#include <vector>
#include <algorithm>
#include "textboxfocusedobserver.h"

using namespace std;
class TextboxFocusedObservable
{
public:
    TextboxFocusedObservable();

    vector<TexboxFocusedObserver> observers;

    void registerTextBoxFocusedObservable(TexboxFocusedObserver observer) {
        if (find(observers.begin(), observers.end(), observer)==observers.end()) {
            observers.push_back(observer);
        }
    }

    void unregisterTextBoxFocusedObservable(TexboxFocusedObserver observer) {
        if (find(observers.begin(), observers.end(), observer)!=observers.end()) {
            observers.erase(remove(observers.begin(), observers.end(), observer), observers.end());
        }
    }


};

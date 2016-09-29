#pragma once

class TexboxFocusedObserver{
    virtual void neighborhoodTextboxFocused(){

    }


public:
    bool operator==(const TexboxFocusedObserver& obs)
    {
        return this==&obs;
    }
};

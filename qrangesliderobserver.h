#pragma once

// Signals and Slots don't mix together with template classes as moc can't compile the corresponding header files,
// therefore an observer / observable pattern
// could have also resolved it by deriving the template class from the slot class but this is a cleaner approach.
class QRangeSlider;

class QRangeSliderObserver
{
public:
    QRangeSliderObserver();

    virtual void qrangeSliderTextboxFocused(QRangeSlider* source);
    virtual void qrangeSliderMinMaxValChanged(QRangeSlider* source);
};

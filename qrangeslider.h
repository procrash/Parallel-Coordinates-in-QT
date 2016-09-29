#ifndef QRANGESLIDER_H
#define QRANGESLIDER_H

#include <QWidget>
#include <QLineEdit>

#include "parallelcoordinatesglobals.h"

#include "textboxfocusedobservable.h"
#include "textboxfocusedobserver.h"


class QRangeSlider : public QWidget, TextboxFocusedObservable, TexboxFocusedObserver
{
public:
    QRangeSlider(QWidget *parent = 0);

    QLineEdit lineEditTop;
    QLineEdit lineEditBottom;

    WIDGET_DATA_TYPE getCurrentSetMinVal();
    WIDGET_DATA_TYPE getCurrentSetMaxVal();

    void setMinVal(WIDGET_DATA_TYPE val);
    void setMaxVal(WIDGET_DATA_TYPE val);

    uint64_t getYPositionForVal(WIDGET_DATA_TYPE val);

    double getXPositionBar();
    double getSliderWidth();
    double getCircleRadius();
    void unsetHighlights();
    void neighborhoodTextboxFocused();



private:

    bool mousePressed = false;
    bool minValGrab = false;
    bool maxValGrab = false;

    bool hightlightTopTextBox = false;
    bool hightlightBottomTextBox = false;
    bool hightlightTopGrabHandle = false;
    bool hightlightBottomGrabHandle = false;


    int slideBarStartY=0;
    int slideBarHeight=0;

    int xPositionSliderBar;

    int textBoxLocationTopX;
    int textBoxLocationTopY;

    int textBoxLocationBottomX;
    int textBoxLocationBottomY;


    int textBoxWidthTopAndBottom;
    int textBoxHeightTopAndBottom;


    int sliderWidth = 0;


    QPointF centerMinVal;
    QPointF centerMaxVal;

    // Lowest Value which can be set
    WIDGET_DATA_TYPE minVal = 0;

    // Highest Value which can be set
    WIDGET_DATA_TYPE maxVal = 0;

    // Actual set values
    WIDGET_DATA_TYPE currentSetMinVal = 0;
    WIDGET_DATA_TYPE currentSetMaxVal = 0;

    const int circleRadius = 26/2;

    void hideTextboxesIfNecessary();
    void paintTextBoxWithValue(QPainter* painter, int x, int y, int width, int height, double val, int precision, bool drawHighlighted);
    void drawGrabHandle(QPainter* painter, int x, int y, int circleRadius, bool drawHighlighted);
    void drawSlideBar(QPainter* painter, int x, int y, int width, int height,QColor strokeColor, QColor gradientColor1, QColor gradientColor2);
    void paintSlider(QPainter* painter, int x, int y , int width, int height, int rangeStart, int rangeEnd);
    bool hitTestTopGrabHandle(int x, int y);
    bool hitTestBottomGrabHandle(int x, int y);
    bool hitTestTopTextBox(int x, int y);
    bool hitTestBottomTextBox(int x, int y);
    inline void resetButtonHighlights();

protected:
    void paintEvent(QPaintEvent *event) Q_DECL_OVERRIDE;
    void mouseMoveEvent(QMouseEvent *event) Q_DECL_OVERRIDE;
    void mousePressEvent(QMouseEvent *event) Q_DECL_OVERRIDE;
    void mouseReleaseEvent(QMouseEvent *event) Q_DECL_OVERRIDE;
    void mouseDoubleClickEvent(QMouseEvent * event) Q_DECL_OVERRIDE;
    void resizeEvent(QResizeEvent * event)  Q_DECL_OVERRIDE;
    virtual void enterEvent(QEvent * event) Q_DECL_OVERRIDE;
    virtual void leaveEvent(QEvent * event) Q_DECL_OVERRIDE;
};

#endif // QRANGESLIDER_H

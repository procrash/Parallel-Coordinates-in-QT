#pragma once

#include <QWidget>
#include <QLineEdit>
#include <vector>

#include "parallelcoordinatesglobals.h"
#include "qrangesliderobserver.h"

using namespace std;

class QRangeSlider : public QWidget
{
    Q_OBJECT

public slots:
    void on_topTextBox_EditingFinishedTriggered();
    void on_bottomTextBox_EditingFinishedTriggered();

public:
    QRangeSlider(QWidget *parent = 0);


    void registerQRangeSliderObserver(QRangeSliderObserver* observer);
    void unregisterQRangeSliderObserver(QRangeSliderObserver* observer);


    QLineEdit lineEditTop;
    QLineEdit lineEditBottom;

    WIDGET_DATA_TYPE getCurrentSetMinVal();
    WIDGET_DATA_TYPE getCurrentSetMaxVal();

    void setMinVal(WIDGET_DATA_TYPE val);
    void setMaxVal(WIDGET_DATA_TYPE val);

    //uint64_t getYPositionForVal(WIDGET_DATA_TYPE val);
    // uint64_t getYPositionForVal(WIDGET_DATA_TYPE val, bool isHandle = false);
    uint64_t getYPositionForVal(WIDGET_DATA_TYPE val, bool dontFlip = false);
    double getXPositionBar();
    double getSliderWidth();
    double getCircleRadius();
    void unsetHighlights();

    void deselect();
    int getSlideBarHeight();
    int getSlideBarStartY();
    bool getMinValDisplayedOnTop();

private:

    vector<QRangeSliderObserver*> observers;

    bool minValDisplayedOnTop = true;

    bool mousePressed = false;
    bool topValGrab = false;
    bool bottomValGrab = false;

    bool hightlightTopTextBox = false;
    bool hightlightBottomTextBox = false;
    bool hightlightTopGrabHandle = false;
    bool hightlightBottomGrabHandle = false;

    const int circleRadius = 26/2;

    int slideBarStartY=circleRadius + 30;
    int slideBarHeight=0;

    int xPositionSliderBar;



    int textBoxLocationTopX;
    int textBoxLocationTopY;

    int textBoxLocationBottomX;
    int textBoxLocationBottomY;


    int textBoxWidthTopAndBottom;
    int textBoxHeightTopAndBottom;


    int sliderWidth = 10;


    QPointF centerTopVal;
    QPointF centerBottomVal;

    int grabHandleYPositionTop;
    int grabHandleYPositionBottom;

    // Lowest Value which can be set
    WIDGET_DATA_TYPE minVal = 0;

    // Highest Value which can be set
    WIDGET_DATA_TYPE maxVal = 0;

    // Actual set values
    WIDGET_DATA_TYPE currentSetTopVal = 0;
    WIDGET_DATA_TYPE currentSetBottomVal = 0;


    void hideTextboxesIfNecessary();
    void paintTextBoxWithValue(QPainter* painter, int x, int y, int width, int height, double val, int precision, bool drawHighlighted);
    void drawGrabHandle(QPainter* painter, int x, int y, int circleRadius, bool drawHighlighted);
    void drawSlideBar(QPainter* painter, int x, int y, int width, int height,QColor strokeColor, QColor gradientColor1, QColor gradientColor2);
    void paintSlider(QPainter* painter, int x, int y , int width, int height, int rangeStart, int rangeEnd);
    bool hitTestTopGrabHandle(int x, int y);
    bool hitTestBottomGrabHandle(int x, int y);
    bool hitTestTopTextBox(int x, int y);
    bool hitTestBottomTextBox(int x, int y);
    bool hitTestOnSliderBar(int x, int y);
    inline void resetButtonHighlights();


    void informObserversTextBoxFocused();
    void informObserversMinMaxValChanged();

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


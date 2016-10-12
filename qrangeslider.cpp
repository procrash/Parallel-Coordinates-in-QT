#include "qrangeslider.h"
#include <qpainter.h>
#include <qwidget.h>
#include <QtWidgets>
#include <string>
#include <iostream>
// #include <boost/lexical_cast.hpp>
#include <sstream>
#include <iomanip>
#include <algorithm>

#include <QObject>

using namespace std;



QRangeSlider::QRangeSlider(QWidget *parent)
{
    this->setMouseTracking(true);
    this->setMinimumWidth(100);

    this->textBoxWidthTopAndBottom = 60;
    this->textBoxHeightTopAndBottom = 20;

    // Position Textboxes Top and Bottom
    lineEditTop.setParent(this);
    lineEditTop.setMaximumWidth(textBoxWidthTopAndBottom);
    lineEditTop.setMinimumWidth(textBoxWidthTopAndBottom);
    lineEditTop.setAlignment(Qt::AlignCenter);


    lineEditBottom.setParent(this);
    lineEditBottom.setMaximumWidth(textBoxWidthTopAndBottom);
    lineEditBottom.setMinimumWidth(textBoxWidthTopAndBottom);
    lineEditBottom.setAlignment(Qt::AlignCenter);


    lineEditTop.move(textBoxLocationTopX,textBoxLocationTopY);
    lineEditBottom.move(textBoxLocationBottomX,textBoxLocationBottomY);

    //Always last action, otherwise TextBoxes seem to be visible
    lineEditTop.setVisible(false);
    lineEditBottom.setVisible(false);


    connect(&lineEditTop, SIGNAL(editingFinished()), this, SLOT(on_topTextBox_EditingFinishedTriggered()));
    connect(&lineEditBottom, SIGNAL(editingFinished()), this, SLOT(on_bottomTextBox_EditingFinishedTriggered()));

}

void QRangeSlider::registerQRangeSliderObserver(QRangeSliderObserver* observer){
    if (std::find(observers.begin(), observers.end(), observer)==observers.end()) {
        observers.push_back(observer);
    }
}

void QRangeSlider::unregisterQRangeSliderObserver(QRangeSliderObserver* observer){
    if (std::find(observers.begin(), observers.end(), observer)!=observers.end()) {
        observers.erase(remove(observers.begin(), observers.end(), observer), observers.end());
    }
}

void QRangeSlider::deselect() {
    unsetHighlights();
    hideTextboxesIfNecessary();
    update();
}

void QRangeSlider::resizeEvent(QResizeEvent * event) {
    xPositionSliderBar = 15+20;

    textBoxLocationTopX = (xPositionSliderBar)-50/2;
    textBoxLocationTopY = 5;

    textBoxLocationBottomX = (xPositionSliderBar)-50/2;
    int height = this->size().height();
    // height=200;
    textBoxLocationBottomY = height-20;

    lineEditTop.move(textBoxLocationTopX,textBoxLocationTopY);
    lineEditBottom.move(textBoxLocationBottomX,textBoxLocationBottomY);

    grabHandleYPositionTop = this->getYPositionForVal(this->currentSetTopVal)-circleRadius;
    grabHandleYPositionBottom = this->getYPositionForVal(this->currentSetBottomVal)-circleRadius;


    centerTopVal=QPointF(this->xPositionSliderBar+this->sliderWidth/2, grabHandleYPositionTop); // Store position for Mouse Hit Test
    centerBottomVal=QPointF(this->xPositionSliderBar+this->sliderWidth/2,grabHandleYPositionBottom);

}

WIDGET_DATA_TYPE QRangeSlider::getCurrentSetMinVal() {
    if (this->currentSetBottomVal<=this->currentSetTopVal)
        return this->currentSetBottomVal;
    else
        return this->currentSetTopVal;
}

WIDGET_DATA_TYPE QRangeSlider::getCurrentSetMaxVal() {
    if (this->currentSetBottomVal>=this->currentSetTopVal)
        return this->currentSetBottomVal;
    else
        return this->currentSetTopVal;
}

void QRangeSlider::setMinVal(WIDGET_DATA_TYPE val) {
    this->minVal = val;


    this->currentSetTopVal = val;

    grabHandleYPositionTop    = this->getYPositionForVal(this->currentSetTopVal)-circleRadius;
    centerTopVal=QPointF(this->xPositionSliderBar+this->sliderWidth/2, grabHandleYPositionTop); // Store position for Mouse Hit Test

}

void QRangeSlider::setMaxVal(WIDGET_DATA_TYPE val) {
    this->maxVal = val;
    this->currentSetBottomVal = val;

    grabHandleYPositionBottom = this->getYPositionForVal(this->currentSetBottomVal)-circleRadius;
    centerBottomVal=QPointF(this->xPositionSliderBar+this->sliderWidth/2,grabHandleYPositionBottom);
}

double QRangeSlider::getXPositionBar() {
    return this->xPositionSliderBar;
}

double QRangeSlider::getSliderWidth() {
    return this->sliderWidth;
}

double QRangeSlider::getCircleRadius() {
    return this->circleRadius;
}



uint64_t QRangeSlider::getYPositionForVal(WIDGET_DATA_TYPE val, bool dontFlip) {

    WIDGET_DATA_TYPE range = (maxVal>minVal)? (maxVal - minVal) : minVal-maxVal;

    uint64_t retVal = this->slideBarStartY +circleRadius;

    if (range!=0) {
        uint64_t ret2 = ((WIDGET_DATA_TYPE)(this->slideBarHeight))*(val-minVal)/range;

        if (minValDisplayedOnTop || dontFlip) {
            retVal += ret2;
        }
        else {
            retVal += slideBarHeight-ret2;
        }


    }

    // if (retVal<this->slideBarStartY +circleRadius) retVal=this->slideBarStartY +circleRadius;
    // if (retVal>this->slideBarStartY+this->slideBarHeight) retVal = this->slideBarStartY+this->slideBarHeight;

   // if (retVal>1000) retVal=1000;

    return retVal;
}


void QRangeSlider::paintEvent(QPaintEvent *) {

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    paintSlider(&painter, this->xPositionSliderBar,slideBarStartY,this->sliderWidth,this->size().height()-55-30, this->minVal, this->maxVal);

}

void QRangeSlider::paintSlider(QPainter* painter, int x, int y , int width, int height, int rangeStart, int rangeEnd) {

    WIDGET_DATA_TYPE topVal;
    WIDGET_DATA_TYPE bottomVal;

    if (this->minValDisplayedOnTop) {
         topVal = this->currentSetTopVal;
         bottomVal = this->currentSetBottomVal;
    } else {
        topVal = this->currentSetBottomVal;
        bottomVal = this->currentSetTopVal;
    }

    paintTextBoxWithValue(painter, textBoxLocationTopX,   textBoxLocationTopY,    textBoxWidthTopAndBottom, textBoxHeightTopAndBottom, topVal, 2, this->hightlightTopTextBox);
    paintTextBoxWithValue(painter, textBoxLocationBottomX,textBoxLocationBottomY, textBoxWidthTopAndBottom, textBoxHeightTopAndBottom, bottomVal, 2, this->hightlightBottomTextBox);

    slideBarHeight = height;

    // Draw unfilled slide area
    drawSlideBar(painter, x, y, width, height,QColor("#BBBBBB"), QColor("#D0D0D0"),QColor("#E1E1E1"));



    // draw filled blueish slide area
    drawSlideBar(painter, x, grabHandleYPositionTop, width, grabHandleYPositionBottom-grabHandleYPositionTop,QColor("#BBBBBB"), QColor("#5C96C5"),QColor("#76ABD3"));

    // Draw Circles for Min Value

    drawGrabHandle(painter, x+width/2, grabHandleYPositionTop, circleRadius, this->hightlightTopGrabHandle);
    // Draw Circle for Max Value
    drawGrabHandle(painter, x+width/2, grabHandleYPositionBottom, circleRadius, hightlightBottomGrabHandle);


    /*
    QGraphicsDropShadowEffect* effect = new QGraphicsDropShadowEffect();
    effect->setBlurRadius(5);
    this->setGraphicsEffect(effect);
    */


}


void QRangeSlider::mouseMoveEvent(QMouseEvent *event) {


    int mouseX = event->pos().x();
    int mouseY = event->pos().y();

    // Test if we need to highlight anything...
    this->hightlightTopGrabHandle = hitTestTopGrabHandle(mouseX, mouseY);
    this->hightlightBottomGrabHandle = hitTestBottomGrabHandle(mouseX, mouseY);

    double newVal = 0;

    if (topValGrab || bottomValGrab) {
        // Calculate the new minimum value

        int y = event->pos().y()-this->slideBarStartY;

        // Limit Ranges
        if (y>this->slideBarHeight) y= this->slideBarHeight;
        if (y<0) y=0;

        if (abs( grabHandleYPositionTop-(y+slideBarStartY)) <
            abs(grabHandleYPositionBottom-(y+slideBarStartY))
           )
            grabHandleYPositionTop    = y+slideBarStartY;
        else
            grabHandleYPositionBottom = y+slideBarStartY;

        centerTopVal=QPointF(this->xPositionSliderBar+this->sliderWidth/2, grabHandleYPositionTop); // Store position for Mouse Hit Test
        centerBottomVal=QPointF(this->xPositionSliderBar+this->sliderWidth/2,grabHandleYPositionBottom);

        double newValTop    = (double)(grabHandleYPositionTop-slideBarStartY)*(maxVal-minVal)/(double) this->slideBarHeight+minVal;
        double newValBottom = (double)(grabHandleYPositionBottom-slideBarStartY)*(maxVal-minVal)/(double) this->slideBarHeight+minVal;

        if (this->minValDisplayedOnTop) {
            this->currentSetTopVal = newValTop;
            this->currentSetBottomVal = newValBottom;
        } else {
            this->currentSetTopVal = newValBottom;
            this->currentSetBottomVal = newValTop;
        }


        // The following code has been amended to snap in to specific values...
        // Commented out as it is causing problems when distances from position y to min and max are equal
        /*
        grabHandleYPositionTop = this->getYPositionForVal(this->currentSetTopVal)-circleRadius;
        grabHandleYPositionBottom = this->getYPositionForVal(this->currentSetBottomVal)-circleRadius;
        */

        // informObserversMinMaxValChanged();

        this->repaint();
    }

}

bool QRangeSlider::hitTestOnSliderBar(int x, int y) {

    int circleCenterMinValX = (int) this->centerTopVal.x();
    int circleCenterMinValY = (int) this->centerTopVal.y();


    /*
    (x,y+width/2);
    (x, y+height-width/2);
    */

    if (x>=circleCenterMinValX-this->circleRadius/2 &&
        x<=circleCenterMinValX+this->circleRadius/2) {
        return true;
    }
    return false;
}

bool QRangeSlider::hitTestTopGrabHandle(int x, int y) {

    int circleCenterTopValX = (int) this->centerTopVal.x();
    int circleCenterTopValY = (int) this->centerTopVal.y();

    if (x>=circleCenterTopValX-this->circleRadius/2 &&
        x<=circleCenterTopValX+this->circleRadius/2 &&
        y>=circleCenterTopValY-this->circleRadius/2 &&
        y<=circleCenterTopValY+this->circleRadius/2) {
        return true;
    }

    return false;
}

bool QRangeSlider::hitTestBottomGrabHandle(int x, int y) {

    int circleCenterBottomValX = (int) this->centerBottomVal.x();
    int circleCenterBottomValY = (int) this->centerBottomVal.y();

    if (x>=circleCenterBottomValX-this->circleRadius/2 &&
        x<=circleCenterBottomValX+this->circleRadius/2 &&
        y>=circleCenterBottomValY-this->circleRadius/2 &&
        y<=circleCenterBottomValY+this->circleRadius/2) {
        return true;
    }
    return false;
}

bool QRangeSlider::hitTestTopTextBox(int x, int y) {
    if (x>=this->textBoxLocationTopX &&
        x <= this->textBoxLocationTopX+this->textBoxWidthTopAndBottom &&
        y>=this->textBoxLocationTopY &&
        y<=this->textBoxLocationTopY+this->textBoxHeightTopAndBottom) {
        return true;
    }
    return false;
}

bool QRangeSlider::hitTestBottomTextBox(int x, int y) {
    if (x>=this->textBoxLocationBottomX &&
        x <= this->textBoxLocationBottomX+this->textBoxWidthTopAndBottom &&
        y>=this->textBoxLocationBottomY &&
        y<=this->textBoxLocationBottomY+this->textBoxHeightTopAndBottom) {
        return true;
    }
    return false;
}

void QRangeSlider::informObserversTextBoxFocused() {
    for (vector<QRangeSliderObserver*>::iterator it=observers.begin(); it!=observers.end(); ++it) {
        (*it)->qrangeSliderTextboxFocused(this);
    }
}

void QRangeSlider::informObserversMinMaxValChanged() {
    for (vector<QRangeSliderObserver*>::iterator it=observers.begin(); it!=observers.end(); ++it) {
        (*it)->qrangeSliderMinMaxValChanged(this);
    }
}

void QRangeSlider::mousePressEvent(QMouseEvent *event) {
    if (event->buttons() == Qt::LeftButton) {
        mousePressed = true;

//        deselectNeighbours();
        informObserversTextBoxFocused();

        // Check if handles are in range...
        int x = event->x();
        int y = event->y();

        if (!hitTestTopTextBox(x,y)) {
            lineEditTop.setVisible(false);
        }

        if (!hitTestBottomTextBox(x,y)) {
            lineEditBottom.setVisible(false);
        }


        if (hitTestTopGrabHandle(x,y)) {
            // Mouse is situated in minVal grab handle
                topValGrab = true;
                bottomValGrab = false;
        } else if (hitTestBottomGrabHandle(x,y)){
            // Mouse is situated in maxVal grab handle
                bottomValGrab = true;
                topValGrab = false;
        } else {
            topValGrab = false;
            bottomValGrab = false;
        }
    }
}


void QRangeSlider::mouseDoubleClickEvent(QMouseEvent * event) {
    if (event->buttons() == Qt::LeftButton) {
        int x = event->x();
        int y = event->y();


        if (hitTestTopTextBox(x,y)) {

            cout << "Double click top" << endl;

            lineEditTop.setVisible(true);
            lineEditTop.setFocus();
            lineEditTop.setText("");

            informObserversTextBoxFocused();


            //deselectNeighbours();

        } else
            if (hitTestBottomTextBox(x,y)) {

                cout << "Double click bottom" << endl;

                lineEditBottom.setVisible(true);
                lineEditBottom.setFocus();
                lineEditBottom.setText("");


                informObserversTextBoxFocused();

                // deselectNeighbours();
            } else {
                if (hitTestOnSliderBar(x,y)) {
                    minValDisplayedOnTop = !minValDisplayedOnTop;


                    int tmp = grabHandleYPositionTop;
                    grabHandleYPositionTop = this->height() - grabHandleYPositionBottom;
                    grabHandleYPositionBottom = this->height() - tmp;

                    centerTopVal=QPointF(this->xPositionSliderBar+this->sliderWidth/2, grabHandleYPositionTop); // Store position for Mouse Hit Test
                    centerBottomVal=QPointF(this->xPositionSliderBar+this->sliderWidth/2,grabHandleYPositionBottom);



                    this->update();


                    /*
                    WIDGET_DATA_TYPE tmp =  this->currentSetTopVal;
                    this->currentSetTopVal = this->currentSetBottomVal;
                    this->currentSetBottomVal = tmp;

                    tmp = minVal;
                    minVal = maxVal;
                    maxVal = tmp;
                    */

                    informObserversMinMaxValChanged();
                }
            }
    }
}

void QRangeSlider::mouseReleaseEvent(QMouseEvent *event) {
    mousePressed=false;

    bottomValGrab = false;
    topValGrab = false;

    resetButtonHighlights();

    informObserversMinMaxValChanged();

    update();
}

void QRangeSlider::hideTextboxesIfNecessary() {
    lineEditTop.setVisible(false);
    lineEditBottom.setVisible(false);
}

//TODO: remove
void QRangeSlider::enterEvent(QEvent * event) {
}

void QRangeSlider::leaveEvent(QEvent * event) {
    unsetHighlights();
    // hideTextboxesIfNecessary();
    update();
}

void QRangeSlider::unsetHighlights() {
    hightlightTopTextBox = false;
    hightlightBottomTextBox = false;
    resetButtonHighlights();
}

void QRangeSlider::resetButtonHighlights() {
    hightlightTopGrabHandle = false;
    hightlightBottomGrabHandle = false;   
}



void QRangeSlider::paintTextBoxWithValue(QPainter* painter, int x, int y, int width, int height, double val, int precision, bool drawHighlighted) {

    painter->setRenderHint(QPainter::Antialiasing);

    QPen penBoxHighlighted(QColor("#FF0000"), 2 , Qt::SolidLine, Qt::FlatCap, Qt::RoundJoin);
    QPen penBoxNotHighlighted(QColor("#BBBBBB"), 1 , Qt::SolidLine, Qt::FlatCap, Qt::RoundJoin);

    if (drawHighlighted) {
    // Draw Rectangles around text    
        painter->setPen(penBoxHighlighted);
    } else {
        painter->setPen(penBoxNotHighlighted);
    }

    QLinearGradient gradientBackgroundBox(QPointF(x,y+height), QPointF(x+width,y));
    gradientBackgroundBox.setColorAt(0, QColor("#FFFFFF"));
    gradientBackgroundBox.setColorAt(1, QColor("#333333"));


    QPainterPath pathBoxUp;
    pathBoxUp.addRoundedRect(x,y, width,height, 5, 5);

    painter->setBrush(gradientBackgroundBox);
    painter->drawPath(pathBoxUp);

    QBrush boxBrush(QColor(255,255,255,230), Qt::SolidPattern);

    painter->setBrush(boxBrush);

    painter->drawRoundedRect(QRect(x,y,width,height), 5, 5);


    QString fontName = FONT_NAME_TEXT_BOX;
    int fontSize = FONT_SIZE_TEXT_BOX;
    

    ostringstream os;
    os << fixed << std::setprecision(precision) << val;

    QString strVal = QString(os.str().c_str()); //QString.fromUtf8(test.c_str());
    QFont textFont(fontName, fontSize);

    // cout << strVal.toStdString() <<endl;

    QFontMetrics fm(textFont);
    int textWidth = fm.width(QString(strVal));


    painter->setFont(textFont);

    QPen penText(QColor("#000000"), 1 , Qt::SolidLine, Qt::FlatCap, Qt::RoundJoin);
    painter->setPen(penText);
    painter->drawText(QRectF(x+width/2-textWidth/2,y, width, height), strVal);

}

void QRangeSlider::drawGrabHandle(QPainter* painter, int x, int y, int circleRadius, bool drawHighlighted) {

    // Draw Shadows....
    int shadowDistanceX = 1;
    int shadowDistanceY = 2;
    int transparency = 130;
    int shadowColor = 0;

    QPainterPath circlePathShadow;
    circlePathShadow.addEllipse(QPointF(x+shadowDistanceX, y+shadowDistanceY), circleRadius, circleRadius);

    QPen penCircleShadow(QColor(shadowColor,shadowColor,shadowColor, transparency), 1 , Qt::SolidLine, Qt::FlatCap, Qt::RoundJoin);

    painter->setPen(penCircleShadow);
    QBrush brushShadow(QColor(shadowColor,shadowColor,shadowColor,transparency), Qt::SolidPattern);

    painter->setBrush(brushShadow);
    painter->drawPath(circlePathShadow);


    // Draw White Circles for Value

    QPainterPath circleMinPath;
    circleMinPath.addEllipse(QPointF(x, y), circleRadius, circleRadius);


    QPen penCircleHighlighted(QColor("#BBBBBB"), 1 , Qt::SolidLine, Qt::FlatCap, Qt::RoundJoin);
    QPen penCircleNotHighlighted(QColor("#BBBBBB"), 1 , Qt::SolidLine, Qt::FlatCap, Qt::RoundJoin);

    QBrush brushHighlighted(QColor("#EEEEEE"), Qt::SolidPattern);
    QBrush brushNotHighlighted(Qt::white, Qt::SolidPattern);

    if (drawHighlighted) {
        painter->setPen(penCircleHighlighted);
        painter->setBrush(brushHighlighted);
    } else {
        painter->setPen(penCircleNotHighlighted);
        painter->setBrush(brushNotHighlighted);
    }




    painter->drawPath(circleMinPath);


}

void QRangeSlider::drawSlideBar(QPainter* painter, int x, int y, int width, int height,QColor strokeColor, QColor gradientColor1, QColor gradientColor2) {

    int strokeWidth = 1;

    QPen pen(strokeColor, strokeWidth , Qt::SolidLine, Qt::FlatCap, Qt::RoundJoin);
    painter->setPen(pen);


    QLinearGradient gradientBackground(QPointF(0,0), QPointF(width,0));
    gradientBackground.setColorAt(0, gradientColor1);
    gradientBackground.setColorAt(1, gradientColor2);

    QPainterPath pathBackground;
    pathBackground.moveTo(x,y+width/2);
    pathBackground.lineTo(x, y+height-width/2);

    pathBackground.arcTo(x,y+height-width, width, width, 180.0,180.0 );
    pathBackground.lineTo(x+width, y+width/2);
    pathBackground.arcTo(x,y, width, width, 0,180.0 );

    pathBackground.closeSubpath();

    painter->setBrush(gradientBackground);
    painter->drawPath(pathBackground);

}

void QRangeSlider::on_topTextBox_EditingFinishedTriggered() {
    // Adjust top slider and update values...

    cout << "Adjust top slider" << endl;

    QString valStr = lineEditTop.text();
    bool conversionOk = false;

    // Used double here to achieve any possible data value...
    double valTop = valStr.toDouble(&conversionOk);

    if (!conversionOk) {
        cout << "Value entered is not in correct range" << endl;
        return;
    }

    // Now cast value to correct Data Type
    WIDGET_DATA_TYPE valTopCorrectDataType = (WIDGET_DATA_TYPE) valTop;

    // Do a range check of entered value...
    if (valTopCorrectDataType>maxVal) valTopCorrectDataType = maxVal;
    if (valTopCorrectDataType<minVal) valTopCorrectDataType = minVal;


    WIDGET_DATA_TYPE saveValTop    = this->currentSetTopVal;
    WIDGET_DATA_TYPE saveValBottom = this->currentSetBottomVal;


    if (minValDisplayedOnTop) {
        if (valTopCorrectDataType<=this->currentSetBottomVal) {
            this->currentSetTopVal = valTopCorrectDataType;
            grabHandleYPositionTop  = this->getYPositionForVal(this->currentSetTopVal)-circleRadius;
        } else {
            // Swap values...
            int savedGrabHandleYPositionBottom = grabHandleYPositionBottom;

            this->currentSetTopVal = saveValBottom;
            this->currentSetBottomVal = valTopCorrectDataType;

            grabHandleYPositionTop  = savedGrabHandleYPositionBottom;
            grabHandleYPositionBottom = this->getYPositionForVal(valTopCorrectDataType)-circleRadius;

            // QT Bugfix, handler is called twice when return is pressed...
            lineEditBottom.setText(QString::number(valTopCorrectDataType));
            lineEditTop.setText(QString::number(saveValBottom));

            centerTopVal=QPointF(this->xPositionSliderBar+this->sliderWidth/2, grabHandleYPositionTop); // Store position for Mouse Hit Test
            centerBottomVal=QPointF(this->xPositionSliderBar+this->sliderWidth/2,grabHandleYPositionBottom);

        }
    }


    this->deselect();
    informObserversMinMaxValChanged();

}

void QRangeSlider::on_bottomTextBox_EditingFinishedTriggered() {
    // Adjust bottom slider and update values...

    cout << "Adjust bottom slider" << endl;

    QString valStr = lineEditBottom.text();
    bool conversionOk = false;

    // Used double here to achieve any possible data value...
    double valBottom = valStr.toDouble(&conversionOk);

    if (!conversionOk) {
        cout << "Value entered is not in correct range" << endl;
        return;
    }


    // Now cast value to correct Data Type
    WIDGET_DATA_TYPE valBottomCorrectDataType = (WIDGET_DATA_TYPE) valBottom;

    // Do a range check of entered value...
    if (valBottomCorrectDataType>maxVal) valBottomCorrectDataType = maxVal;
    if (valBottomCorrectDataType<minVal) valBottomCorrectDataType = minVal;


    WIDGET_DATA_TYPE saveValTop    = this->currentSetTopVal;
    WIDGET_DATA_TYPE saveValBottom = this->currentSetBottomVal;


    if (minValDisplayedOnTop) {
        if (valBottomCorrectDataType>=this->currentSetTopVal) {
            this->currentSetBottomVal = valBottomCorrectDataType;
            grabHandleYPositionBottom  = this->getYPositionForVal(this->currentSetBottomVal)-circleRadius;
        } else {
            int savedGrabHandleYPositionTop = grabHandleYPositionTop;

            this->currentSetBottomVal = saveValTop;
            this->currentSetTopVal = valBottomCorrectDataType;

            grabHandleYPositionTop  = this->getYPositionForVal(valBottomCorrectDataType)-circleRadius;
            grabHandleYPositionBottom = savedGrabHandleYPositionTop;

            // QT Bugfix, handler is called twice when return is pressed...
            lineEditBottom.setText(QString::number(saveValTop));
            lineEditTop.setText(QString::number(valBottomCorrectDataType));

            centerTopVal=QPointF(this->xPositionSliderBar+this->sliderWidth/2, grabHandleYPositionTop); // Store position for Mouse Hit Test
            centerBottomVal=QPointF(this->xPositionSliderBar+this->sliderWidth/2,grabHandleYPositionBottom);

        }
    }


    this->deselect();
    informObserversMinMaxValChanged();

}


/*
void QRangeSlider::textBoxFocused(QRangeSlider* me) {

}
*/




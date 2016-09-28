#include "qrangeslider.h"
#include <qpainter.h>
#include <qwidget.h>
#include <QtWidgets>
#include <string>
#include <iostream>
// #include <boost/lexical_cast.hpp>
#include <sstream>
#include <iomanip>

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

}

void QRangeSlider::resizeEvent(QResizeEvent * event) {
    xPositionSliderBar = 15+20;

    textBoxLocationTopX = (xPositionSliderBar)-50/2;
    textBoxLocationTopY = slideBarStartY;

    textBoxLocationBottomX = (xPositionSliderBar)-50/2;
    int height = this->size().height();
    // height=200;
    textBoxLocationBottomY = slideBarStartY+height-60;

    lineEditTop.move(textBoxLocationTopX,textBoxLocationTopY);
    lineEditBottom.move(textBoxLocationBottomX,textBoxLocationBottomY);

}

WIDGET_DATA_TYPE QRangeSlider::getCurrentSetMinVal() {
    return this->currentSetMinVal;
}

WIDGET_DATA_TYPE QRangeSlider::getCurrentSetMaxVal() {
    return this->currentSetMaxVal;
}

void QRangeSlider::setMinVal(WIDGET_DATA_TYPE val) {
    this->minVal = val;
    this->currentSetMinVal = val;

}

void QRangeSlider::setMaxVal(WIDGET_DATA_TYPE val) {
    this->maxVal = val;
    this->currentSetMaxVal = val;
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


uint64_t QRangeSlider::getYPositionForVal(WIDGET_DATA_TYPE val) {

    WIDGET_DATA_TYPE range = (maxVal>minVal)? (maxVal - minVal) : minVal-maxVal;

    uint64_t retVal = this->slideBarStartY +circleRadius;

    if (range!=0) {
        // uint64_t ret1 = (val-minVal)/range * (WIDGET_DATA_TYPE)this->slideBarHeight;
        uint64_t ret2 = ((WIDGET_DATA_TYPE)(this->slideBarHeight))*(val-minVal)/range;

        retVal += ret2;


    }

    // if (retVal<this->slideBarStartY +circleRadius) retVal=this->slideBarStartY +circleRadius;
    // if (retVal>this->slideBarStartY+this->slideBarHeight) retVal = this->slideBarStartY+this->slideBarHeight;

   // if (retVal>1000) retVal=1000;

    return retVal;
}

void QRangeSlider::paintEvent(QPaintEvent *) {

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    paintSlider(&painter, this->xPositionSliderBar,0,10,this->size().height()-40, this->minVal, this->maxVal);

}

void QRangeSlider::mouseMoveEvent(QMouseEvent *event) {


    int mouseX = event->pos().x();
    int mouseY = event->pos().y();

    // Test if we need to highlight anything...
    this->hightlightTopGrabHandle = hitTestTopGrabHandle(mouseX, mouseY);
    this->hightlightBottomGrabHandle = hitTestBottomGrabHandle(mouseX, mouseY);

    this->hightlightTopTextBox = hitTestTopTextBox(mouseX, mouseY);
    this->hightlightBottomTextBox = hitTestBottomTextBox(mouseX, mouseY);


    double newVal = 0;

    if (minValGrab || maxValGrab) {
        // Calculate the new minimum value

        int y = event->pos().y()-this->slideBarStartY;

        // Limit Ranges
        if (y>this->slideBarHeight) y= this->slideBarHeight;
        if (y<0) y=0;

        newVal = (double)y*(maxVal-minVal)/(double) this->slideBarHeight+minVal;

    }


    if (minValGrab || maxValGrab) {        

        cout << "Top value is " << this->currentSetMaxVal << " Bottom Value is " << this->currentSetMaxVal << " " << newVal << endl;

        if (newVal <= this->currentSetMinVal) {
            this->minValGrab = true;
            this->maxValGrab = false;
        } else
        if (newVal >this->currentSetMaxVal) {
            this->maxValGrab = true;
            this->minValGrab = false;
        }

        if (minValGrab) {
            // Calculate the new minimum value
            this->currentSetMinVal = newVal;
        }
        else
        if (maxValGrab) {
            // Calculate the new maximum value
            this->currentSetMaxVal = newVal;
        }


        this->repaint();
    }

}

bool QRangeSlider::hitTestTopGrabHandle(int x, int y) {

    int circleCenterMinValX = (int) this->centerMinVal.x();
    int circleCenterMinValY = (int) this->centerMinVal.y();

    if (x>=circleCenterMinValX-this->circleRadius/2 &&
        x<=circleCenterMinValX+this->circleRadius/2 &&
        y>=circleCenterMinValY-this->circleRadius/2 &&
        y<=circleCenterMinValY+this->circleRadius/2) {
        return true;
    }

    return false;
}

bool QRangeSlider::hitTestBottomGrabHandle(int x, int y) {



    int circleCenterMaxValX = (int) this->centerMaxVal.x();
    int circleCenterMaxValY = (int) this->centerMaxVal.y();

    if (x>=circleCenterMaxValX-this->circleRadius/2 &&
        x<=circleCenterMaxValX+this->circleRadius/2 &&
        y>=circleCenterMaxValY-this->circleRadius/2 &&
        y<=circleCenterMaxValY+this->circleRadius/2) {
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


void QRangeSlider::mousePressEvent(QMouseEvent *event) {
    if (event->buttons() == Qt::LeftButton) {
        mousePressed = true;


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
            minValGrab = true;
            maxValGrab = false;
        } else if (hitTestBottomGrabHandle(x,y)){
            // Mouse is situated in maxVal grab handle
            maxValGrab = true;
            minValGrab = false;
        } else {
            minValGrab = false;
            maxValGrab = false;
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
            lineEditTop.focusWidget();

        } else
            if (hitTestBottomTextBox(x,y)) {

                cout << "Double click bottom" << endl;

                lineEditBottom.setVisible(true);
                lineEditBottom.focusWidget();

            }
    }
}

void QRangeSlider::mouseReleaseEvent(QMouseEvent *event) {
    mousePressed=false;

    maxValGrab = false;
    minValGrab = false;

    resetButtonHighlights();
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
    cout << "Leaving" << endl;
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

void QRangeSlider::paintSlider(QPainter* painter, int x, int y , int width, int height, int rangeStart, int rangeEnd) {


    // TODO: Those values should be made global and const
    /*
    this->textBoxLocationTopX=x-50/2;
    this->textBoxLocationTopY=y;

    this->textBoxLocationBottomX=x-50/2;
    this->textBoxLocationBottomY=y+height-20;
    */






    paintTextBoxWithValue(painter, textBoxLocationTopX,   textBoxLocationTopY,    textBoxWidthTopAndBottom, textBoxHeightTopAndBottom, this->currentSetMinVal, 2, this->hightlightTopTextBox);
    paintTextBoxWithValue(painter, textBoxLocationBottomX,textBoxLocationBottomY, textBoxWidthTopAndBottom, textBoxHeightTopAndBottom, this->currentSetMaxVal, 2, this->hightlightBottomTextBox);

    y+=50;
    height-=100;


    slideBarStartY = y;
    slideBarHeight = height;
    sliderWidth = width;

    // Draw unfilled slide area
    drawSlideBar(painter, x, y, width, height,QColor("#BBBBBB"), QColor("#D0D0D0"),QColor("#E1E1E1"));


    int grabHandleYPositionMin = this->getYPositionForVal(this->currentSetMinVal)-circleRadius;
    int grabHandleYPositionMax = this->getYPositionForVal(this->currentSetMaxVal)-circleRadius;

    // cout << grabHandleYPositionMin << " " << grabHandleYPositionMax << endl;


    // draw filled blueish slide area    
    drawSlideBar(painter, x, grabHandleYPositionMin, width, grabHandleYPositionMax-grabHandleYPositionMin,QColor("#BBBBBB"), QColor("#5C96C5"),QColor("#76ABD3"));


    // Draw Circles for Min Value
    centerMinVal=QPointF(x+width/2, grabHandleYPositionMin); // Store position for Mouse Hit Test
    drawGrabHandle(painter, x+width/2, grabHandleYPositionMin, circleRadius, this->hightlightTopGrabHandle);

    // Draw Circle for Max Value
    centerMaxVal=QPointF(x+width/2,grabHandleYPositionMax);
    drawGrabHandle(painter, x+width/2, grabHandleYPositionMax, circleRadius, hightlightBottomGrabHandle);


    /*
    QGraphicsDropShadowEffect* effect = new QGraphicsDropShadowEffect();
    effect->setBlurRadius(5);
    this->setGraphicsEffect(effect);
    */






}



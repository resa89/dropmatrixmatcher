#include "prolabel.h"
#include <QPainter>
#include <QMouseEvent>
#include <QDebug>
#include <QAction>
#include <QFileDialog>

ProLabel::ProLabel(QWidget *parent)
    : QLabel(parent)
{
    selectionStarted=false;

    QAction *saveAction=contextMenu.addAction("Save");

    connect(saveAction,SIGNAL(triggered()),this,SLOT(saveSlot()));
}

ProLabel::~ProLabel()
{

}

void ProLabel::paintEvent(QPaintEvent *e)
{
    QLabel::paintEvent(e);
    QPainter painter(this);
    painter.setPen(QPen(QBrush(QColor(0,0,0,180)),1,Qt::DashLine));
    painter.setBrush(QBrush(QColor(255,255,255,120)));

    painter.drawRect(selectionRect);
}

void ProLabel::mousePressEvent(QMouseEvent *e)
{
    if (e->button()==Qt::RightButton)
    {
        if (selectionRect.contains(e->pos())) contextMenu.exec(this->mapToGlobal(e->pos()));
    }
    else
    {
    selectionStarted=true;
    selectionRect.setTopLeft(e->pos());
    selectionRect.setBottomRight(e->pos());
    }

}

void ProLabel::mouseMoveEvent(QMouseEvent *e)
{
    if (selectionStarted)
    {
        selectionRect.setBottomRight(e->pos());
        repaint();
    }
}

void ProLabel::mouseReleaseEvent(QMouseEvent *e)
{
    selectionStarted=false;
}

void ProLabel::saveSlot()
{
    QString fileName = QFileDialog::getSaveFileName(this, QObject::tr("Save File"),
                                                    "./DropMatrixMatcherData",               //"~/"
                                                    QObject::tr("Images (*.jpg)"));
    this->pixmap()->copy(selectionRect).save(fileName);
}

QRect ProLabel::getQImageRect(float widthFactor, float heightFactor)
{
    QRect qimageSizedRect;

//      ////1.0
//    int x = (float)selectionRect.x() * widthFactor;
//    int y = (float)selectionRect.y() * heightFactor;
//    int w = (float)selectionRect.width() * widthFactor;
//    int h = (float)selectionRect.height() * heightFactor;

    ////<- new after 1.0: selection of rectangle also possible from right bottom to left top
    int x1 = this->selectionRect.left();
    int x2 = this->selectionRect.right();
    int y1 = this->selectionRect.top();
    int y2 = this->selectionRect.bottom();

    if (x1 > x2){
        QPoint firstPoint = QPoint(x1, y1);
        QPoint secondPoint = QPoint(x2, y2);

        this->selectionRect.setTopLeft(secondPoint);
        this->selectionRect.setBottomRight(firstPoint);
    }

    int x = (float)selectionRect.x() * widthFactor;
    int y = (float)selectionRect.y() * heightFactor;
    int w = (float)selectionRect.width() * widthFactor;
    int h = (float)selectionRect.height() * heightFactor;
    ////-> end

    qimageSizedRect.setX(x);
    qimageSizedRect.setY(y);
    qimageSizedRect.setWidth(w);
    qimageSizedRect.setHeight(h);

    return qimageSizedRect;
}


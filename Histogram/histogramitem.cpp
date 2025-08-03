#include "histogramitem.h"

#include <QGraphicsScene>

HistogramItem::HistogramItem() {}

void HistogramItem::updateData(QVector<uint> histValues)
{
    values = histValues;
    update();
}

void HistogramItem::setActiveRange(int l, int r)
{
    leftLimit = l;
    rightLimit = r;
    update();
}

QRectF HistogramItem::boundingRect() const
{
    return QRect (0, 0, 265, 220);
}

void HistogramItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *)
{
    painter->setRenderHint(QPainter::Antialiasing);
    painter->setBrush(QColor("#171a23"));
    painter->setPen(Qt::NoPen);
    painter->drawRect(boundingRect());

    int r = 0;

    for (auto &val : values) {
        int d;
        if (val == 0){d = 0;}
        if (val != 0){d = 15*log(val);}
        if ((r < rightLimit) && (r > leftLimit)) painter->setBrush(QColor("#d1d1d3"));
        else painter->setBrush(QColor("#979797"));
        if (d < boundingRect().height())
        {
            painter->drawRect(r, boundingRect().height(), 1, -d);
        }
        else
        {
            d = boundingRect().height();
            painter->drawRect(r, boundingRect().height(), 1, -d);
        }
        r +=1;
    }
}

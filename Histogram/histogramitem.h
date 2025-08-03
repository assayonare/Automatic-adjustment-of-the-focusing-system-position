#ifndef HISTOGRAMITEM_H
#define HISTOGRAMITEM_H

#include <QGraphicsObject>
#include <QObject>
#include <QPainter>

class HistogramItem : public QGraphicsObject
{
    Q_OBJECT
public:
    HistogramItem();

    void updateData(QVector<uint> histValues);
    void setActiveRange(int l, int r);

private:
    QRectF boundingRect() const;
    void paint (QPainter* painter, const QStyleOptionGraphicsItem*, QWidget*);

    QVector<uint> values = {};
    int leftLimit;
    int rightLimit;
};

#endif // HISTOGRAMITEM_H

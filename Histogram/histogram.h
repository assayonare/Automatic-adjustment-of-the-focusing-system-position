#ifndef HISTOGRAM_H
#define HISTOGRAM_H

#include <QObject>

#include "plot.h"

class Histogram : public QObject
{
    Q_OBJECT
public:
    explicit Histogram(QObject *parent = nullptr);

    void getHistogram(ushort* RAWData, int width, int height);
    Plot *getPointerToPlot();
    ushort *getData();
    uint getWidth();
    uint getHeight();


private:
    QVector<uint> histData;
    ushort *data = nullptr;
    ushort *filteredData = nullptr;
    uint width;
    uint height;

    int xR = 256;
    int xL = 0;

    qreal gamma = 1.0;

    Plot *plot = nullptr;

    QLabel *label = nullptr;


private slots:
    void changeContrast(int x_left, int x_right);

signals:

    void histogramDone(ushort* data, int width, int height);
};

#endif // HISTOGRAM_H

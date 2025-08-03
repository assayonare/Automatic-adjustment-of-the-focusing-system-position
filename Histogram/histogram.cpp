#include "histogram.h"

#include "Tiff/service_functions.h"

Histogram::Histogram(QObject *parent)
    : QObject{parent}
{}

void Histogram::getHistogram(ushort *RAWData, int w, int h)
{
    if (data) delete[] data;
    if (filteredData) delete[] filteredData;

    histData.resize(255);
    histData.fill(0);

    width = w;
    height = h;

    data = new ushort[w * h];
    filteredData = new ushort[w * h];

    memcpy(data, RAWData, w * h * 2);
    memcpy(filteredData, RAWData, w * h * 2);

    for (int i = 0; i < w; i++) {
        for (int j = 0; j < h; j++) {
            int index = 255 * filteredData[i * w + j] / 65535;
            histData[index] ++;
        }
    }

    plot = new Plot();
    connect(plot, &Plot::makeContrast, this, &Histogram::changeContrast);
    emit histogramDone(filteredData, width, height);

    plot->setData(histData);
}

Plot *Histogram::getPointerToPlot()
{
    return plot;
}

ushort *Histogram::getData()
{
    return filteredData;
}

uint Histogram::getWidth()
{
    return width;
}

uint Histogram::getHeight()
{
    return height;
}

void Histogram::changeContrast(int x_left, int x_right)
{
    x_left = round((double)x_left * 65535.0 / 255.0);
    x_right = round((double)x_right * 65535.0 / 255.0);

    for (int i = 0; i < width; i++) {
        for (int j = 0; j < width; j++) {
            int pixel = data[i * width + j];

            if ((x_right - x_left)!= 0)
                pixel = round(65535.0*((double)pixel - (double)x_left)/((double)x_right - (double)x_left));
            else
                pixel = round(65535.0*((double)pixel - (double)x_left)/((double)x_right - (double)x_left + 0.00001));

            pixel = gamma * pixel;

            pixel = qBound(0, pixel, 65535);
            filteredData[i * width + j] = pixel;
        }
    }
    emit histogramDone(filteredData, width, height);
}

#ifndef PLOT_H
#define PLOT_H

#include <QGraphicsView>

#include "add_lines.h"
#include "add_lines_2.h"
#include "histogramitem.h"

class Plot : public QGraphicsView
{
    Q_OBJECT
public:
    Plot();
    void setData(QVector<uint> histData);

private:
    QGraphicsScene *scene;
    HistogramItem *histogramItem = nullptr;
    Add_Lines *left_line = nullptr;
    Add_Lines_2 *right_line = nullptr;

    qreal gCoeff;

private slots:
    void linesValuesChanged();

signals:
    void makeContrast(int x_left, int x_right);
};

#endif // PLOT_H

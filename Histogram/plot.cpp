#include "plot.h"

Plot::Plot() {

    QLine l_line (5, 0, 5, 220);
    QLine r_line (5, 0, 5, 220);

    histogramItem = new HistogramItem();

    left_line = new Add_Lines();
    left_line->line = l_line;

    right_line = new Add_Lines_2();
    right_line->line = r_line;
    right_line->setPos(256, 0);

    connect(left_line, &Add_Lines::posChanged, right_line, &Add_Lines_2::setLimit);
    connect(right_line, &Add_Lines_2::posChanged, left_line, &Add_Lines::setLimit);
    connect(left_line, &Add_Lines::l_clicked, this, &Plot::linesValuesChanged);
    connect(right_line, &Add_Lines_2::r_clicked, this, &Plot::linesValuesChanged);

    scene = new QGraphicsScene();
    setScene(scene);
    scene->setSceneRect(QRect(0, 0, 261, 220));
    scene->addItem(histogramItem);
    scene->addItem(left_line);
    scene->addItem(right_line);

}

void Plot::setData(QVector<uint> histData)
{
    histogramItem->updateData(histData);
}

void Plot::linesValuesChanged()
{
    emit makeContrast(left_line->x_left, right_line->x_right);
    histogramItem->setActiveRange(left_line->x_left + 3, right_line->x_right + 3);
}

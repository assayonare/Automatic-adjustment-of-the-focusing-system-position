#ifndef ADD_LINES_2_H
#define ADD_LINES_2_H
#include <QtWidgets>

class Add_Lines_2 : public QObject, public QGraphicsItem
{
    Q_OBJECT
public:

    Add_Lines_2();

    QLine line;
    int x_right;

public slots:
    void setLimit(int limit);

private:

    QColor lines_color;
    QPointF left_line_position, right_line_position;

    int oldY = 0;

    QRectF boundingRect() const;
    void paint (QPainter* painter, const QStyleOptionGraphicsItem*, QWidget*);
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
    void mousePressEvent(QGraphicsSceneMouseEvent *event);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
    void hoverEnterEvent(QGraphicsSceneHoverEvent *event);
    void hoverLeaveEvent(QGraphicsSceneHoverEvent *event);

    int leftLimit = 0;

signals:

    void posChanged(int pos);

    void r_clicked();
};

#endif // ADD_LINES_2_H

#ifndef ADD_LINES_H
#define ADD_LINES_H
#include <QtWidgets>

class Add_Lines : public QObject, public QGraphicsItem
{
    Q_OBJECT
public:

    Add_Lines();
    QLine line;
    int x_left;

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

    int rightLimit = 256;

signals:

    void l_clicked();
    void posChanged(int pos);

};

#endif // ADD_LINES_H

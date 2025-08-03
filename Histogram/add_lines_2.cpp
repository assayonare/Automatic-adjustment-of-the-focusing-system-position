#include "add_lines_2.h"

Add_Lines_2::Add_Lines_2()
{
    x_right = 256;
    setAcceptHoverEvents(true);
}

void Add_Lines_2::setLimit(int limit)
{
    leftLimit = limit;
}

QRectF Add_Lines_2::boundingRect() const
{
    return QRect (0, 0, 10, 220);
}

void Add_Lines_2::paint (QPainter *painter, const QStyleOptionGraphicsItem*, QWidget*)
{
    painter->setRenderHint(QPainter::Antialiasing);

    //Цвет правой линии
    lines_color = QColor("#5487ff");

    QPen pen(lines_color);
    pen.setWidth(3);
    painter->setPen(pen);
    painter->drawLine(line);
}


void Add_Lines_2::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    QPointF point = mapToScene(event->pos());

    x_right = (std::max(leftLimit + 10.0, std::min(point.x(), 256.0)));
    setPos(x_right, 0);

    update();
    emit posChanged(x_right);
    emit r_clicked();
}

void Add_Lines_2::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    setCursor(QCursor(Qt::ClosedHandCursor));
}

void Add_Lines_2::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    setCursor(QCursor(Qt::SizeHorCursor));
}

void Add_Lines_2::hoverEnterEvent(QGraphicsSceneHoverEvent *event)
{
    setCursor(QCursor(Qt::SizeHorCursor));
}

void Add_Lines_2::hoverLeaveEvent(QGraphicsSceneHoverEvent *event)
{
    setCursor(QCursor(Qt::ArrowCursor));
}


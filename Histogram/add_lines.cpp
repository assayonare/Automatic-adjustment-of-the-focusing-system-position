#include "add_lines.h"

Add_Lines::Add_Lines()
{
    x_left = 0;
    setAcceptHoverEvents(true);
}

void Add_Lines::setLimit(int limit)
{
    rightLimit = limit;
}

QRectF Add_Lines::boundingRect() const
{
    return QRect (0, 0, 7, 220);
}

void Add_Lines::paint (QPainter *painter, const QStyleOptionGraphicsItem*, QWidget*)
{
    painter->setRenderHint(QPainter::Antialiasing);

    //Цвет левой линии
    lines_color = QColor("#5487ff");

    QPen pen(lines_color);
    pen.setWidth(3);
    painter->setPen(pen);
    painter->drawLine(line);
}


void Add_Lines::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    QPointF point = mapToScene(event->pos());

    x_left = (std::max(0.0, std::min(point.x(), rightLimit - 10.0)));
    setPos(x_left, 0);

    update();
    emit posChanged(x_left);
    emit l_clicked();
}

void Add_Lines::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    setCursor(QCursor(Qt::ClosedHandCursor));
}

void Add_Lines::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    setCursor(QCursor(Qt::SizeHorCursor));
}

void Add_Lines::hoverEnterEvent(QGraphicsSceneHoverEvent *event)
{
    setCursor(QCursor(Qt::SizeHorCursor));
}

void Add_Lines::hoverLeaveEvent(QGraphicsSceneHoverEvent *event)
{
    setCursor(QCursor(Qt::ArrowCursor));
}


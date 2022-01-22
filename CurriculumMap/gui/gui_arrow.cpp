#include "gui_arrow.h"

#include <qmath.h>
#include <QPen>
#include <QPainter>
#include <QLineEdit>
#include <QString>

#include <iostream>

Arrow::Arrow(QWidget* s, QWidget* e, R_type t, QGraphicsItem *parent) :
    QGraphicsLineItem(parent)
{
    set_link(s, e);
    set_type(t);
    //setFlag(QGraphicsLineItem::ItemIsSelectable, true);
    set_start_end();
}

QRectF Arrow::boundingRect() const
{
    qreal delta_x = abs(line().p2().x() - line().p1().x());
    qreal delta_y = abs(line().p2().y() - line().p1().y());

// delta_y ^ 2 = 1.28 * (2 * delta_y) * extra_x
    
    // 20 is the size of arrow head
    // consider that it might be a parabola, so add width of parabola to extra too
    qreal extra = 20 + pen().width() + delta_y / 2.56;

    qreal xl = std::min(line().p2().x(), line().p1().x());
    qreal yu = std::min(line().p2().y(), line().p1().y());

    return QRectF(xl, yu, delta_x, delta_y)
            .normalized()
            .adjusted(-extra, -extra, extra, extra);
}

void Arrow::set_link(QWidget* s, QWidget* e)
{
    start_widget = s;
    end_widget = e;
}
/*
link the midpoint of right edge of left node, with the midpoint of left edge of the right node
*/
void Arrow::set_start_end()
{
    if (is_empty()) return;
    int xl = start_widget->x(), yl = start_widget->y(), wl = start_widget->width(), hl = start_widget->height();
    int xr = end_widget->x(), yr = end_widget->y(), wr = end_widget->width(), hr = end_widget->height();
    if (xl > xr)
    {
        sp.setX(xl);
        sp.setY(yl + hl / 2);
        ep.setX(xr + wr);
        ep.setY(yr + hr / 2);
    }
    else
    {
        sp.setX(xl + wl);
        sp.setY(yl + hl / 2);
        ep.setX(xr);
        ep.setY(yr + hr / 2);
    }
    setLine(sp.x(), sp.y(), ep.x(), ep.y());
}

void Arrow::set_color_alpha(const qreal &alpha)
{
    // 0 for completely transparent, 1 for not transparent at all
    arrow_color.setAlphaF(alpha);
}
/*
set different style for different type
*/
void Arrow::set_type(const Arrow::R_type t)
{
    r_type = t;
    switch (t)
    {
    case R_type::AND :
        arrow_color = Qt::darkBlue;
        pen_width = 3;
        pen_style = Qt::PenStyle::SolidLine;
        break;
    case R_type::OR :
        arrow_color = Qt::darkGray;
        pen_width = 3;
        pen_style = Qt::PenStyle::SolidLine;
        break;
    case R_type::EX :
        arrow_color = Qt::red;
        pen_width = 2;
        pen_style = Qt::PenStyle::DotLine;
        break;
    case R_type::CO :
        arrow_color = Qt::darkGreen;
        pen_width = 3;
        pen_style = Qt::PenStyle::SolidLine;
        break;
    }
    set_color_alpha(1);
}

/*
arrow head of EX is a cross
the direction the head is pointing at is specified by slope, which has been calculated when drawing arrow line
*/
void Arrow::set_arrow_head_ex(const QPointF & head_main, const double & angle)
{
    qreal arrowSize = 8;
    QPointF head_mid = head_main;

    QPointF head1 = head_mid - QPointF(cos(angle + M_PI / 4) * arrowSize,
                                       sin(angle + M_PI / 4) * arrowSize);
    QPointF head2 = head_mid - QPointF(cos(angle - M_PI / 4) * arrowSize,
                                       sin(angle - M_PI / 4) * arrowSize);
    QPointF head3 = head_mid + QPointF(cos(angle + M_PI / 4) * arrowSize,
                                       sin(angle + M_PI / 4) * arrowSize);
    QPointF head4 = head_mid + QPointF(cos(angle - M_PI / 4) * arrowSize,
                                       sin(angle - M_PI / 4) * arrowSize);
    arrow_head.clear();
    arrow_head << head1 << head_mid << head2 << head_mid << head3 << head_mid << head4 << head_mid;
}
/*
normal arrow head for other types
*/
void Arrow::set_arrow_head_triangle(const QPointF & head_main, const double & angle)
{
    qreal arrowSize = 20;
    QPointF head1 = head_main;

    QPointF head2 = head1 - QPointF(cos(angle + M_PI / 12) * arrowSize,
                                    sin(angle + M_PI / 12) * arrowSize);
    QPointF head3 = head1 - QPointF(cos(angle - M_PI / 12) * arrowSize,
                                    sin(angle - M_PI / 12) * arrowSize);
    arrow_head.clear();
    arrow_head << head1 << head2 << head3;
}

/*
use parabola formula
*/
void Arrow::draw_quad_line(QPainter* & painter, QPointF & head_main, double & angle)
{
    double x = start_widget->x();
    double yu = line().p1().y();
    double yd = line().p2().y();

//y^2 = px
//2 y dy = p dx
//dy / dx = p / (2y)
    if (yu > yd) std::swap(yu, yd);
    double p = 1.28 * (yd - yu);

    double y_relative = (yd - yu) / 2;
//yd - y_m == y_origin - yu == y_relative - 0
    double y_origin = yd - y_relative;
    double x_relative = y_relative * y_relative / p;
//x_origin - 0 == x - x_relative
    double x_origin = x - x_relative;

    QVector<QPointF> points;
    double y_step = (r_type == Arrow::R_type::EX) ? 3 : 1;
    for (double y = -y_relative; y <= y_relative; y += y_step)
    {
        double x = y * y / p;
        points.push_back(QPointF(x + x_origin, y + y_origin));
    }
    painter->drawLines(points);
    yu = line().p1().y();
    yd = line().p2().y();
    qreal y_head = yu + (yd - yu) * 7 / 8;
// y_h - y_h_r = y_origin - 0
    qreal y_head_relative = y_head - y_origin;
    qreal x_head_relative = y_head_relative * y_head_relative / p;
    angle = std::atan2(p / (2 * y_head_relative), 1.0);
    head_main = QPointF(x_origin + x_head_relative, y_origin + y_head_relative);
}

void Arrow::paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *)
{
    painter->setBrush(arrow_color);
    painter->setRenderHint(QPainter::Antialiasing);

    QPointF head_main = line().p1() / 8 + line().p2() / 8 * 7;
    double angle = std::atan2(line().dy(), line().dx());

    // Generate & Paint Line
    painter->setPen(QPen(arrow_color, pen_width, pen_style, Qt::RoundCap, Qt::RoundJoin));
    if (start_widget->x() == end_widget->x())
        draw_quad_line(painter, head_main, angle);
    else
        painter->drawLine(line());
    // Generate Arrow Head
    if (r_type == Arrow::R_type::EX)
        this->set_arrow_head_ex(head_main, angle);
    else
        this->set_arrow_head_triangle(head_main, angle);
    // Paint Arrow Head
    painter->setPen(QPen(arrow_color, pen_width, Qt::PenStyle::SolidLine, Qt::RoundCap, Qt::RoundJoin));
    painter->drawPolygon(arrow_head);
}


/* arrow.h
different types of arrows connecting nodes (including courses & logical operators)

The process of drawing arrows consists of two step :
1. draw the arrow line
2. draw the arrow head

In most cases, arrow line is simply a straight line starts from the midpointthe right border of left nodes, and ends at the midpoint of the left border of right node.
When two nodes are in the same column (have same x value), then use the parabola formula to link the midpoints of the left borders of two nodes

The shape of arrow head is decided by the type (cross for EX, triangle for others). 
To avoid overlap of "in" arrows near nodes, the position of it is set to the 7/8 of the line (7/8 y value if parabola).
And the angle of the direction of the arrow is the same as the tangent line at that point (derivative if parabola)

The color and the style is decided by the type too
*/

#ifndef GUI_ARROW_H
#define GUI_ARROW_H

#include <QGraphicsLineItem>
#include <QPointF>
#include <QGraphicsView>

class Arrow : public QGraphicsLineItem
{
public:
    enum class R_type {AND, OR, EX, CO};

    //Arrow(QPointF sp, QPointF ep, R_type t = R_type::AND, QGraphicsItem *parent = nullptr);
    Arrow(QWidget* s = nullptr, QWidget* e = nullptr, R_type t = R_type::AND, QGraphicsItem *parent = nullptr);
    QRectF boundingRect() const override;

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = nullptr) override;

    void set_link(QWidget* s, QWidget* e);
    bool is_empty() { return start_widget == nullptr; }



    //void update_position();
    void set_start_end();
    void set_type(const Arrow::R_type t);
    void set_color_alpha(const qreal& alpha);
    void set_arrow_head_ex(const QPointF & head_main, const double & angle);
    void set_arrow_head_triangle(const QPointF & head_main, const double & angle);
    void draw_quad_line(QPainter* & painter, QPointF & head_main, qreal & slope);

public slots :
    /*
     *update by start&end PointX
     */


private :
    enum R_type r_type;
    QColor arrow_color;
    qreal pen_width;
    Qt::PenStyle pen_style;

    QPolygonF arrow_head;
    QPointF sp, ep;
    QWidget *start_widget = nullptr, *end_widget = nullptr;
};

#endif // GUI_ARROW_H

/*
 * graph : from left to right
 * start PointX : the midpoint of right edge
 * end PointX : the midpoint of left edge
 *
 *
 *
 */

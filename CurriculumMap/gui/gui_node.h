/*gui_node.h
Base Class : Node, inherited from QPushButton, in order to deal with clicked/hover/context menu conviniently.

The geometry attributes (position, size) can be set using member functions in QPushButton (QWidget)

name is the common property of its derived class

hover event : fade(opacity) other arrows that do NOT directly connected with this node, in order to underline those directly connected with it.
To judge arrowsm, we need to store the from arrows and to arrows in this class, which are also the common part of its derived classes. And we use vector to store them


*/
#ifndef GUI_NODE_H
#define GUI_NODE_H

#include <QPushButton>
#include <QMenu>
#include <QAction>
#include <QMouseEvent>
#include <QTextEdit>
#include <QLabel>
/*Qt STL*/
#include <vector>

using namespace std;

class Arrow;

class Node : public QPushButton
{
    Q_OBJECT

public:
    explicit Node(QWidget *parent = 0);
    ~Node();

    QString get_name() const {return name;}

    void set_name(const QString& s);
    void set_info_box(const QString& s);

    void add_from(Arrow* arrow, Node* node);
    void add_to(Arrow* arrow, Node* node);

    void enterEvent(QEnterEvent *event) override;
    void leaveEvent(QEvent *event) override;

private:
    QString name;

    vector<pair<Arrow*, Node*> > from_list;
    vector<pair<Arrow*, Node*> > to_list;
};

#endif // GUI_NODE_H

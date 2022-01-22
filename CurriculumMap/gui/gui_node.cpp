#include "gui_arrow.h"
#include "gui_node.h"
#include "global.h"

Node::Node(QWidget *parent) :
    QPushButton(parent)
{
    from_list.clear();
    to_list.clear();
}

Node::~Node()
{

}

void Node::set_name(const QString& s)
{
    this->name = s;
    this->setText(s);
}

void Node::set_info_box(const QString& s)
{
    Global::info_box->setText(s);
}

void Node::add_from(Arrow* arrow, Node* node)
{
    from_list.push_back(make_pair(arrow, node));
}
void Node::add_to(Arrow* arrow, Node* node)
{
    to_list.push_back(make_pair(arrow, node));
}

/*
hover event.
first lower the alpha of all arrows,
then traverse all the arrows directly connected with this node, change the alpha of them back to 1
*/
void Node::enterEvent(QEnterEvent*)
{
    for (unsigned int i = 0; i < Global::list_arrow.size(); ++i)
    {
        Global::list_arrow[i]->set_color_alpha(0.1);
        Global::list_arrow[i]->update();
    }
    for (unsigned int i = 0; i < from_list.size(); ++ i)
    {
        (from_list[i].first)->set_color_alpha(1.0);
        (from_list[i].first)->update();
    }
    for (unsigned int i = 0; i < to_list.size(); ++ i)
    {
        (to_list[i].first)->set_color_alpha(1.0);
        (to_list[i].first)->update();
    }

}
void Node::leaveEvent(QEvent*)
{
    for (unsigned int i = 0; i < Global::list_arrow.size(); ++i)
    {
        Global::list_arrow[i]->set_color_alpha(1);
        Global::list_arrow[i]->update();
    }
}

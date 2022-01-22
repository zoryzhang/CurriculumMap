#include "gui_rnode.h"
#include "global.h"

/*
pass the pointer to logic P as parameter, in order to improve readability.
*/
RNode::RNode(LogicP* logicp, const int& x, const int& y, QWidget *parent) : Node(parent)
{
    this->setFixedSize( Global::button_height , Global::button_height );
    this->set_name(logicp->get_name());
    this->r_type = (this->get_name() == "OR") ? (R_type::OR) : (R_type::AND);
    this->set_style(this->r_type);
    this->move(x, y);
}


void RNode::set_style(const R_type &t)
{
    switch (t)
    {
    case R_type::AND :
        setText("AND");
        break;
    case R_type::OR :
        setText("OR");
        break;
    }
}


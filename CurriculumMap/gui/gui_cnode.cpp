#include "gui_cnode.h"
#include "global.h"
#include <iostream>

#include <QDebug>


using namespace std;

CNode::CNode(Course* course, int x, int y, QMenu* menu, QWidget *parent)
    : Node(parent), menu(menu), to_point(course)
{
    this->set_name(course->get_name());
    this->setFixedSize(Global::button_width, Global::button_height);
    this->move(x, y);

    //setToolTip("click to show highlight options"); this will cover other courses
    connect(this, SIGNAL (clicked()), this, SLOT (is_clicked()));

    //FOR DIFFERENT COLOR-SILIY METHOD
    QString str= this->get_name().split(' ').first();
    this->setStyleSheet(Global::set_style(str, course->get_enrolled(), course->get_searched(), course->get_r_searched()));
}

/*
menu interaction
*/
void CNode::contextMenuEvent(QContextMenuEvent  *event)
{
    Global::selected_point = this->to_point;

    PointX *p=Global::dictionary[this->get_name()];
    QList<QAction *> actions = menu->actions();
    for (auto i = actions.begin(); i != actions.end(); ++ i)
    {
        if ((*i)->text() == "search")
            (*i)->setChecked(p->get_searched());
        else if ((*i)->text() == "enrolled")
            (*i)->setChecked(p->get_enrolled());
        else if ((*i)->text() == "Reverse search")
            (*i)->setChecked(p->get_r_searched());
    }
    menu->exec(event->globalPos());
}

/*
call the member functions of course object to get all detailed information.
This information is only stored in Course class, in order to save memory.
*/
void CNode::is_clicked()
{
    Global::selected_point = this->to_point;
    Course* course=dynamic_cast<Course*>(to_point);
    this->set_info_box(this->get_name() + "\n" +
                       course->get_fullname() + "\n" +
                       course->get_credit() + "\npre-requisite: " +
                       course->get_fullpre() + "\nco-requisite: " +
                       course->get_fullco() + "\nexclusion: " +
                       course->get_fullex() + "\n\n" +
                       course->get_description());
}

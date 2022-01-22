/*gui_rnode.h

Derived class of Node : RNode

set the style according to whether it represents OR or AND.

*/

#ifndef GUI_RNODE_H
#define GUI_RNODE_H

#include "gui_node.h"
#include "global.h"

class RNode : public Node
{
    Q_OBJECT

public:

    enum class R_type {AND, OR};

    RNode(LogicP* logicp, const int& x, const int& y, QWidget *parent = 0);
    void set_style(const R_type& t);


private:
    R_type r_type;
};

#endif // GUI_RNODE_H



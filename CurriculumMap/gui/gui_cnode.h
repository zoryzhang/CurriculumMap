/*gui_cnode.h


context menu (right click) is implemented here, since only courses have these operations :
1. enroll, set it as enrolled, then the pre requisites courses would (both direct and indirect) would be hidden, in order to reduce the complexity of the graph and provide
    key information for user
2. search, display the pre requisites of the course, except those "before" the courses that marked as "enroll"
3. reversely search, display the courses need this one as pre requisite
These 3 operations match 3 checkable QAction's in that menu, note that these QActions and QMenu are shared with all courses, in order to save memory
And these 3 operations havee theirs own highlight methods, i.e., these highlight are INDEPENDENT, won't cover each other, can exist at the same time

Courses have other detailed information. Since we ignore the less important information, they will be displayed in the info box in the sidebar

Each CNode object corresponds to unique Course object (in CMD), the transmission of information is realized by calling member functions of Course class

*/

#ifndef GUI_CNODE_H
#define GUI_CNODE_H

#include <QGraphicsSceneContextMenuEvent>
/* my headers*/
#include "gui_node.h"
#include "global.h"

class CNode : public Node
{
    Q_OBJECT

public:

    CNode(Course* course, int x = 0, int y = 0, QMenu* menu = nullptr, QWidget *parent = 0);

public slots:
    void is_clicked();

private:
    QMenu *menu;
    PointX* to_point;

protected:
    void contextMenuEvent(QContextMenuEvent *event) override;
};

#endif // GUI_CNODE_H

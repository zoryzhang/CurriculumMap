/*gui_cscene.h

CScene class is inherited from QGraphicsScene.

It plays an important role as a connector of each part of this project. The main function is to :

1. provide support for interactions of items in it, like : 
1.a) judge which items is clicked
1.b) generate & store the shared menu, and enroll the pointer to it to CNode objects
1.c) implement menu event and update graph(see part 3)
1.d) maintain the list (showing history operations on course) in sidebar

2. transport (read) information from CMD part to GUI part
2.a) initialize Node objects
2.b) generate Arrow objects, bind the 2 nodes for each arrow

3. update the graph
3.a) when an course is clicked, according to the change of its flags(3 operations), call CMD to re-generate the graph
3.b) call part 2 to repaint the graph(GUI)
*/

#ifndef GUI_CSCENE_H
#define GUI_CSCENE_H

#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>
#include <QGraphicsProxyWidget>

/*my headers*/
#include "gui_arrow.h"
#include "gui_rnode.h"
#include "gui_cnode.h"
#include "global.h"

/*Qt STL*/
#include <map>

using namespace std;

class CScene : public QGraphicsScene
{
    Q_OBJECT

public:
    explicit CScene(QObject *parent = nullptr);
    ~CScene();

    void create_menu();

    void delete_all();
    void set_node_from_to(Node* & sp, Node* & ep, Arrow* & arrow);

    void read_from_cmd();
    void generate_graph();
    void Repaint_Map();

    QMenu* get_menu() const { return this->menu; }

private slots:
    /* Menu Part */
    void change_enrolled_status(const bool& enrolled);
    void change_searched_status(const bool& searched);
    void change_rsearched_status(const bool& rsearched);

public slots:
    void itemClicked(QListWidgetItem *item);

private:
    // Rightclick Menu
    QAction *act_enroll;
    QAction *act_search;
    QAction *act_rsearch;
    QMenu *menu;

    map<PointX*, Node*> map_point_to_node;
    vector<Node*> list_node;
    vector<QGraphicsRectItem*> list_rect;

    qreal x_value,y_value;
};

#endif // GUI_CSCENE_H

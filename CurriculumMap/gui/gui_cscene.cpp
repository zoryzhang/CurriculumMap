#include "gui_cscene.h"
#include "global.h"
#include <iostream>
#include <QMessageBox>

#ifdef Q_OS_MACOS
#include <ApplicationServices/ApplicationServices.h>
#endif

CScene::CScene(QObject *parent) : QGraphicsScene(parent), x_value(0), y_value(0)
{
    /* Set Menu */
    this->create_menu();
    Global::selected_point = nullptr;
}

/*
set up the Menu and its Actions.
They are shared by all cnode, so that we decide to store them in cscene
*/
void CScene::create_menu()
{
    this->menu = new QMenu;

    this->act_enroll = menu->addAction("enrolled");
    this->act_enroll->setCheckable(true);
    connect(act_enroll, SIGNAL (toggled(bool)), this, SLOT (change_enrolled_status(bool)));

    this->act_search = menu->addAction("search");
    this->act_search->setCheckable(true);
    connect(act_search, SIGNAL (toggled(bool)), this, SLOT (change_searched_status(bool)));

    this->act_rsearch = menu->addAction("Reverse search");
    this->act_rsearch->setCheckable(true);
    connect(act_rsearch, SIGNAL (toggled(bool)), this, SLOT (change_rsearched_status(bool)));
    menu->move(0, 0);
}

CScene::~CScene()
{
    this->delete_all();
// NOTE!! remember to delete menu
    delete menu;
}

// NOTE!! : won't delete menu. will delete arrow & button
void CScene::delete_all()
{
    Global::list_arrow.clear();
    list_node.clear();
    list_rect.clear();
    map_point_to_node.clear();
    QGraphicsScene::clear();//delete all items&wedgit in scene, no need to delete manully
}

void CScene::set_node_from_to(Node* & sp, Node* & ep, Arrow* & arrow)
{
    sp->add_to(arrow, ep);
    ep->add_from(arrow, sp);
}

/*
Read graph genereated by CMD, and contruct Nodes and Arrows accordingly
*/
void CScene::read_from_cmd()
{
    this->delete_all();

    for (unsigned int i = 0; i < Global::Course_list.size(); ++ i)
    {
        Course* now = Global::Course_list[i].first;
        int x = Global::Course_list[i].second.first;
        int y = Global::Course_list[i].second.second;
        Node* node = new CNode {now, x, y, this->menu};
        map_point_to_node[now] = node;
        list_node.push_back(node);
    }

    //set scene size
    setSceneRect(QRectF(-1000, -1000, Global::Graph_width+2000, Global::Graph_height+2000));

    for (unsigned int i = 0; i < Global::LogicP_list.size(); ++ i)
    {
        LogicP* now = Global::LogicP_list[i].first;
        int x = Global::LogicP_list[i].second.first;
        int y = Global::LogicP_list[i].second.second;
        Node* node = new RNode {now, x, y};
        map_point_to_node[now] = node;
        list_node.push_back(node);
    }
    for (unsigned int i = 0; i < Global::Edge_list.size(); ++ i)
    {
        Node* sp = map_point_to_node[Global::Edge_list[i].first];
        Node* ep = map_point_to_node[Global::Edge_list[i].second];
        //from Node to "OR" : OR
        //otherwise : "AND"
        Arrow::R_type r_type = (ep->get_name() == "OR") ? (Arrow::R_type::OR) : (Arrow::R_type::AND);

        Arrow* now = new Arrow {sp, ep, r_type};
        Global::list_arrow.push_back(now);
        set_node_from_to(sp, ep, now);
    }
    for (unsigned int i = 0; i < Global::Exclu_list.size(); ++ i)
    {
        Node* sp = map_point_to_node[Global::Exclu_list[i].first];
        Node* ep = map_point_to_node[Global::Exclu_list[i].second];
        Arrow* now = new Arrow {sp, ep, Arrow::R_type::EX};
        Global::list_arrow.push_back(now);
        set_node_from_to(sp, ep, now);
    }
    for (unsigned int i = 0; i < Global::Coreq_list.size(); ++ i)
    {
        Node* sp = map_point_to_node[Global::Coreq_list[i].first];
        Node* ep = map_point_to_node[Global::Coreq_list[i].second];
        Arrow* now = new Arrow {sp, ep, Arrow::R_type::CO};
        set_node_from_to(sp, ep, now);
        Global::list_arrow.push_back(now);
    }
    for (unsigned int i = 0; i < Global::Rectangle_list.size(); ++ i)
    {
        qreal xl = Global::Rectangle_list[i].first.first;
        qreal yu = Global::Rectangle_list[i].first.second;
        qreal w = Global::Rectangle_list[i].second.first - xl;
        qreal h = Global::Rectangle_list[i].second.second - yu;
        list_rect.push_back( new QGraphicsRectItem {xl, yu, w, h} );
    }
}

/*
Add the items to the scene.
*/
void CScene::generate_graph()
{
    for (unsigned int i = 0; i < list_node.size(); ++ i)
        this->addWidget(list_node[i]);
    for (unsigned int i = 0; i < Global::list_arrow.size(); ++ i)
        this->addItem(Global::list_arrow[i]);
    for (unsigned int i = 0; i < list_rect.size(); ++ i)
        this->addItem(list_rect[i]);
}

void CScene::Repaint_Map()
{
    Global::extract();
    this->read_from_cmd();
    this->generate_graph();
}

/* Menu Part */
/*
change_<FLAG>_status series, tranfer the change of flag to CMD part, and call CMD to update the graph
at the same time, maintain the list in sidebar, which shows the history queries
*/
void delete_from_list(const QString& s)
{
    QList<QListWidgetItem *> list_to_delete = Global::listwidget->findItems(s, Qt::MatchStartsWith);
    for (auto i = list_to_delete.begin(); i != list_to_delete.end(); ++ i)
    {
        delete *i;
        Global::listwidget->removeItemWidget(*i);
    }
}

void CScene::change_enrolled_status(const bool& enrolled)
{
    if (Global::selected_point == nullptr) return;
    if (enrolled == Global::selected_point->get_enrolled()) return;
    if (enrolled == false)
    {
        Global::selected_point->de_enroll();
        delete_from_list(Global::selected_point->get_name() + Global::ENROLLED );
    }
    else
    {
        Global::selected_point->enroll();
        Global::listwidget->addItem(new QListWidgetItem{ Global::selected_point->get_name() + Global::ENROLLED});
    }
    this->Repaint_Map();
}

void CScene::change_searched_status(const bool& searched)
{
    if (Global::selected_point == nullptr) return;
    if (searched == Global::selected_point->get_searched()) return;
    if (searched == false)
    {
        Global::selected_point->de_search();
        delete_from_list(Global::selected_point->get_name() + Global::SEARCHED );
    }
    else
    {
        Global::selected_point->search();
        Global::listwidget->addItem(new QListWidgetItem{ Global::selected_point->get_name() + Global::SEARCHED });
    }
    this->Repaint_Map();
}

void CScene::change_rsearched_status(const bool& rsearched)
{
    if (Global::selected_point == nullptr) return;
    if (rsearched == Global::selected_point->get_r_searched()) return;
    if (rsearched == false)
    {
        Global::selected_point->de_reverse_search();
        delete_from_list(Global::selected_point->get_name() + Global::R_SEARCHED );
    }
    else
    {
        Global::selected_point->reverse_search();
        Global::listwidget->addItem(new QListWidgetItem{ Global::selected_point->get_name() + Global::R_SEARCHED });
    }
    this->Repaint_Map();
}

/*
move the course clicked in list the the center of the scene
*/
void CScene::itemClicked(QListWidgetItem *item)
{
    QString str=item->text().split('[').first();
    assert(Global::dictionary.count(str));
    assert(map_point_to_node.count(Global::dictionary[str]));
    Node* now= map_point_to_node[ Global::dictionary[str] ];
    QGraphicsScene::views().at(0)->centerOn(now->graphicsProxyWidget());
    emit static_cast<CNode*>(now)->is_clicked();
    //below: jump the cursor
    QPointF go=now->graphicsProxyWidget()->mapToScene(0,0);
    go=QGraphicsScene::views().at(0)->mapFromScene(go);
    go=QGraphicsScene::views().at(0)->mapToGlobal(go);

    //ask for trust
    #ifdef Q_OS_MACOS
    CFStringRef keys[] = { kAXTrustedCheckOptionPrompt };
    CFTypeRef values[] = { kCFBooleanTrue };
    CFDictionaryRef options = CFDictionaryCreate(NULL,
                                                 (const void **)&keys,
                                                 (const void **)&values,
                                                 sizeof(keys) / sizeof(keys[0]),
                                                 &kCFTypeDictionaryKeyCallBacks,
                                                 &kCFTypeDictionaryValueCallBacks);
    AXIsProcessTrustedWithOptions(options);
    CFRelease(options);
    #endif

    QCursor::setPos(go.x()+Global::button_width/2,go.y());
}

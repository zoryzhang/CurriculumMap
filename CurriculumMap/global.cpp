#include<QString>
#include<map>
#include<vector>
using namespace std;

#include"global.h"

//global varible
namespace Global
{
    int button_width=100,button_height=60;
    int Graph_width,Graph_height;

	vector<PointX*> Allpt;
    map<QString,PointX*> dictionary;
    map<QString,PointX*> map_logicp;
    QString input[3000][11];

    vector< pair< Course*,Pos > > Course_list;
    vector< pair< LogicP*,Pos > > LogicP_list;
    vector< pair<PointX*,PointX*> > Edge_list;
    vector< pair<Course*,Course*> > Exclu_list;//(x,y)->x exclu y
    vector< pair<Course*,Course*> > Coreq_list;//(x,y)->x co-requisite y
    vector< pair<Pos,Pos> > Rectangle_list;
    bool Want_exclu=0,Want_co=0,Want_pre=1;

    vector<Arrow*> list_arrow;

    QTextEdit *info_box;
    PointX *selected_point;
    QListWidget *listwidget;
}

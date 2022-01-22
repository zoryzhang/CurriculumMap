#ifndef GLOBAL_H
#define GLOBAL_H

#include<QString>
#include<map>
#include<vector>
#include<QString>
#include<QTextEdit>
#include<QListWidget>
using namespace std;

#include "cmd/cmd_Point.h"
#include "cmd/cmd_LogicP.h"
#include "cmd/cmd_Course.h"
#include "gui/gui_cscene.h"


namespace Global
{
    //global definition
    typedef pair<int,int> Pos;

    //global constant
    const QString ENROLLED="[enrolled]";
    const QString SEARCHED="[searched]";
    const QString R_SEARCHED="[reversely searched]";
    const QString HELP=
        "Welcome to Curriculum Map!\n"
        "Before start, you shall know it's a tool for visualization of undergraduate courses in Hong Kong University of Science and Technology\n\n"
        "Hints: \n"
        "for exclusion, if A excludes B, which means you can't take A after B, there will be a red cross near A >_< \n"
        "A rectangle represents courses inside excludes each other\n"
        "Enrolled flag is helpful in terms of simplifing\n\n"
        "Try some course, click or right click, drag somewhere... enjoy it!";


    //global varible
    extern int button_width,button_height;
    extern int Graph_width,Graph_height;
    extern bool Want_exclu,Want_co,Want_pre;

	extern vector<PointX*> Allpt;
    extern map<QString,PointX*> dictionary;
    extern map<QString,PointX*> map_logicp;
    extern QString input[3000][11];

    extern vector< pair< Course*,Pos > > Course_list;
    extern vector< pair< LogicP*,Pos > > LogicP_list;
    extern vector< pair<PointX*,PointX*> > Edge_list;
    extern vector< pair<Course*,Course*> > Exclu_list;
    extern vector< pair<Course*,Course*> > Coreq_list;

    extern vector< pair<Pos,Pos> > Rectangle_list;

    extern PointX *selected_point;
    extern QTextEdit *info_box;//in ui
    extern QListWidget *listwidget;//in ui

    extern vector<Arrow*> list_arrow;

    //global function
    extern int read_csv(QString dir);//cmd_get_information.cpp
    extern void build_graph(int rows);//cmd_read.cpp
    extern void prepare();//cmd_subgraph.cpp

    extern void extract();//cmd_subgraph.cpp
    extern QString set_style(const QString &name, const bool& enrolled, const bool& search, const bool& rsearch);//color.cpp
}
#endif

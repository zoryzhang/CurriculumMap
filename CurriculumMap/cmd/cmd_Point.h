/*
cmd_Point.h

it define class PointX as base class of class Course and LogicP
store common features: name, flags status, edges, varible used by extract()

layer_val():
it shows contribute to layer increase
since each layer consist of a column of course and multiple columns of logic point

*/

#ifndef CMD_POINT_H
#define CMD_POINT_H

#include<vector>
#include<QString>
using namespace std;

class PointX
{

public:
    PointX()=default;
    PointX(const PointX&) = delete;
    virtual ~PointX()=default;
    virtual int layer_val() = 0;//0 logicP, 1 course

    //Accesser
    QString get_name(){return name;}
    bool get_searched() {return searched;}
    bool get_enrolled() {return enrolled;}
    bool get_r_searched() {return r_searched;}
    const vector<PointX*> & get_from(){return from;}
    const vector<PointX*> & get_to(){return to;}

    //Mutators
    inline void add_from(PointX* course){ from.push_back(course); }
    inline void add_to(PointX* course){ to.push_back(course); }
    inline void set_name(const QString &s){name=s;}

    inline void enroll(){ enrolled=1; }
    inline void de_enroll(){ enrolled=0; }
    inline void search(){ searched=1; }
    inline void de_search(){ searched=0; }
    inline void reverse_search(){ r_searched=1; }
    inline void de_reverse_search(){ r_searched=0; }

private:
    vector<PointX*> from;//pre-requisite
    vector<PointX*> to;//reverse pre-requisite

    QString name;//course name/AND/OR
    bool enrolled=0,searched=0,r_searched=0;

//build for each time "EXTRACT", public for convenience, since meaningless to change
public:
    int depth=0,degree=0;
    bool visible=0,v1,v2,v3;
    int layer=0;//layer=k means this PointX belongs to k-th layer
};

#endif

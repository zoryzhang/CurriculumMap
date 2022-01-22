/*
cmd_Course.h

derive class Course from base class PointX
store unique features that only course hold

derive class Block from base class PointX as a whole merged PointX

*/

#ifndef CMD_COURSE_H
#define CMD_COURSE_H

#include"cmd_Point.h"
#include<QString>
#include<set>

class Block;//declaration
class Course : public PointX
{
public:
    virtual inline int layer_val() override final { return 1; }

    Course() = default;
    virtual ~Course() = default;
	
    //Accessors
    const QString& get_fullname() const {return fullname;}
    const QString& get_credit() const {return credit;}
    const QString& get_description() const {return description;}
    const QString& get_fullpre() const {return fullpre;}
    const QString& get_fullco() const {return fullco;}
    const QString& get_fullex() const {return fullex;}
    const vector<Course*>& get_co(){return co;}
    const set<Course*>& get_exclu(){return exclu;}

    //Mutators
    inline void add_co(Course* course){ co.push_back(course); }
    inline void add_exclu(Course* course){ exclu.insert(course); }

    inline void set_fullname(const QString& s) {fullname = s;}
    inline void set_credit(const QString& s) {credit = s;}
    inline void set_description(const QString& s) {description = s;}
    inline void set_fullpre(const QString& s) {fullpre = s;}
    inline void set_fullco(const QString& s) {fullco = s;}
    inline void set_fullex(const QString& s) {fullex = s;}

    //meaningless to change, so just public
    Block* belong=nullptr;
    vector<Course*> bi_exclu;

private:
    QString fullname, credit, description, fullpre, fullco, fullex;
    vector<Course*> co;//co-requisite
    set<Course*> exclu;//exclusion
};

//merged PointX, meaningless to change, so just public
class Block : public PointX
{
public:
    virtual inline int layer_val() override final { return -1; }
    vector<Course*> in;
    void ins(Course* x){in.push_back(x);x->belong=this;}
};

#endif

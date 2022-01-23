#include <iostream>
#include <algorithm>
#include <vector>
#include <stack>
#include <map>
#include <sstream>
#include <QDebug>

/* Qt Lib*/
#include <QCryptographicHash>

using namespace std;

/*my headers*/
#include "global.h"

int cnt_PointX;

/*
judge_or(), judge_and(), judge_course() :
Judge if the the substring that starts from s[start] represents or/and/course code, respectively.
If it does, return the substring, which will be used to skip loop later;
Otherwise, return empty QString.
*/
QString judge_or(const QString& s, const int& len, const int& start)
{
    if (s[start] == '/') return "/";
    if (start + 1 < len && s.sliced(start,2) == "OR") return "OR";
    return QString();
}
QString judge_and(const QString& s, const int& len, const int& start)
{
    if (s[start] == ',') return ",";
    if (start + 2 < len && s.sliced(start,3) == "AND") return "AND";
    return QString();
}
QString judge_course(const QString& s, const int& len, const int& start)
{
    /* find 4 consecutive BIG letters */
    bool flag = true;
    if (start + 8 < len)
    {
        for (int i = 0; i < 4; ++ i)
            if (!('A' <= s[start + i] && s[start + i] <= 'Z')) flag = false;
        if (!flag) return QString();
    }
    else return QString();
    int end_pos = start + 3;

    /* skip one space */
    if (s[end_pos + 1] == ' ') ++ end_pos;

    /* find 4 consecutive digits */
    for (int i = 0; i < 4; ++ i)
        if (!('0' <= s[end_pos + 1 + i] && s[end_pos + 1 + i] <= '9')) flag = false;
    if (!flag) return QString();
    end_pos += 4;
    /* can be sure that it represents a course code */

    /* suffix H,Q,J etc.*/
    /* OR xxxx-xxxx format */
    if (end_pos + 1 < len)
    {
        if ('A' <= s[end_pos + 1] && s[end_pos + 1] <= 'Z') ++ end_pos;
        else if (s[end_pos + 1] == '-')
        {
            bool flag = true;
            for (int i = 1; i <= 4; ++ i)
                if ( end_pos+1+i>=len or !('0' <= s[end_pos + 1 + i] && s[end_pos + 1 + i] <= '9') ) flag = false;
            if (flag)
                end_pos += 5;
        }
    }
    return s.sliced(start, end_pos - start + 1);
}

/*
get the PointXer to the `Course` specified by a course code
If this course had been constructed, return the PointX directly;
Otherwise, construct it and return
*/
PointX* & get_dictionary(const QString& s) // RBR, would not be killed
{
    if (Global::dictionary.count(s)) // 1 if found, complexity : log in size
        return Global::dictionary[s];
    else // 0 if not found
    {
        PointX *now = new Course;
        Global::Allpt.push_back(now);
        now->set_name(s);
        return Global::dictionary[s] = now;
    }
}
PointX* & get_map_logicp(const QString& s, const bool& flag, bool & flag_exist) // true for AND, false for OR
{
    if (Global::map_logicp.count(s)) // 1 if found
    {
        flag_exist = true;
        return Global::map_logicp[s];
    }
    else
    {
        PointX *now = new LogicP;
        Global::Allpt.push_back(now);
        now->set_name(flag ? "AND" : "OR");
        flag_exist = false;
        return Global::map_logicp[s] = now;
    }
}

/*
Analyze the QString describing the pre-requisite of a course, and construct relevant `PointX`
*/
PointX* get_pre(const QString& s)
{
    if (s.isEmpty()) return nullptr;
    int len = s.length();
    bool is_space = true; // contains only spaces
    for (int i = 0; i < len; ++ i)
        if (s[i] != ' ') is_space = false;
    if (is_space) return nullptr;
    /*
    an stack maintaining the matches between parentheses :
        If it is ( , push it in;
        If it is ) , pop the top ( of the stack
    We only care about the "outermost" parentheses, so that every time we meet a ):
        If it's inner one (stack isn't empty), ignore it;
        If it's the "outermost" one (empty stack), then remove this pair of parentheses, analyze inner content recursively
    */
    stack<int> stk_interval;
    while (!stk_interval.empty()) stk_interval.pop();
    bool relation_now = true; // false for OR, true for AND
    vector<PointX*> children_PointX;
    children_PointX.clear();
    for (int i = 0; i < len; ++ i)
    {
        if (s[i] == '(')
        {
            stk_interval.push(i);
        }
        else if (s[i] == ')')
        {
            int pos = stk_interval.top();
            stk_interval.pop();
            if (stk_interval.empty())
            {
                PointX *child = get_pre(s.sliced(pos + 1, i - pos - 1)); // Recursion
                if (child != nullptr)
                    children_PointX.push_back(child);
            }
        }
        /*
        stack is empty, then :
            If the following is OR/AND, then it is the logical operator connecting "outermost" contents
            If it is a course code, then it is a "outermost" operand
        */
        if (stk_interval.empty())
        {
            QString judge = judge_or(s, len, i);
            if (!judge.isEmpty())
            {
                relation_now = false;
                i += judge.length() - 1;
                continue;
            }
            judge = judge_and(s, len, i);
            if (!judge.isEmpty())
            {
                relation_now = true;
                i += judge.length() - 1;
                continue;
            }
            judge = judge_course(s, len, i);
            if (!judge.isEmpty())
            {
                if(Global::course_name.count(judge))
                    children_PointX.push_back(get_dictionary(judge)),
                    i += judge.length() - 1;
                continue;
            }
        }
    }
    /*
    Special case : Since we ignore less important characters, it might become sth like "( / / / )", "()", etc.
    Just ignore it.
    */
    /*
    bool all_nullptr = true;
    for (unsigned int i = 0; i < children_PointX.size(); ++ i)
    {
        if (children_PointX[i] != nullptr) all_nullptr = false;
    }
    if (all_nullptr) return nullptr;
    */
    if (children_PointX.empty()) return nullptr;
    /*
    Otherwise, it is a valid `PointX`
    For each of the "outermost" operands, we link it with the PointX representing the whole QString, and set the relation to the "outermost" logical operand.
    Special case : if there is only one operands (like "(COMP XXXX)"), we set the relation to AND
    */
    /*
    PointX* relation_PointX = new LogicP;
    Global::Allpt.push_back(relation_PointX);
    relation_PointX->set_name(relation_now ? "AND" : "OR");
    for (unsigned int i = 0; i < children_PointX.size(); ++ i)
    {
//        if (children_PointX[i] == nullptr) continue;
        children_PointX[i]->add_to(relation_PointX);
        relation_PointX->add_from(children_PointX[i]);
    }
    */
    /*

     */
    sort(children_PointX.begin(), children_PointX.end());
    stringstream ss;
    ss.clear();
    ss.str(string());
    ss << (relation_now ? "AND" : "OR");
    for (unsigned int i = 0; i < children_PointX.size(); ++ i)
    {
        const void* address = static_cast<const void*>(children_PointX[i]);
        ss << address;
    }
    bool flag_exist = false;
    PointX* relation_PointX = get_map_logicp(QString::fromStdString(ss.str()), relation_now, flag_exist);
    if (!flag_exist)
    {
        for (unsigned int i = 0; i < children_PointX.size(); ++ i)
        {
            children_PointX[i]->add_to(relation_PointX);
            relation_PointX->add_from(children_PointX[i]);
        }
    }
    return relation_PointX;
}
/*
Reserve character and QString:
    parentheses : ()[], convert [] to ()
    operator : / OR , AND
    course code
    other : space (since the space between 4 big letters and 4 digits takes part in the judgement of a course code )
As mentioned before, sometimes "()" are used to enclose other condition. And get_pre() will judge these cases.
*/
void ignore_info(QString& s) // PBR, modify it
{
    int len = s.length();
    for (int i = 0; i < len; ++ i)
    {
        if (s[i] == '[' || s[i] == '{') s[i] = '(';
        else if (s[i] == ']' || s[i] == '}') s[i] = ')';
    }
    s = s.toUpper();
    bool mark[1000] = {};
    for (int i = 0; i < len; ++ i) mark[i] = false;

    QString tmp = "";

    for (int i = 0; i < len; ++ i)
    {
        if (s[i] == '(' || s[i] == ')' || s[i] == ';' || s[i] == ' ') { mark[i] = true; continue; }
        QString judge = "", tmp = "";
        tmp = judge_or(s, len, i);
        if (!tmp.isEmpty()) judge = tmp;
        else
        {
            tmp = judge_and(s, len, i);
            if (!tmp.isEmpty()) judge = tmp;
            else
            {
                tmp = judge_course(s, len, i);
                if (!tmp.isEmpty()) judge = tmp;
            }
        }
        if (!judge.isEmpty())
        {
            for (unsigned int j = 0; j < tmp.length(); ++ j)
                mark[i + j] = true;
            i += judge.length() - 1;
            continue;
        }
    }
    tmp = "";
    for (int i = 0; i < len; ++ i)
    {
        if (mark[i]) tmp.push_back(s[i]);
    }
    s = tmp;
}

/*
get_list series :
We do not care about the logical operator,
but simply find out all course code and store them in corresponding vectors
*/

void get_list_exclu(int row) //
{
    int len = Global::input[row][4].length();
    if (len == 0) return;
    QString tmp = "";
    for (int i = 0; i < len; ++ i)
    {
        tmp = judge_course(Global::input[row][4], len, i);
        if(!Global::course_name.count(tmp)) continue;
        dynamic_cast<Course*>( get_dictionary(tmp) )->add_exclu(dynamic_cast<Course*>( Global::dictionary[Global::input[row][0]] ));
        i += tmp.length() - 1;
    }
}
void get_list_co(int row) //
{
    int len = Global::input[row][5].length();
    if (len == 0) return;
    QString tmp = "";
    for (int i = 0; i < len; ++ i)
    {
        tmp = judge_course(Global::input[row][5], len, i);
        if(!Global::course_name.count(tmp)) continue;
        dynamic_cast<Course*>(Global::dictionary[Global::input[row][0]])->add_co(dynamic_cast<Course*>(get_dictionary(tmp)));
        i += tmp.length() - 1;
    }
}

void special_case_continuous_course(int& total_rows)
{
    QString temp_input[3000][11];
    int temp_total_rows = 0;
    for (int i = 0; i < total_rows; ++ i)
    {
        QString now = Global::input[i][0];
        if (now.contains('-'))
        {
            //AAAA 0000-9999
            int start_code = (now.sliced(5, 4)).toInt();
            int end_code = (now.sliced(10, 4)).toInt();
            for (int code = start_code; code <= end_code; ++ code)
            {
                QString now_code = now.sliced(0, 5) + QString::number(code);
                temp_input[temp_total_rows][0] = now_code;
                for (int j = 1; j < 11; ++ j)
                    temp_input[temp_total_rows][j] = Global::input[i][j];
                ++ temp_total_rows;
            }
        }
        else
        {
            temp_input[temp_total_rows][0] = now;
            for (int j = 1; j < 11; ++ j)
                temp_input[temp_total_rows][j] = Global::input[i][j];
            ++ temp_total_rows;
        }
    }
    for (int i = 0; i < temp_total_rows; ++ i)
        for (int j = 0; j < 11; ++ j)
        {
            Global::input[i][j] = temp_input[i][j];
        }
    total_rows = temp_total_rows;
}

namespace Global{
/*
input[][] is initialized by other program
each row represents one course
enroll each column to corresponding function
*/
void build_graph(int total_rows)
{
    special_case_continuous_course(total_rows);
    Global::course_name.insert("AND"),Global::course_name.insert("OR");
    for (int i = 0; i < total_rows; ++ i) Global::course_name.insert(Global::input[i][0]);
    for (int i = 0; i < total_rows; ++ i)
    {
        /* Course Code [0]*/
        Course *now = dynamic_cast<Course*>(get_dictionary(Global::input[i][0]));
        now->set_name(Global::input[i][0]);

        /* Course Full Name [1] */
        now->set_fullname(Global::input[i][1]);

        /* Course Credits (save as QString) [2] */
        now->set_credit(Global::input[i][2]);

        /* Handle pre-requisite [3] */
        if(Global::input[i][0]=="MATH 4632")//only such one so lazy stuff
            Global::input[i][3] = "(COMP 2011 OR COMP 2012 OR COMP 2012H) AND (COMP 2711 OR COMP 2711H OR MATH 2343) AND (MATH 2111 OR MATH 2121 OR MATH 2131) ";
        now->set_fullpre(Global::input[i][3]);

        ignore_info(Global::input[i][3]);
        PointX* relation_PointX = get_pre(Global::input[i][3]);
        if (relation_PointX != nullptr)
        {
            relation_PointX->add_to(now);
            now->add_from(relation_PointX);
        }

        /* Handle exclusion [4]*/
        now->set_fullex(Global::input[i][4]);
        get_list_exclu(i);

        /* Handle co-requisite [5]*/
        now->set_fullco(Global::input[i][5]);
        get_list_co(i);

        /* Handle Description [6]-[10] */
        now->set_description(
            (Global::input[i][6].size()? "Co-list with: "+Global::input[i][6]+"\n" : "")+
            (Global::input[i][7].size()? "Mode of Delivery: "+Global::input[i][7]+"\n" : "")+
            (Global::input[i][8].size()? "Previous Course Code: "+Global::input[i][8]+"\n" : "")+
            (Global::input[i][9].size()? "Alternate code: "+Global::input[i][9]+"\n" : "")+
            (Global::input[i][10].size()? "Description: "+Global::input[i][10]+"\n" : "")
        );
    }
}
}

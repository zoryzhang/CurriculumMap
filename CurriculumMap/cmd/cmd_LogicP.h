/*
cmd_LogicP.h
basicly empty
but still nessesary to keep, so that it match with class rnode in gui
*/

#ifndef CMD_LOGICP_H
#define CMD_LOGICP_H

#include"cmd_Point.h"
#include<QString>

class LogicP : public PointX
{
public:
    virtual inline int layer_val() override final { return 0; }
};

#endif

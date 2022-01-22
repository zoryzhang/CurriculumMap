#include<iostream>
#include "global.h"

using namespace std;


const QString colors[47]={
    "ACCT","BIBU","BIEN","CENG","CHEM",
    "CIVL","COMP","CPEG","DASC","DSCT",
    "ECON","ELEC","ENEG","ENGG","ENTR",
    "ENVR","ENVS","FINA","GUBS","GNED",
    "HART","HLTH","HUMA","IDPO","IEDA",
    "IIMP","ISDN","ISOM","LABU","LANG",
    "LIFS","MARK","MATH","MECH","MGMT",
    "OCES","PHYS","PPOL","RMBI","SBMT",
    "SCIE","SHSS","SOSC","SUST","TEMG","UROP","WBBA"};


//set the different course cnode into different color; eg, COMP course share one color and ACCT course share another color.
namespace Global{
QString set_style(const QString &name, const bool& enrolled, const bool& search, const bool& rsearch)
{
    QString border_color = (enrolled) ? "border-color:green;" : "border-color:red;";
    QString border_px = (enrolled || search || rsearch) ? "border-width:4px;" : "border-width:1px;";
    //top right bottom left
    QString border_typ = "border-style: ";
    if (enrolled || search || rsearch)
    {
        QString border_typ_top = enrolled ? "solid" : "dotted";
        QString border_typ_right = rsearch ? "solid" : "dotted";
        QString border_typ_bottom = enrolled ? "solid" : "dotted";
        QString border_typ_left = search ? "solid" : "dotted";
        border_typ += border_typ_top + " " + border_typ_right + " " + border_typ_bottom + " " + border_typ_left + ";";

        QString border_color_top = enrolled ? "green" : "red";
        QString border_color_right = (search || rsearch) ? "red" : "green";
        QString border_color_bottom = enrolled ? "green" : "red";
        QString border_color_left = border_color_right;
        border_color += border_color_top + " " + border_color_right + " " + border_color_bottom + " " + border_color_left + ";";
    }
    else border_typ+="none;";
    QString style = "QPushButton{";
    for(int i = 0; i < 47; ++ i)
    {
        if(name == colors[i])
        {
            if(i<23)
            {
                int r=10+i*10;
                int g=230-i*10;
                style += "background-color:rgba("+QString::number(r)+","+QString::number(g)+",100,10%);";
            }
            else
            {
                int r=10+(i-23)*10;
                int b=10+(i-23)*10;
                style += "background-color:rgba("+QString::number(r)+",70,"+QString::number(b)+",10%);";
            }
            if (enrolled || search || rsearch)
                style += border_px + " " + border_typ + " " + border_color + "}";
            else
                style += "}";
            break;
        }
    }
    return style;
}
}

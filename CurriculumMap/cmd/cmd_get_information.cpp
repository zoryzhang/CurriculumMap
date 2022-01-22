/*

This part of the code is used to read the generated csv file.

*/

#include <iostream>
#include <fstream>
#include <cstring>
#include <cmath>
#include<QFileDialog>
#include<QMessageBox>
#include<QObject>

using namespace std;

#include "global.h"


namespace Global{
int read_csv(QString dir)
{

    FILE *file;
    int all_course_num; 
    int return_from_fgetc;
    char test_in[10000];

    #ifdef Q_OS_MACOS
    QString filename = dir+"/../../../courses.csv";
    #else
    QString filename = dir+"/../../courses.csv";
    #endif
    if ((file = fopen(filename.toStdString().c_str(), "r")) == NULL)
    {
        QMessageBox::warning(nullptr,"Meet some problem","Can't open the file courses.csv !");
        exit(0);
    }

    //read Stastic_use_course_num
    fscanf(file, "%[^,\n]" , test_in); 
    fgetc(file); 
    fscanf(file, "%[^,\n]" , test_in); 
    fgetc(file);

    all_course_num = QString(test_in).toInt() ;

    //read Space
    for(int i = 0; i < 8; i++)
    {
        fscanf(file, "%[^,\n]" , test_in); 
        fgetc(file);
    }

    //read heads
    for(int i = 0; i < 10; i++)
    {
        fscanf(file, "%[^,\n]" , test_in); 
        fgetc(file); 
    }

    //read courses in 11 items
    for(int i = 0; i < all_course_num; i++)
    {
        for (int j = 0; j < 11; j++)
        {
            fscanf(file, "%[^,\n\"]" , test_in); 
            return_from_fgetc = fgetc(file);

            //special cases of "" and \r\n in Mac system
            if(return_from_fgetc == (int)((unsigned char)('\"')))
            {
                fscanf(file, "%[^\"]" , test_in); 
                fgetc(file);
                return_from_fgetc = fgetc(file);
                if (return_from_fgetc == (int)((unsigned char)('\r')))
                    fgetc(file);
            }
            
            Global::input[i][j]=QString(test_in);
            strcpy(test_in,"");
        }
    }
    return all_course_num;
}
}

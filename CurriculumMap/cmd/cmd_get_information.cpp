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
#include <unistd.h>

namespace Global
{
int read_csv(QString Dir)
{
    string filename = getcwd(NULL,0);
#ifdef Q_OS_MACOS
    filename=(Dir+"/../../courses.csv").toStdString();
//    Dir.toStdWString()
#else
    filename+="/courses.csv";
#endif
    cout << filename << endl;

    std::ifstream ifs;
    ifs.exceptions ( std::ifstream::failbit | std::ifstream::badbit );
    try
    {
        ifs.open(filename);
    }
    catch (std::ifstream::failure e)
    {
        cout << "IO Error in" << endl;
        QMessageBox::warning(nullptr,"Meet some problem","Can't open the file : "+QString::fromStdString(filename));
        exit(0);
    }

    int all_course_number;
    string buffer;

    // 1st line
    getline(ifs, buffer, ',');
    ifs >> all_course_number;
    // 1st line rest
    getline(ifs, buffer);

    // 2nd line
    getline(ifs, buffer);

    // courses line
    for (int row = 0; row < all_course_number; ++ row)
    {
        // 9 commas, 10 columns
        /*
        getline(ifs, buffer, '@');
        while (buffer.back() == '\r')
            buffer.pop_back();
        */
        //cout << "row : " << row << endl;
        buffer.append(",");
        for (int col = 0; col < 11; ++ col)
        {
            getline(ifs, buffer, ',');
            for (int i = 0; i < buffer.length(); ++ i)
            {
                if (buffer[i] == '@') buffer[i] = ',';
                else if (buffer[i] == '$') buffer[i] = '\"';
                else if (buffer[i] == '#') buffer[i] = '\'';
            }
            input[row][col] = QString::fromStdString(buffer);
        }
        getline(ifs, buffer);
        /*
        for (int col = 0, i = 0; col < 12; ++ col)
        {
//            cout << "col : " << col << endl;
            char delim = ',';
            int start = i, end = i;
            if (buffer[i] == '"')
                start = ++ i, delim = '"';
            while (buffer[i] != delim) ++ i;
            end = i;
            input[row][col] = QString::fromStdString(buffer.substr(start, end - start));
            ++ i;
            if (delim == '"') ++ i;
        }
        getline(ifs, buffer);
        */
    }
    ifs.close();
    
    return all_course_number;
}
}

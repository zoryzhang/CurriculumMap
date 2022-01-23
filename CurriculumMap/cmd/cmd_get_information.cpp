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


namespace Global
{
int read_csv(QString dir)
{   
#ifdef Q_OS_MACOS
    QString filename = dir+"/../../../courses.csv";
#else
    QString filename = dir+"/../../courses.csv";
#endif
    
    std::ifstream ifs;
    ifs.exceptions ( std::ifstream::failbit | std::ifstream::badbit );
    cout << "fuck" << endl;
    try
    {
        ifs.open(filename.toStdString());
    }
    catch (std::ifstream::failure e)
    {
        cout << "IO Error" << endl;
        exit(0);
    }

    int all_course_number;
    string buffer;

    // 1st line
    getline(ifs, buffer, ',');
    ifs >> all_course_number;
    // 1st line rest
    getline(ifs, buffer, '@');
    getline(ifs, buffer);
    // 2nd line
    getline(ifs, buffer, '@');
    getline(ifs, buffer);

    // courses line
    for (int row = 0; row < all_course_number; ++ row)
    {
        // 9 commas, 10 columns
        getline(ifs, buffer, '@');
        while (buffer.back() == '\r')
            buffer.pop_back();
        //cout << "row : " << row << endl;
        buffer.append(",");
        for (int col = 0, i = 0; col < 12; ++ col)
        {
            cout << "col : " << col << endl;
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
    }
    ifs.close();
    
    return all_course_number;
}
}

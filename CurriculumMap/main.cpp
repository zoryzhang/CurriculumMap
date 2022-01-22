#include "gui/mainwindow.h"

#include "global.h"

#include <QApplication>
#include <QFile>
#include <QTime>
#include <QMessageBox>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Global::build_graph(Global::read_csv( QApplication::applicationDirPath() ));
    Global::prepare();

    //set styleSheet
    QFile css(":/res/style.css");
    assert( css.open(QFile::ReadOnly) );
    a.setStyleSheet(css.readAll());
    css.close();

    //set font and button size properly
    QFont font("Comic Sans MS",14);
    font.setStyleHint(QFont::Monospace);
    a.setFont(font);
    QFontMetrics FontMetrics(font);
    QSize btn_size=FontMetrics.size(Qt::TextSingleLine,"COMP 2012H");
    Global::button_width=btn_size.width()+20,
    Global::button_height=btn_size.height()+20;

    MainWindow w;
    w.show();
    //after window has shown:
    QSettings settings("CurriculumMap","CurriculumMap");
    if( settings.value("new",true).toBool() )
        QMessageBox::information(nullptr,"Welcome!",Global::HELP);
    return a.exec();
}

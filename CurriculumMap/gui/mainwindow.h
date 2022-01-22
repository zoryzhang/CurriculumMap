/* mainwindow.h
Majority of the ui mainwindow is implemented in this .h file. The mainwindow class contains Constructor, destructor and a brench of private slot.

The constructor initialize the sidebar (left most of the mainwindow), statusbar(bottom), graphicsViewers (the place for the map),
and the storage of setting and courses.

The slots in the file are implemented for the push buttons or other interactions shown on the mainwindow. 
*/

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "gui_cscene.h"
#include <QMainWindow>
#include <QMenu>
#include <QPropertyAnimation>
#include <QSplitter>
#include <QSettings>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_btn_enroll_clicked();
    void on_btn_search_clicked();
    void on_btn_search_2_clicked();
    void on_btn_clear_clicked();
    void on_btn_sidebar_clicked();
    void on_btn_export_clicked();
    void on_btn_help_clicked();
    void on_btn_space_clicked();
    void on_btn_cwiki_clicked();

    void on_box_exclu_stateChanged(int arg1);
    void on_box_co_stateChanged(int arg1);


    void on_box_pre_stateChanged(int arg1);

    void on_text_search_returnPressed();

private:
    bool check_course();

    Ui::MainWindow *ui;
    CScene* cscene;
    QSplitter*  splitter;

    bool hide_side_flag;
};
#endif // MAINWINDOW_H

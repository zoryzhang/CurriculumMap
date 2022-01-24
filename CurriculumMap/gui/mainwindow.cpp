#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <iostream>
#include <QMessageBox>
#include <QFileDialog>
#include <QPropertyAnimation>
#include <QTimer>
#include <QThread>
#include <QCursor>
#include <QLabel>
#include <QDesktopServices>
#include <QUrl>
#include"global.h"

using namespace std;


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow), hide_side_flag(0)
{
    ui->setupUi(this);

    //sidebar
    ui->btn_sidebar->setCheckable(true);
    Global::info_box = ui->info_box;
    Global::listwidget = ui->listwidget;
    ui->listwidget->setSortingEnabled(true);
    splitter=new QSplitter(Qt::Vertical,ui->side_widget);
    ui->verticalLayout_2->addWidget(splitter);
    splitter->addWidget(ui->listwidget);
    splitter->addWidget(ui->info_box);
    splitter->addWidget(ui->go_widget);
    splitter->setChildrenCollapsible(false);//avoid size to be 0

    //status bar
    ui->statusbar->showMessage("CURRICULUMMAP @COMP2012H GROUP PROJECT 2021-22");

    //settings
    QSettings settings("CurriculumMap","CurriculumMap");
    if( settings.value("new",true).toBool() ) ui->text_search->setText("Type course here");
    settings.beginGroup("MainWindow");
    resize( settings.value("size",QSize(1300,800)).toSize() );
    move( settings.value("pos",QSize(0,0)).toPoint() );
    splitter->restoreState( settings.value("spllitter_size").toByteArray() );
    if(hide_side_flag!=settings.value("hide_side",0).toBool()) on_btn_sidebar_clicked();
    settings.endGroup();

    settings.beginGroup("Courses");
    for(auto x:Global::Allpt)
    {
        QString name= x->get_name();
        int tmp= settings.value(name,0).toInt() ;
        if(tmp)
        {
            if(tmp/4) x->enroll(),ui->listwidget->addItem(name + Global::ENROLLED);
            if(tmp/2%2) x->search(),ui->listwidget->addItem(name + Global::SEARCHED);
            if(tmp%2) x->reverse_search(),ui->listwidget->addItem(name + Global::R_SEARCHED);
            settings.remove(name);
        }
    }
    settings.endGroup();

    //graphicsViewer
    cscene=new CScene;
    ui->graphicsView->setScene(cscene);
    connect(
        ui->listwidget,SIGNAL(itemClicked(QListWidgetItem*) ),
        cscene, SLOT(itemClicked(QListWidgetItem*) )
    );

    ui->box_pre->setChecked( Global::Want_pre=settings.value("MainWindow/pre",1).toBool() );
    ui->box_co->setChecked( Global::Want_co=settings.value("MainWindow/co",0).toBool() );
    ui->box_exclu->setChecked( Global::Want_exclu=settings.value("MainWindow/exclu",0).toBool() );

    cscene->Repaint_Map();//only once at startup
}

MainWindow::~MainWindow()
{
    //settings
    QSettings settings("CurriculumMap","CurriculumMap");
    settings.setValue("new",false);
    settings.beginGroup("MainWindow");
    settings.setValue("size",size());
    settings.setValue("pos",pos());
    settings.setValue("spllitter_size", splitter->saveState() );
    settings.setValue("hide_side", hide_side_flag );
    settings.setValue("pre", ui->box_pre->isChecked() );
    settings.setValue("co", ui->box_co->isChecked() );
    settings.setValue("exclu", ui->box_exclu->isChecked() );
    settings.endGroup();

    settings.beginGroup("Courses");
    for(auto x:Global::Allpt) if(x->get_enrolled() or x->get_searched() or x->get_r_searched())
        settings.setValue(x->get_name(),x->get_enrolled()*4+x->get_searched()*2+x->get_r_searched());
    settings.endGroup();

    delete cscene;
    delete splitter;
    delete ui;
    for(auto x:Global::Allpt) delete x;
}

bool MainWindow::check_course()
{
    //cOmp1021->COMP 1021
    QString name=ui->text_search->text().toUpper();
    if(name.size()>4 and name.at(4)!=' ') name.insert(4,' ');
    if(Global::course_name.count(name))
    {
        ui->text_search->setText(name);
        return 1;
    }
    QMessageBox::warning(this,"Error :(","No such course(may be postgraduate/canceled course, or you may run get_information.py to grub newest version)");return 0;
}

void MainWindow::on_btn_enroll_clicked()
{
    if(!check_course()) return;
    Global::dictionary[ui->text_search->text()]->enroll();
    QListWidgetItem* p=new QListWidgetItem{ui->text_search->text() + Global::ENROLLED};
    Global::listwidget->addItem(p);
    cscene->Repaint_Map();
    emit ui->listwidget->itemClicked(p);
}

void MainWindow::on_btn_search_clicked()
{
    if(!check_course()) return;
    Global::dictionary[ui->text_search->text()]->search();
    QListWidgetItem* p=new QListWidgetItem{ui->text_search->text() + Global::SEARCHED};
    Global::listwidget->addItem(p);
    cscene->Repaint_Map();
    emit ui->listwidget->itemClicked(p);
}

void MainWindow::on_btn_search_2_clicked()
{
    if(!check_course()) return;
    Global::dictionary[ui->text_search->text()]->reverse_search();
    QListWidgetItem* p=new QListWidgetItem{ui->text_search->text() + Global::R_SEARCHED};
    Global::listwidget->addItem(p);
    cscene->Repaint_Map();
    emit ui->listwidget->itemClicked(p);
}

void MainWindow::on_btn_clear_clicked()
{
    for(auto x:Global::Allpt) x->de_search(),x->de_reverse_search(),x->de_enroll();
    ui->listwidget->clear();
    ui->info_box->clear();
    cscene->Repaint_Map();
}

void MainWindow::on_btn_export_clicked()
{
    //save graph as png file
    QString path = QFileDialog::getSaveFileName(this, tr("Save File"), "/", "Picture File (*.png)");
    if(path.length()!=0) ui->graphicsView->grab().save(path);
}

void MainWindow::on_btn_sidebar_clicked()
{
    if(hide_side_flag)//animation
    {
        ui->btn_sidebar->setText(">>");
        ui->side_widget->setVisible(1);
    }
    else ui->side_widget->setVisible(0),ui->btn_sidebar->setText("<<");
    hide_side_flag = !hide_side_flag;
}

void MainWindow::on_box_pre_stateChanged(int arg1)
{
    Global::Want_pre=arg1;
    if(this->isVisible()) cscene->Repaint_Map();
}

void MainWindow::on_box_exclu_stateChanged(int arg1)
{
    Global::Want_exclu=arg1;
    if(this->isVisible()) cscene->Repaint_Map();
}


void MainWindow::on_box_co_stateChanged(int arg1)
{
    Global::Want_co=arg1;
    if(this->isVisible()) cscene->Repaint_Map();
}

void MainWindow::on_btn_space_clicked()
{
    if(Global::selected_point!=nullptr)
    {
        QString s=Global::selected_point->get_name();
        s=s.split(" ").at(0)+s.split(" ").at(1);
        QDesktopServices::openUrl(QUrl( "https://ust.space/review/"+s  ) );
    }
}


void MainWindow::on_btn_cwiki_clicked()
{
    if(Global::selected_point!=nullptr)
    {
        QString s=Global::selected_point->get_name();
        s=s.split(" ").at(0)+s.split(" ").at(1);
        QDesktopServices::openUrl(QUrl( "http://ug.msss.ust.hk/cwiki_catalog/"+s  ) );
    }
}

void MainWindow::on_btn_help_clicked()
{
    QMessageBox::information(nullptr,"Welcome!", Global::HELP);
}

void MainWindow::on_text_search_returnPressed()
{
    on_btn_search_clicked();
}


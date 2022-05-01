#include "rpgwindow.h"
#include "ui_rpgwindow.h"

RPGwindow::RPGwindow(QWidget *parent, bool isPractice) :
    QWidget(parent),
    ui(new Ui::rpgwindow) {
    ui->setupUi(this);
    if(isPractice){
    }
    QWidget::showMaximized();
}


RPGwindow::~RPGwindow()
{
    delete ui;
}

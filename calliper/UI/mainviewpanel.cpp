#include "mainviewpanel.h"
#include "ui_mainviewpanel.h"

MainViewPanel::MainViewPanel(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MainViewPanel)
{
    ui->setupUi(this);
}

MainViewPanel::~MainViewPanel()
{
    delete ui;
}

#include "mainwindowplaceholder.h"
#include "ui_mainwindowplaceholder.h"

MainWindowPlaceholder::MainWindowPlaceholder(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindowPlaceholder)
{
    ui->setupUi(this);
}

MainWindowPlaceholder::~MainWindowPlaceholder()
{
    delete ui;
}

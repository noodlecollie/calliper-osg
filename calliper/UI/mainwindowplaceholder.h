#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui {
class MainWindowPlaceholder;
}

class MainWindowPlaceholder : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindowPlaceholder(QWidget *parent = 0);
    ~MainWindowPlaceholder();

private:
    Ui::MainWindowPlaceholder *ui;
};

#endif // MAINWINDOW_H

#ifndef MAINVIEWPANEL_H
#define MAINVIEWPANEL_H

#include <QWidget>

namespace Ui {
class MainViewPanel;
}

class MainViewPanel : public QWidget
{
    Q_OBJECT

public:
    explicit MainViewPanel(QWidget *parent = 0);
    ~MainViewPanel();

private:
    Ui::MainViewPanel *ui;
};

#endif // MAINVIEWPANEL_H

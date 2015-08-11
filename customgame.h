#ifndef CUSTOMGAME_H
#define CUSTOMGAME_H

#include <QDialog>
#include <QSpinBox>
#include "gui_main.h"
#include <QSettings>

namespace Ui {
class CustomGame;
}

class CustomGame : public QDialog
{
    Q_OBJECT

public:
    explicit CustomGame( int width, int height, int mines, QWidget *parent = 0);
    ~CustomGame();
    QSpinBox *spinWidth;
    QSpinBox *spinHeight;
    QSpinBox *spinMines;

private:
    Ui::CustomGame *ui;
    QSettings settings;
};

#endif // CUSTOMGAME_H

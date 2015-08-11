#include "customgame.h"
#include "ui_customgame.h"

CustomGame::CustomGame( int width, int height, int mines, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CustomGame)
{
    ui->setupUi(this);
    this->spinWidth = this->ui->spinWidth;
    this->spinHeight = this->ui->spinHeight;
    this->spinMines = this->ui->spinMines;

    this->spinWidth->setValue( width );
    this->spinHeight->setValue( height );
    this->spinMines->setValue( mines );
}



CustomGame::~CustomGame()
{
    delete ui;
}

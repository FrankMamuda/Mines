/*
===========================================================================
Copyright (C) 2015 Avotu Briezhaudzetava

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program. If not, see http://www.gnu.org/licenses/.

===========================================================================
*/

#ifndef GUI_MAIN
#define GUI_MAIN

//
// includes
//
#include <QMainWindow>
#include <QGridLayout>
#include <QPushButton>
#include <QList>
#include <QDebug>
#include <QLabel>
#include "minebutton.h"
#include <QTime>
#include "tile.h"
#include <QSettings>

//
// namespace: Ui
//
namespace Ui {
class Gui_Main;
}

//
// namespace: Mines
//
namespace Mines {
    const static unsigned int buttonSize = 32;
    const static unsigned int maxSize = 256;
    const static unsigned int defaultMines = 30;
    const static unsigned int width = 20;
    const static unsigned int height = 20;

}

//
// class: Gui_Main
//
class Gui_Main : public QMainWindow {
    Q_OBJECT

public:
    explicit Gui_Main(QWidget *parent = 0);
    ~Gui_Main();

private:
    Ui::Gui_Main *ui;
    QGridLayout *grid;
    QList <Tile*> minefield;
    QList <Tile*> mines;
    int minefieldWidth;
    int minefieldHeight;
    int numMines;
    int numFlags;
    bool gameOver;
    bool gameStart;
    QSettings settings;

    Tile *tileForLocation( int x, int y ) {
        if ( x < 0 || y < 0 || x >= this->minefieldWidth || y >= this->minefieldHeight )
            return NULL;

        foreach ( Tile *tile, this->minefield ) {
            if ( tile->x == x && tile->y == y )
                return tile;
        }
        return NULL;
    }
    QList <Tile*> getNearbyTiles( Tile *tile, bool mined = false, bool ignoreChecked = false );
    int getNearbyMineNum( Tile *tile );
    void setMineNum(Tile *tile , int mines);

private slots:
    void buttonPressed( int x, int y );
    void defuse( int x, int y );
    void refillMines();
    void refillField( int width, int height, int numMines );
    void on_smileyButton_clicked();
    void displayFlags( int mines );
    void on_actionBeginner_triggered();
    void on_actionIntermediate_triggered();
    void on_actionExpert_triggered();
    void on_actionInsane_triggered();
    void on_actionCustom_triggered();
};

#endif // GUI_MAIN

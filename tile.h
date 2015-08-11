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

#ifndef TILE_H
#define TILE_H

//
// includes
//
#include <QObject>
#include "minebutton.h"
#include <QGridLayout>

//
// class: Tile
//
class Tile : public QObject {
    Q_OBJECT

public:
   explicit Tile( int x, int y, QGridLayout *grid );
    ~Tile() { this->disconnect( this->button, SIGNAL(leftClicked())); this->disconnect( this->button, SIGNAL(rightClicked())); this->grid->removeWidget( this->button ); delete this->button; this->grid = NULL; }

   int x;
   int y;
   bool hasMine;
   int number;
   float rand;
   MineButton *button;
   QGridLayout *grid;
   bool visited;
   bool flagged;

signals:
   void pressed( int x, int y );
   void rightPressed( int x, int y );

public slots:
   void buttonPressed() {
       emit this->pressed( this->x, this->y );
   }
   void rightButtonPressed() {
       emit this->rightPressed( this->x, this->y );
   }
};


#endif // TILE_H

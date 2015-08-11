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

//
// includes
//
#include "tile.h"
#include "gui_main.h"
#include <QFont>

/*
 * constructor
 */
Tile::Tile( int row, int column, QGridLayout *grid ) {
        this->x = row;
        this->y = column;
        this->button = new MineButton();
        this->button->setMaximumWidth( Mines::buttonSize );
        this->button->setMaximumHeight( Mines::buttonSize );
        this->button->setMinimumWidth( Mines::buttonSize );
        this->button->setMinimumHeight( Mines::buttonSize );
        this->button->setCheckable( true );
        this->button->setStyleSheet( "MineButton:checked{ border: none; }" );
        this->button->connect( this->button, SIGNAL(leftClicked()), this, SLOT(buttonPressed()));
        this->button->connect( this->button, SIGNAL(rightClicked()), this, SLOT(rightButtonPressed()));
        this->grid = grid;
        this->grid->addWidget( this->button, column, row, 1, 1 );
        this->visited = false;
        this->flagged = false;

        QFont font = this->button->font();
        font.setPointSize( 18 );
        this->button->setFont( font );
}

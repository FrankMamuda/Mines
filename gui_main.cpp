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
#include "gui_main.h"
#include "ui_gui_main.h"
#include <algorithm>
#include <QSpacerItem>
#include <QPalette>
#include "customgame.h"

/*
 * flrand (non-class)
 */
float flrand () {
    return static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
}

/*
 * greater (non-class)
 */
struct greater {
    template<class T>
    bool operator()(T const &a, T const &b) const { return a > b; }
};

/*
 * refillMines
 */
void Gui_Main::refillMines() {
    QList<float> randDensity;
    int counter = 0;

    this->mines.clear();

    foreach ( Tile *tile, this->minefield ) {
        tile->hasMine = false;
        tile->visited = false;
        tile->flagged = false;
        tile->button->setText( "" );
        tile->button->setChecked( false );
        tile->button->setIcon( QIcon());
        tile->rand = flrand();
        randDensity << tile->rand;
    }

    std::sort( randDensity.begin(), randDensity.end(), greater());

    foreach ( Tile *tile, this->minefield ) {
        if ( randDensity.contains( tile->rand ) && randDensity.indexOf( tile->rand ) < this->numMines ) {
            tile->hasMine = true;
            this->mines << tile;
            counter++;
        }

        if ( counter == this->numMines )
            break;
    }
}

/*
 * refillField
 */
void Gui_Main::refillField( int width, int height, int numMines ) {
    int row, column;

    this->minefieldWidth = width;
    this->minefieldHeight = height;
    this->numMines = numMines;
    this->numFlags = 0;
    this->gameOver = false;
    this->gameStart = false;

    foreach ( Tile *tile, this->minefield ) {
        this->disconnect( tile, SIGNAL(pressed(int,int)));
        this->disconnect( tile, SIGNAL(rightPressed(int,int)));
        this->grid->removeWidget( tile->button );
        delete tile;
    }
    this->minefield.clear();
    this->mines.clear();

    for ( column = 0; column < this->minefieldWidth; column++ ) {
        for ( row = 0; row < this->minefieldHeight; row++ )
            this->minefield << new Tile( column, row, this->grid );
    }


    foreach ( Tile *tile, this->minefield ) {
        this->connect( tile, SIGNAL(pressed(int,int)), this, SLOT(buttonPressed(int,int)));
        this->connect( tile, SIGNAL(rightPressed(int,int)), this, SLOT(defuse(int,int)));
    }

    int y;

    // this seems ultra-dumb, but it werks!!
    for ( y = 0; y < 10; y++ )
          QApplication::processEvents();

    this->resize( this->minimumSizeHint());
}

/*
 * construcor (class: Gui_Main)
 */
Gui_Main::Gui_Main( QWidget *parent ) : QMainWindow( parent) , ui( new Ui::Gui_Main ) {
   ui->setupUi(this);
   this->grid = this->ui->grid;
   this->refillField( this->settings.value("game/width", Mines::width ).toInt(), this->settings.value("game/height", Mines::height ).toInt(), this->settings.value( "game/mines",  Mines::defaultMines ).toInt() );
   this->refillMines();
   this->grid->setHorizontalSpacing( 11 );
   this->grid->setVerticalSpacing( 11 );
   this->resize( 1, 1 );
   this->ui->flagNumber->display( this->numMines );
  // this->setS
//   this->window()->siz
}

/*
 * destrucor (class: Gui_Main)
 */
Gui_Main::~Gui_Main() {
    foreach ( Tile *tile, this->minefield ) {
        this->disconnect( tile, SIGNAL(pressed(int,int)));
        this->disconnect( tile, SIGNAL(rightPressed(int,int)));
        delete tile;
    }
    delete ui;
}

/*
 * getNearbyTiles
 */
QList <Tile*> Gui_Main::getNearbyTiles( Tile *tile, bool mined, bool ignoreChecked ) {
    int y;
    QList <Tile*> out;

    int tiles[8][2] = {
        { -1, -1 }, // top left
        { 0, -1 }, // top
        { 1, -1 }, // top right
        { -1, 0 }, // left
        { 1, 0 }, // right
        { -1, 1 }, // bottom left
        { 0, 1 }, // bottom
        { 1, 1 } // bottom right

    };

    for ( y = 0; y < 8; y++ ) {
        Tile *newTile = this->tileForLocation( tile->x + tiles[y][0], tile->y + tiles[y][1] );
        if ( newTile != NULL ) {
            if ( mined ) {
                if ( newTile->hasMine )
                    out << newTile;
            } else
                if ( ignoreChecked ) {
                    if ( !newTile->button->isChecked() && !newTile->visited )
                        out << newTile;
                } else
                   out << newTile;
        }
    }
    return out;
}

/*
 * getNearbyMineNum
 */
int Gui_Main::getNearbyMineNum( Tile *tile ) {
    QList <Tile*> nearbyMines = this->getNearbyTiles( tile, true );
    return nearbyMines.count();
}

/*
 * setMineNum
 */
void Gui_Main::setMineNum( Tile *tile, int mines ) {
    if ( tile == NULL )
        return;

    if ( mines > 0 )
        tile->button->setText( QString( "%1" ).arg( mines ));

    QPalette pal = tile->button->palette();
    switch ( mines ) {
    case 1:
        pal.setColor(QPalette::ButtonText, QColor::fromRgb( 0, 0, 255 ));
        break;
    case 2:
        pal.setColor(QPalette::ButtonText, QColor::fromRgb( 0, 130, 0 ));
        break;
    case 3:
        pal.setColor(QPalette::ButtonText, QColor::fromRgb( 255, 0, 0 ));
        break;
    case 4:
        pal.setColor(QPalette::ButtonText, QColor::fromRgb( 0, 0, 132 ));
        break;
    case 5:
        pal.setColor(QPalette::ButtonText, QColor::fromRgb( 132, 0, 0 ));
        break;
    case 6:
        pal.setColor(QPalette::ButtonText, QColor::fromRgb( 0, 132, 132 ));
        break;
    case 7:
        pal.setColor(QPalette::ButtonText, QColor::fromRgb( 132, 0, 132 ));
        break;
    case 8:
        pal.setColor(QPalette::ButtonText, QColor::fromRgb( 132, 132, 132 ));
        break;
    }

    tile->button->setPalette( pal );
}

/*
 * buttonPressed
 */
void Gui_Main::buttonPressed( int x, int y ) {
    if ( this->gameOver)
        return;

 //   qDebug() << x, y;

    Tile *tile = this->tileForLocation( x, y );

    if ( tile == NULL )
        return;

    if ( tile->flagged )
        return;

    if ( !this->gameStart )
        this->ui->lcdTimer->timer->start( 1000 );


    // check for legal moves
    bool anyOpen = false;
    {

        foreach ( Tile *tile, this->minefield ) {
            if ( tile->visited )
                anyOpen = true;
        }

        if ( !anyOpen )
            qDebug() << "no legal moves at start";
    }

    /*{
        bool legalMoves = false;
        foreach ( Tile *tile, this->minefield ) {
            if ( tile->visited ) {
                 QList <Tile*> nearbyTiles = this->getNearbyTiles( tile, false, true );

                 int tilesWithMines = 0, openTiles = 1;
                 foreach ( Tile *nTile, nearbyTiles ) {
                    if ( nTile->hasMine )
                        tilesWithMines++;

                    if ( nTile->visited )
                        openTiles++;
                 }

                 qDebug() << openTiles << tilesWithMines;


                 if ( openTiles + tilesWithMines == 9 ) {
                     legalMoves = true;
                 }

                 //  MMC   mines = 2   NOT OK   0 + 2 < 9
                 //  C2C   closed = 6
                 //  CCC   open = 0
                 //
                 //  MMO   mines = 2   OK      7 + 2 = 9
                 //  O2O   closed = 0
                 //  OOO   open = 7
                 //
                 //  MMO   mines = 2   NOT OK  6 + 2 < 9
                 //  O2O   closed = 1
                 //  OOC   open = 6
                 //
            }
        }

        qDebug() << legalMoves;
    }*/


    tile->button->setChecked( true );
    tile->visited = true;
    this->gameStart = true;

    if ( !tile->hasMine ) {
        int mines = this->getNearbyMineNum( tile );

        if ( mines > 0 )
            this->setMineNum( tile, mines );
        else {
            QList <Tile*> nearbyTiles = this->getNearbyTiles( tile, false, true );
            while ( !nearbyTiles.isEmpty()) {
                foreach ( Tile* nTile, nearbyTiles ) {
                    mines = this->getNearbyMineNum( nTile );

                    if ( nTile->flagged ) {
                        nTile->button->setIcon( QIcon());
                        nTile->flagged = false;
                        this->numFlags--;
                        this->displayFlags( this->numMines - this->numFlags );
                    }

                    if ( nTile->visited ) {
                        nearbyTiles.removeAll( nTile );
                        continue;
                    }

                    if ( mines > 0 ) {
                        this->setMineNum( nTile, mines );
                        nTile->button->setChecked( true );
                        nTile->visited = true;
                        nearbyTiles.removeAll( nTile );
                        continue;
                    }

                    nTile->visited = true;
                    nearbyTiles.removeAll( nTile );

                    if ( !nTile->hasMine ) {
                        nTile->button->setChecked( true );
                    }

                    nearbyTiles.append( this->getNearbyTiles( nTile, false, true ));
                }
            }
        }
    } else {
        foreach ( Tile *tile, this->minefield ) {
            if ( tile->hasMine ) {
                tile->button->setIcon( QIcon( ":/icons/mine.png" ));
                tile->button->setChecked( false );
            }
        }

        this->gameOver = true;
        this->ui->lcdTimer->timer->stop();
        this->ui->smileyButton->setIcon( QIcon( ":/icons/sad.png" ));
    }

    {
        if ( anyOpen ) {


            bool legalMoves =false;

            foreach ( Tile *tile, this->minefield ) {
                if ( tile->visited ) {

                    QList <Tile*> nearbyTiles = this->getNearbyTiles( tile, false, true );



                     int tilesWithMines = 0, openTiles = 1, flaggedTiles = 0;
                     foreach ( Tile *nTile, nearbyTiles ) {
                        if ( nTile->hasMine )
                            tilesWithMines++;

                        if ( nTile->visited )
                            openTiles++;

                        if ( nTile->flagged && nTile->hasMine )
                            flaggedTiles++;
                     }

                     //  MMC   mines = 2        NOT OK   2 : 8 - 0   2 < 8
                     //  C2C   closed = 6 + 1
                     //  CCC   open = 0
                     //
                     //  MMO   mines = 2        OK       2 : 8 - 6   2 = 2
                     //  O2O   closed = 0
                     //  OOO   open = 6 +
                     //
                     //  MMO   mines = 2        NOT OK   2 : 8 - 5   2 < 3
                     //  O2O   closed = 1
                     //  OOC   open = 5
                     //

                     //  MOO   mines = 1       1 : 8 - 7    1 < 2
                     //  O1O   open = 7
                     //  OOO   closed = 0




                    if ( tilesWithMines > 0 && tilesWithMines == nearbyTiles.count() + 1 - openTiles ) {
                        qDebug() << QString( "   legal move at %1 %2" ).arg( tile->x ).arg( tile->y );
                        legalMoves = true;
                    }
                }
            }

            if ( !legalMoves)
                qDebug() << "no legal moves";
        }
    }

}

/*
 * defuse
 */
void Gui_Main::defuse( int x, int y ) {

    if ( this->gameOver)
        return;

    Tile *tile = this->tileForLocation( x, y );

    if ( tile == NULL )
        return;

    if ( tile->visited )
        return;

    if ( !tile->flagged ) {
        //if ( tile->)
        tile->button->setIcon( QIcon( ":/icons/flag.png" ));
        tile->flagged = true;
        this->numFlags++;

        // special case
        // TODO: check all FIELDS

        if ( this->numFlags == this->numMines ) {
            int counter = 0/*, visited = 0*/;

            foreach ( Tile *tile, this->minefield ) {
                if ( tile->hasMine && tile->flagged )
                    counter++;
               // else
               //     visited++;
            }

            if ( counter == this->numMines /*&& visited == this->minefield.size() - this->mines.size()*/ ) {
                this->ui->smileyButton->setIcon( QIcon( ":/icons/laugh.png" ));
                this->ui->lcdTimer->timer->stop();
            }
        }
    } else {
        tile->button->setIcon( QIcon());
        tile->flagged = false;
        this->numFlags--;
    }

    this->displayFlags( this->numMines - this->numFlags );
}

/*
 * smileyButton->clicked
 */
void Gui_Main::on_smileyButton_clicked() {
    this->refillMines();
    this->gameStart = false;
    this->ui->lcdTimer->timer->stop();
    this->ui->lcdTimer->timeValue->setHMS( 0, 0, 0 );
    this->ui->lcdTimer->display( 0 );
    this->ui->smileyButton->setIcon( QIcon( ":/icons/smile.png" ));
    this->numFlags = 0;
    this->gameOver = false;
    this->displayFlags( this->numMines );
    qDebug() << "new game";
}

/*
 * displayFlags
 */
void Gui_Main::displayFlags( int mines ) {
    this->ui->flagNumber->display( mines );
}

/*
 * actionBeginner->triggered
 */
void Gui_Main::on_actionBeginner_triggered() {
    settings.setValue( "game/width", 8 );
    settings.setValue( "game/height", 8 );
    settings.setValue( "game/mines", 10 );
    this->refillField( 8, 8, 10 );
    this->on_smileyButton_clicked();
}

/*
 * actionIntermediate->triggered
 */
void Gui_Main::on_actionIntermediate_triggered() {
    settings.setValue( "game/width", 16 );
    settings.setValue( "game/height", 16 );
    settings.setValue( "game/mines", 40 );
    this->refillField( 16, 16, 40 );
    this->on_smileyButton_clicked();
}

/*
 * actionExpert->triggered
 */
void Gui_Main::on_actionExpert_triggered() {
    settings.setValue( "game/width", 30 );
    settings.setValue( "game/height", 16 );
    settings.setValue( "game/mines", 99 );
    this->refillField( 30, 16, 99 );
    this->on_smileyButton_clicked();
}

/*
 * actionInsane->triggered
 */
void Gui_Main::on_actionInsane_triggered() {
    settings.setValue( "game/width", 16 );
    settings.setValue( "game/height", 16 );
    settings.setValue( "game/mines", 150 );
    this->refillField( 30, 16, 150 );
    this->on_smileyButton_clicked();
}

/*
 * actionCustom->triggered
 */
void Gui_Main::on_actionCustom_triggered() {
  //  int lastWidth = ;
  //  int lastHei = ;
 //   int lastWidth = ;

    CustomGame cgDialog( this->settings.value("game/width", Mines::width ).toInt(), this->settings.value("game/height", Mines::height ).toInt(), this->settings.value("game/mines", Mines::defaultMines ).toInt(), this );
    switch ( cgDialog.exec()) {
    case QDialog::Accepted:
    {
        int width = cgDialog.spinWidth->value();
        int height = cgDialog.spinHeight->value();
        int mines = cgDialog.spinMines->value();

        if ( mines >= width * height )
            // TODO: ERROR
            return;

        settings.setValue( "game/width", width );
        settings.setValue( "game/height", height );
        settings.setValue( "game/mines", mines );
        this->refillField( width, height, mines );
        this->on_smileyButton_clicked();
    }
        break;

    default:
    case QDialog::Rejected:
        break;
    }

}

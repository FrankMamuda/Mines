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

#ifndef LCDTIMER
#define LCDTIMER

//
// includes
//
#include <QLCDNumber>
#include <QTimer>
#include <QTime>

//
// class: LCDTimer
//
class LCDTimer: public QLCDNumber {
  Q_OBJECT

  public:
  QTimer* timer;
  QTime*  timeValue;

  public:
    LCDTimer(QWidget * parentWidget) {
        this->timer = new QTimer();
        this->timeValue = new QTime(0,0,0);
        this->setParent(parentWidget);
        this->display(0);
        QObject::connect(this->timer,SIGNAL(timeout()),this,SLOT(setDisplay()));
    }
    ~ LCDTimer(){}

   public slots:
    void setDisplay() {
      this->timeValue->setHMS(0,this->timeValue->addSecs(0).minute(),this->timeValue->addSecs(1).second());
      this->display( 60 * this->timeValue->minute() + this->timeValue->second());
    }
};


#endif // LCDTIMER


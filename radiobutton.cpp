#include "radiobutton.h"
#include <QDebug>

RadioButton::RadioButton(QWidget *parent) :
	QRadioButton(parent)
{
}


void RadioButton::paintEvent(QPaintEvent *e)
{
	QRadioButton::paintEvent(e);
	//qDebug() << "r";
}

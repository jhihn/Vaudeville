#ifndef RADIOBUTTON_H
#define RADIOBUTTON_H

#include <QRadioButton>

class RadioButton : public QRadioButton
{
	Q_OBJECT
	void paintEvent(QPaintEvent *);
public:
	explicit RadioButton(QWidget *parent = 0);

signals:

public slots:

};

#endif // RADIOBUTTON_H

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "httpserver.h"
#include "qpaintengine_html5_p.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
	Q_OBJECT

	HttpServer m_http;

	QHtml5PaintEngine *m_engine;
	QByteArray m_commandStorage;
	QBuffer m_commandBuffer;

	void paintEvent ( QPaintEvent * event );
	void timerEvent(QTimerEvent *);

public:
	explicit MainWindow(QWidget *parent = 0);
	~MainWindow();
public slots:
	acceptConnection();
	gotMouseEvent(QMouseEvent*);
	gotKeyEvent(QMouseEvent*);


private:
	Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H

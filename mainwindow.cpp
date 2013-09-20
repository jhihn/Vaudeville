#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QPainter>
#include <QDebug>
#include "qhtml5canvas.h"
#include "httpserver.h"

MainWindow::MainWindow(QWidget *parent) :
	QMainWindow(parent),
	ui(new Ui::MainWindow)
{
	ui->setupUi(this);
	setAutoFillBackground(true);
	startTimer(1000);
	m_commandBuffer=QBuffer(&m_commandStorage, this);

	connect(&m_http, SIGNAL(newConnection()), this, SLOT(acceptConnection()));
	connect(&m_http, SIGNAL(gotMouseEvent(QMouseEvent*)), this, SLOT(doMouseEvent(QMouseEvent*)));
	connect(&m_http, SIGNAL(gotKeyEvent(QKeyEvent*)), this, SLOT(doKeyEvent(QKeyEvent*)));
}

MainWindow::~MainWindow()
{
	delete ui;
}

void MainWindow::paintEvent ( QPaintEvent * event )
{

	QMainWindow::paintEvent(event);
	if (m_acceptedSocket)
	{
		if (m_commandStorage.size())
		{
			m_http.sendCommand(m_commandStorage);
			m_commandStorage.truncate();
			m_commandBuffer.seek(0);
		}
	}

}
void MainWindow::timerEvent(QTimerEvent *)
{
	//if (ui->progressBar->value())
		ui->progressBar->setValue(ui->progressBar->value()-1);
}

void MainWindow::acceptConnection()
{

	m_http.sendCommand("<!DOCTYPE html>\r\n<HTML><BODY onLoad=\"draw()\"><CANVAS id=\"qt\" width=\"1024\" height=\"768\"></BODY></HTML>\r\n<SCRIPT LANGUAGE=\"JavaScript\">\r\n");
	m_http.sendCommand("function draw()\r\n{\r\n\tvar canvas = document.getElementById(\"qt\");\r\n\tvar context = canvas.getContext('2d');");
	m_http.sendCommand("\r\n\tcontext.shadowBlur=0;\r\n");
	QPalette p;
	m_pEngine->updateBackground(Qt::OpaqueMode, QBrush(QColor(p.color(QPalette::Background))));
	m_http.sendCommand(QString("\tcontext.fillStyle = \"%1\";\r\n").arg(p.color(QPalette::Background).name()).toUtf8());
	m_http.sendCommande(QString("\tcontext.fillRect(%2, %3, %4, %5);\r\n")
				   .arg(0).arg(0)
				   .arg(geometry.width()).arg(geometry.height())
				   .toUtf8());
	m_http.sendCommand("\r\n");
	m_http.sendCommand("}\r\n</SCRIPT>\r\n");
	m_engine = new QHtml5PaintEngine(&m_commandBuffer);

}
void MainWindow::doMouseEvent(QMouseEvent* e)
{
	switch (e->type())
	{
		case QEvent::MouseMove: mouseMoveEvent(e); break;
		case QEvent::MouseButtonDblClick: mouseDoubleClickEvent(e); break;
		case QEvent::MouseButtonPress: mousePressEvent(e); break;
		case QEvent::MouseButtonRelease: mouseReleaseEvent(e); break;

	}
	deleteLater(e);
}

void MainWindow::doKeyEvent(QKeyEvent* e)
{
	switch (e->type())
	{
		case QEvent::KeyPress: keyPressEvent(e); break;
		case QEvent::KeyRelease: keyReleaseEvent(e); break;
	}

	deleteLater(e);
}


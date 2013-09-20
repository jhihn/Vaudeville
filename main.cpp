#include <QtGui/QApplication>
#include "mainwindow.h"
#include "qjsoncanvas.h"
#include "qhtml5canvas.h"



int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	MainWindow w;
	w.show();

	/*QHtml5Canvas c(w.geometry());
	w.render(&c);
	c.save("123.html");*/

	return a.exec();
}

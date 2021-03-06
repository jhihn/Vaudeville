#include <QFile>

#include "qhtml5canvas.h"
#include "qpaintengine_html5_p.h"

#include <QPalette>

QHtml5Canvas::QHtml5Canvas(QRect geometry)
	:QPicture()
{
	m_pEngine = new QHtml5PaintEngine(&m_buffer);
	m_buffer.open((QIODevice::WriteOnly));
	m_buffer.write("<!DOCTYPE html>\r\n<HTML><BODY onLoad=\"draw()\"><CANVAS id=\"qt\" width=\"1024\" height=\"768\"></BODY></HTML>\r\n<SCRIPT LANGUAGE=\"JavaScript\">\r\n");
	m_buffer.write("function draw()\r\n{\r\n\tvar canvas = document.getElementById(\"qt\");\r\n\tvar context = canvas.getContext('2d');");
	//m_buffer.write("\r\n\tcontext.translate(0.5, 0);");
	m_buffer.write("\r\n\tcontext.shadowBlur=0;\r\n");
	QPalette p;
	m_pEngine->updateBackground(Qt::OpaqueMode, QBrush(QColor(p.color(QPalette::Background))));
	m_buffer.write(QString("\tcontext.fillStyle = \"%1\";\r\n").arg(p.color(QPalette::Background).name()).toUtf8());
	m_buffer.write(QString("\tcontext.fillRect(%2, %3, %4, %5);\r\n")
				   .arg(0).arg(0)
				   .arg(geometry.width()).arg(geometry.height())
				   .toUtf8());
	m_buffer.write("\r\n");
}

QHtml5PaintEngine *QHtml5Canvas::paintEngine() const
{
	return m_pEngine;
}

bool QHtml5Canvas::save ( const QString & fileName, const char * format)
{
	QFile f(fileName);
	f.open(QIODevice::WriteOnly);
	f.write(m_buffer.data());
	f.write("}</SCRIPT>\r\n");
	f.close();

	Q_UNUSED(format);
	return true;}

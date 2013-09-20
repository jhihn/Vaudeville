#include <QFile>

#include "qjsoncanvas.h"
#include "qpaintengine_json_p.h"

#include <QPalette>

QJsonCanvas::QJsonCanvas(QRect geometry)
	:QPicture()
{
	m_pEngine = new QJsonPaintEngine(&m_buffer);
	m_buffer.open((QIODevice::WriteOnly));
	m_buffer.write("<!DOCTYPE html>\r\n<HTML><BODY onLoad=\"draw()\"><CANVAS id=\"qt\" width=\"1024\" height=\"768\"></BODY></HTML>\r\n<SCRIPT LANGUAGE=\"JavaScript\">\r\n");
	m_buffer.write("function draw()\r\n{cmds = [\r\n");
	QPalette p;

	m_buffer.write("\r\n");
}

QJsonPaintEngine *QJsonCanvas::paintEngine() const
{
	return m_pEngine;
}

bool QJsonCanvas::save ( const QString & fileName, const char * format)
{
	QFile f(fileName);
	f.open(QIODevice::WriteOnly);
	f.write(m_buffer.data());
	f.write("];}</SCRIPT>\r\n");
	f.close();

	Q_UNUSED(format);
	return true;
}

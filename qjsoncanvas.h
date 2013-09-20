#ifndef QJSONCANVAS_H
#define QJSONCANVAS_H

#include <QPicture>
#include <QBuffer>
#include "qpaintengine_json_p.h"

class QJsonCanvas : public QPicture
{
	QJsonPaintEngine *m_pEngine;
	QBuffer m_buffer;

public:
	QJsonCanvas(QRect geometry);
	QJsonPaintEngine *paintEngine() const;
	bool save ( const QString & fileName, const char * format = 0);
};

#endif // QHTML5CANVAS_H

#ifndef QHTML5CANVAS_H
#define QHTML5CANVAS_H

#include <QPicture>
#include <QBuffer>
#include "qpaintengine_html5_p.h"

class QHtml5Canvas : public QPicture
{
	QHtml5PaintEngine *m_pEngine;
	QBuffer m_buffer;

public:
	QHtml5Canvas(QRect geometry);
	QHtml5PaintEngine *paintEngine() const;
	bool save ( const QString & fileName, const char * format = 0);
};

#endif // QHTML5CANVAS_H

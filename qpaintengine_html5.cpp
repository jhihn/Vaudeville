#include "private/qpaintengine_p.h"
#include "qbuffer.h"
#include "qbytearray.h"
#include "qtextstream.h"
#include "qmath.h"
#include "qpaintengine_html5_p.h"
#include "qpicture.h"
#include "qpolygon.h"
#include "qrect.h"


#include <QPaintEngine>
#include <QPen>
#include <QBrush>
#include <QFont>
#include <QTransform>
#include <QRegion>
#include <QPainterPath>
#include <QUuid>
#define QT_PICTURE_DEBUG
#include <qdebug.h>

class QHtml5PaintEnginePrivate : public QPaintEnginePrivate
{
public:
	QIODevice *b;
	QTextStream s;
	QPainter *pt;
	QHtml5PaintEnginePrivate *pic_d;
	QPoint pos;
	QPen strokePen;
	QBrush fillBrush;
	QBrush backgroundBrush;
	QFont font;
};

QHtml5PaintEngine::QHtml5PaintEngine(QIODevice *buffer)
	: QPaintEngine(*(new QHtml5PaintEnginePrivate), AllFeatures)
{
	Q_D(QHtml5PaintEngine);
	d->pt = 0;
	d->b=buffer;

}

QHtml5PaintEngine::QHtml5PaintEngine(QHtml5PaintEnginePrivate &dptr)
	: QPaintEngine(dptr, AllFeatures)
{
	Q_D(QHtml5PaintEngine);
	d->pt = 0;
}

QHtml5PaintEngine::~QHtml5PaintEngine()
{
}

bool QHtml5PaintEngine::begin(QPaintDevice *pd)
{
	Q_D(QHtml5PaintEngine);
#ifdef QT_PICTURE_DEBUG
	qDebug() << "QHtml5PaintEngine::begin()";
#endif
	Q_ASSERT(pd);
	d->s.setDevice(d->b);
	d->s << "\tcontext.save()\r\n";

	setActive(true);
	return true;
}

bool QHtml5PaintEngine::end()
{
	Q_D(QHtml5PaintEngine);
#ifdef QT_PICTURE_DEBUG
	qDebug() << "QHtml5PaintEngine::end()";
#endif
	d->s << "\tcontext.restore();\r\n";
	setActive(false);
	return true;
}


void QHtml5PaintEngine::setCanvasPen()
{
	Q_D(QHtml5PaintEngine);
	if (d->strokePen.style()==Qt::SolidLine)
	{
		d->s << QString("\tcontext.strokeStyle = \"%1\";\r\n").arg(d->strokePen.color().name());
		d->s << QString("\tcontext.lineWidth = %1;\r\n").arg(d->strokePen.width() );
		d->s << QString("\tcontext.globalAlpha = %1;\r\n").arg(d->strokePen.color().alphaF());
	} else if (d->strokePen.style()==Qt::NoPen) {
		d->s << QString("\tcontext.globalAlpha = 0.0;\r\n");
		d->s << QString("\tcontext.lineWidth = 0;\r\n");
	} else {
		qDebug() << "PEN STYLE NOT IMPLEMENTED" << d->strokePen.style();
	}
}

void QHtml5PaintEngine::updatePen(const QPen &pen)
{
	Q_D(QHtml5PaintEngine);
#ifdef QT_PICTURE_DEBUG
	qDebug() << " -> updatePen(): width:" << pen.width() << "style:" << pen.style() << "color:" << pen.color().name();
	d->s << "/* updatePen(): width:" << pen.width() << " style:" << pen.style() << " color:" << pen.color().name() << " */\r\n";
#endif
	d->strokePen = pen;
	setCanvasPen();
}

void QHtml5PaintEngine::updateCompositionMode(QPainter::CompositionMode cmode)
{
	Q_D(QHtml5PaintEngine);
#ifdef QT_PICTURE_DEBUG
	qDebug() << " -> updateCompositionMode():" << cmode;
#endif
	QString mode;
	switch(cmode)
	{
		case QPainter::CompositionMode_SourceOver: mode = "source-over";
		case QPainter::CompositionMode_DestinationOver: mode = "destation-over";
		case QPainter::CompositionMode_Clear: mode = "";
		case QPainter::CompositionMode_Source: mode = "source-over";
		case QPainter::CompositionMode_Destination: mode = "destination-atop";
		case QPainter::CompositionMode_SourceIn: mode = "source-in";
		case QPainter::CompositionMode_DestinationIn: mode = "destination-in";
		case QPainter::CompositionMode_SourceOut: mode = "source-out";
		case QPainter::CompositionMode_DestinationOut: mode = "destination-out";
		case QPainter::CompositionMode_SourceAtop: mode = "source-atop";
		case QPainter::CompositionMode_DestinationAtop: mode = "";
		case QPainter::CompositionMode_Xor: mode = "xor";
		case QPainter::CompositionMode_Plus: mode = "";
		case QPainter::CompositionMode_Multiply: mode = "";
		case QPainter::CompositionMode_Screen: mode = "";
		case QPainter::CompositionMode_Overlay: mode = "";
		case QPainter::CompositionMode_Darken: mode = "";
		case QPainter::CompositionMode_Lighten: mode = "lighter";
		case QPainter::CompositionMode_ColorDodge: mode = "";
		case QPainter::CompositionMode_ColorBurn: mode = "";
		case QPainter::CompositionMode_HardLight: mode = "";
		case QPainter::CompositionMode_SoftLight: mode = "";
		case QPainter::CompositionMode_Difference: mode = "";
		case QPainter::CompositionMode_Exclusion: mode = "";
		case QPainter::RasterOp_SourceOrDestination: mode = "";
		case QPainter::RasterOp_SourceAndDestination: mode = "";
		case QPainter::RasterOp_SourceXorDestination: mode = "xor";
		case QPainter::RasterOp_NotSourceAndNotDestination: mode = "";
		case QPainter::RasterOp_NotSourceOrNotDestination: mode = "";
		case QPainter::RasterOp_NotSourceXorDestination: mode = "";
		case QPainter::RasterOp_NotSource: mode = "";
		case QPainter::RasterOp_NotSourceAndDestination: mode = "";
		case QPainter::RasterOp_SourceAndNotDestination: mode = "";
	}
	d->s << QString("context.globalCompositeOperation = \"%1\";\r\n").arg(mode);

}

void QHtml5PaintEngine::updateClipEnabled(bool enabled)
{
	Q_D(QHtml5PaintEngine);
#ifdef QT_PICTURE_DEBUG
	qDebug() << " -> updateClipEnabled():" << enabled;
#endif
	qDebug() << "PREVIOUS FUNCTION NOT IMPLEMENTED";
	Q_UNUSED(d)
}

void QHtml5PaintEngine::updateOpacity(qreal opacity)
{
	Q_D(QHtml5PaintEngine);
#ifdef QT_PICTURE_DEBUG
	qDebug() << " -> updateOpacity():" << opacity;
#endif
	d->s << QString("\tcontext.globalAlpha = %1;\r\n").arg(opacity);
}

void QHtml5PaintEngine::setCanvasFillBrush()
{
	Q_D(QHtml5PaintEngine);
	d->s << QString("\tcontext.fillStyle = \"%1\";\r\n").arg(d->fillBrush.color().name());
	d->s << QString("\tcontext.globalAlpha = %1;\r\n").arg(d->fillBrush.color().alphaF());
}

void QHtml5PaintEngine::updateBrush(const QBrush &brush)
{
	Q_D(QHtml5PaintEngine);
#ifdef QT_PICTURE_DEBUG
	qDebug() << " -> updateBrush(): style:" << brush.style() << brush.color().name();
	d->s << "/* updateBrush(): style:" << brush.style() << "color:" << brush.color().name() << " */ \r\n";
#endif
	d->fillBrush = brush;
	setCanvasFillBrush();
}

void QHtml5PaintEngine::updateBrushOrigin(const QPointF &p)
{
	Q_D(QHtml5PaintEngine);
#ifdef QT_PICTURE_DEBUG
	qDebug() << " -> updateBrushOrigin(): " << p;
#endif
	qDebug() << "PREVIOUS FUNCTION NOT IMPLEMENTED";
	Q_UNUSED(d)
}

void QHtml5PaintEngine::updateFont(const QFont &font)
{
	Q_D(QHtml5PaintEngine);
#ifdef QT_PICTURE_DEBUG
	qDebug() << " -> updateFont(): pt sz:" << font.pointSize();
	qDebug() << "/* updateFont(): pt sz:" << font.pointSize() << " name:" << font.family() << " */";
#endif
	QStringList decorators;
	if (font.italic())     decorators << "itallic";
	if (font.strikeOut())   decorators << "strikeout";
	if (font.underline())   decorators << "underline";
	switch (font.weight())
	{
	case QFont::Light: decorators << "lighter"; break;
	case QFont::DemiBold: decorators << "bolder"; break;
	case QFont::Bold: decorators << "bold"; break;
	case QFont::Black: decorators << "bold"; break;
	case QFont::Normal: break;
	default: break;
	}

	d->font =  font;
	if (font.pixelSize() != -1)
		d->s << QString("\tcontext.font = \"%1 %2px '%3'\";\r\n").arg(decorators.join(" ")).arg(font.pixelSize()).arg(font.family());
	else
		d->s << QString("\tcontext.font = \"%1 %2pt '%3'\";\r\n").arg(decorators.join(" ")).arg(font.pointSize()).arg(font.family());
}

void QHtml5PaintEngine::updateBackground(Qt::BGMode bgMode, const QBrush &bgBrush)
{
	Q_D(QHtml5PaintEngine);
#ifdef QT_PICTURE_DEBUG
	qDebug() << " -> updateBackground(): mode:" << bgMode << "style:" << bgBrush.style();
	d->s << "/* > updateBackground(): color:" <<bgBrush.color().name() << " mode:" << bgMode << " */\r\n";
#endif
	d->backgroundBrush = bgBrush;
}

void QHtml5PaintEngine::updateMatrix(const QTransform &matrix)
{
	Q_D(QHtml5PaintEngine);
#ifdef QT_PICTURE_DEBUG
	qDebug() << " -> updateMatrix():" << matrix;
#endif
	d->s << QString("\tcontext.setTransform(%1, %2, %3, %4, %5, %6);\r\n")
			.arg(matrix.m11())
			.arg(matrix.m12())
			.arg(matrix.m21())
			.arg(matrix.m22())
			.arg(matrix.dx())
			.arg(matrix.dy());
}

void QHtml5PaintEngine::updateClipRegion(const QRegion &region, Qt::ClipOperation op)
{
	Q_D(QHtml5PaintEngine);
#ifdef QT_PICTURE_DEBUG
	qDebug() << " -> updateClipRegion(): op:" << op
			 << "bounding rect:" << region.boundingRect();
#endif
	if (op != Qt::NoClip)
	{
		d->s << QString("\tcontext.rect(%1, %2, %3, %4);\r\n")
			.arg(region.boundingRect().left())
			.arg(region.boundingRect().top())
			.arg(region.boundingRect().left()+region.boundingRect().width())
			.arg(region.boundingRect().top()+region.boundingRect().height());
		d->s << QString("\tcontext.clip();\r\n");
	}
}

void QHtml5PaintEngine::updateClipPath(const QPainterPath &path, Qt::ClipOperation op)
{
	Q_D(QHtml5PaintEngine);
#ifdef QT_PICTURE_DEBUG
	qDebug() << " -> updateClipPath(): op:" << op
			 << "bounding rect:" << path.boundingRect();
#endif
	qDebug() << "PREVIOUS FUNCTION NOT IMPLEMENTED";
	Q_UNUSED(d)
}

void QHtml5PaintEngine::updateRenderHints(QPainter::RenderHints hints)
{
	Q_D(QHtml5PaintEngine);
#ifdef QT_PICTURE_DEBUG
	qDebug() << " -> updateRenderHints(): " << hints;
#endif
	qDebug() << "PREVIOUS FUNCTION NOT IMPLEMENTED";
	Q_UNUSED(d)
}

void QHtml5PaintEngine::drawEllipse(const QRectF &rect)
{
	Q_D(QHtml5PaintEngine);
#ifdef QT_PICTURE_DEBUG
	qDebug() << " -> drawEllipse():" << rect;
#endif
	d->s << QString("\tcontext.arc(%1, %2, %3, %4, %5, %6);\r\n")
			.arg(rect.center().x()+0.5)
			.arg(rect.center().y()+0.5)
			.arg(rect.center().y())
			.arg(0)
			.arg(2*3.14159)
			.arg("true");
}

void QHtml5PaintEngine::drawPath(const QPainterPath &path)
{
	Q_D(QHtml5PaintEngine);
#ifdef QT_PICTURE_DEBUG
	qDebug() << " -> drawPath():" << path.boundingRect();
	d->s << "/* drawPath(): boundingRect:" << " */\r\n";
#endif
	QString command;
	 d->s << QString("\tcontext.beginPath();\r\n");
	 for(int i=0; i< path.elementCount(); i++)
	 {
		 QPainterPath::Element e = path.elementAt(i);
		 switch (e.type)
		 {
		 case QPainterPath::MoveToElement:
			 command = QString("\t\tcontext.moveTo(%1, %2);\r\n").arg(e.x+0.5).arg(e.y+0.5); break;
		 case QPainterPath::LineToElement:
			 command = QString("\t\tcontext.lineTo(%1, %2);\r\n").arg(e.x+0.5).arg(e.y+0.5); break;
		 case QPainterPath::CurveToElement:
			 command = QString("\t\tcontext.arcTo(%1, %2, %3, %4);\r\n").arg(d->pos.x()+0.5).arg(d->pos.y()+0.5).arg(e.x+0.5).arg(e.y+0.5); break;
		 case QPainterPath::CurveToDataElement:
			 command = QString("\t\tcontext.arcTo(%1, %2, %3, %4);\r\n").arg(d->pos.x()+0.5).arg(d->pos.y()+0.5).arg(e.x+0.5).arg(e.y+0.5); break;
		 default: break;
		 }

		  d->s << command;
		  d->pos.setX(e.x);
		  d->pos.setX(e.y);
	 }
	 if (d->fillBrush.style()==Qt::SolidPattern)
		d->s << QString("\tcontext.fill();\r\n");
	 if (d->strokePen.style() != 0)
		d->s << QString("\tcontext.stroke();\r\n");

}

void QHtml5PaintEngine::drawPolygon(const QPointF *points, int numPoints, PolygonDrawMode mode)
{
	Q_D(QHtml5PaintEngine);
#ifdef QT_PICTURE_DEBUG
	qDebug() << " -> drawPolygon(): size:" << numPoints << "mode:" << mode;
	d->s << "/* drawPolygon(): size:" << numPoints << " mode:" << mode << " */\r\n";
#endif

	if (numPoints < 1) return;
	 d->s << QString("\tcontext.beginPath();\r\n");
	 d->s << QString("\t\tcontext.moveTo(%1, %2);\r\n").arg(points[0].x()+0.5).arg(points[0].y()+0.5);
	 for(int i=1; i< numPoints; i++)
	 {
		 d->s << QString("\t\tcontext.lineTo(%1, %2);\r\n").arg(points[i].x()+0.5).arg(points[i].y()+0.5);
	 }

	 if (mode != (PolygonDrawMode)QPaintEngine::PolylineMode)
		d->s << QString("\tcontext.fill();\r\n");

	 d->s << QString("\tcontext.stroke();\r\n");

}

void QHtml5PaintEngine::drawPixmap(const QRectF &r, const QPixmap &pm, const QRectF &sr)
{
	Q_D(QHtml5PaintEngine);
#ifdef QT_PICTURE_DEBUG
	qDebug() << " -> drawPixmap():" << r;
#endif
	QString src="img" + QUuid::createUuid().toString().replace("-", "").replace("{", "");

	pm.save(QString("%1.png").arg(src));

	//d->s << QString("\tvar %1=document.getElementById(\"%1\");\r\n").arg(image);
	d->s << QString("\tvar %1=new Image();\r\n");
	d->s << QString("\t%1.src=\"%1\"").arg(src);
	d->s << QString("\tcontext.drawImage(%1,   %2, %3, %4, %5,   %6, %7, %8, %9);\r\n")
			.arg(src)
			.arg(sr.left()).arg(sr.top()).arg(sr.width()).arg(sr.height())
			.arg(r.left()).arg(r.top()).arg(r.width()).arg(r.height());
}

void QHtml5PaintEngine::drawTiledPixmap(const QRectF &r, const QPixmap &pixmap, const QPointF &s)
{
	Q_D(QHtml5PaintEngine);
#ifdef QT_PICTURE_DEBUG
	qDebug() << " -> drawTiledPixmap():" << r << s;
#endif
	QString src="img" + QUuid::createUuid().toString().replace("-", "").replace("{", "");

	pixmap.save(QString("%1.png").arg(src));

	d->s << QString("\t\tvar clipped=c.getContext(\"2d\");\r\n");
	d->s << QString("\t\tclipped.rect(%1, %2, %3, %4);\r\n").arg(r.top()).arg(r.left()).arg(r.width()).arg(r.height());
	d->s << QString("\t\tclipped.clip();\r\n");
	d->s << QString("\tvar %1=new Image();\r\n");
	d->s << QString("\t%1.src=\"%1\"").arg(src);

	for(int y=s.y(); y < r.height()-s.y(); y+=pixmap.height())
	{
		for(int x=s.x(); x < r.width()-s.x(); x+=pixmap.width())
		{
			d->s << QString("\t\tclipped.drawImage(%1, %2, %3);\r\n").arg(src).arg(x+0.5).arg(y+0.5);
		}
	}
}

void QHtml5PaintEngine::drawImage(const QRectF &r, const QImage &image, const QRectF &sr,
									Qt::ImageConversionFlags flags)
{
	Q_D(QHtml5PaintEngine);
#ifdef QT_PICTURE_DEBUG
	qDebug() << " -> drawImage():" << r << sr;
#endif
	QString src="img" + QUuid::createUuid().toString().replace("-", "").replace("{", "");

	image.save(QString("%1.png").arg(src));

	//d->s << QString("\tvar %1=document.getElementById(\"%1\");\r\n").arg(image);
	d->s << QString("\tvar %1=new Image();\r\n");
	d->s << QString("\t%1.src=\"%1\"").arg(src);
	d->s << QString("\tcontext.drawImage(%1,   %2, %3, %4, %5,   %6, %7, %8, %9);\r\n")
			.arg(src)
			.arg(sr.left()).arg(sr.top()).arg(sr.width()).arg(sr.height())
			.arg(r.left()).arg(r.top()).arg(r.width()).arg(r.height());
}

void QHtml5PaintEngine::drawTextItem(const QPointF &p , const QTextItem &ti)
{
	Q_D(QHtml5PaintEngine);
#ifdef QT_PICTURE_DEBUG
	qDebug() << " -> drawTextItem():" << p << ti.text();
	d->s << "/* drawTextItem():" << p.x() << "," << p.y() << " text:" << ti.text() << " */\r\n";
#endif
	QFontMetrics fm(d->font);
	fm.height();
	d->s << QString("\tcontext.fillStyle = \"%1\";\r\n").arg(d->backgroundBrush.color().name());
	d->s << QString("\tcontext.fillRect(%1, %2, %3, %4);\r\n").arg(p.x()+0.5).arg(p.y()+0.5-fm.height()).arg(fm.width(ti.text())).arg(fm.height());
	d->s << QString("\tcontext.fillStyle = \"%1\";\r\n").arg(d->fillBrush.color().name());
	d->s << QString("\tcontext.fillText(\"%1\",   %2, %3);\r\n").arg(ti.text()).arg(p.x()+0.5).arg(p.y()+0.5);


}

void QHtml5PaintEngine::updateState(const QPaintEngineState &state)
{
	QPaintEngine::DirtyFlags flags = state.state();
	if (flags & DirtyPen) updatePen(state.pen());
	if (flags & DirtyBrush) updateBrush(state.brush());
	if (flags & DirtyBrushOrigin) updateBrushOrigin(state.brushOrigin());
	if (flags & DirtyFont) updateFont(state.font());
	if (flags & DirtyBackground) updateBackground(state.backgroundMode(), state.backgroundBrush());
	if (flags & DirtyTransform) updateMatrix(state.transform());
	if (flags & DirtyClipEnabled) updateClipEnabled(state.isClipEnabled());
	if (flags & DirtyClipRegion) updateClipRegion(state.clipRegion(), state.clipOperation());
	if (flags & DirtyClipPath) updateClipPath(state.clipPath(), state.clipOperation());
	if (flags & DirtyHints) updateRenderHints(state.renderHints());
	if (flags & DirtyCompositionMode) updateCompositionMode(state.compositionMode());
	if (flags & DirtyOpacity) updateOpacity(state.opacity());
}


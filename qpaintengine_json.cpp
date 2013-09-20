#include "private/qpaintengine_p.h"

#include <Qbuffer>
#include <QBytearray>
#include <QTextstream>

#include <QPicture>
#include <QPolygon>
#include <QRect>
#include <QPen>
#include <QBrush>
#include <QFont>
#include <QTransform>
#include <QRegion>
#include <QPainterPath>
#include <QUuid>
#define QT_PICTURE_DEBUG
#include <qdebug.h>

#include "qpaintengine_json_p.h"

QT_BEGIN_NAMESPACE

class QJsonPaintEnginePrivate : public QPaintEnginePrivate
{
	Q_DECLARE_PUBLIC(QJsonPaintEngine)
public:
	QIODevice *b;
	QTextStream s;
	QPainter *pt;
	QJsonPaintEnginePrivate *pic_d;
	QPoint pos;
	QPen strokePen;
	QBrush fillBrush;
};

QJsonPaintEngine::QJsonPaintEngine(QIODevice *buffer)
	: QPaintEngine(*(new QJsonPaintEnginePrivate), AllFeatures)
{
	Q_D(QJsonPaintEngine);
	d->pt = 0;
	d->b=buffer;

}

QJsonPaintEngine::QJsonPaintEngine(QJsonPaintEnginePrivate &dptr)
	: QPaintEngine(dptr, AllFeatures)
{
	Q_D(QJsonPaintEngine);
	d->pt = 0;
}

QJsonPaintEngine::~QJsonPaintEngine()
{
}

bool QJsonPaintEngine::begin(QPaintDevice *pd)
{
	Q_D(QJsonPaintEngine);
#ifdef QT_PICTURE_DEBUG
	qDebug() << "QHtml5PaintEngine::begin()";
#endif
	Q_ASSERT(pd);
	d->s.setDevice(d->b);
	d->s << "[\r\n";

	setActive(true);
	return true;
}

bool QJsonPaintEngine::end()
{
	Q_D(QJsonPaintEngine);
#ifdef QT_PICTURE_DEBUG
	qDebug() << "QHtml5PaintEngine::end()";
#endif
	d->s << "],\r\n";
	setActive(false);
	return true;
}


void QJsonPaintEngine::setCanvasPen()
{
	Q_D(QJsonPaintEngine);
	QString("\t{\"p\": {\"s\": \"%1\", \"w\": %2, \"a\": %3, \"x\": %4}},\r\n")
		.arg(d->strokePen.color().name())
		.arg(d->strokePen.width())
		.arg(d->strokePen.color().alphaF())
		.arg(d->strokePen.style());
}

void QJsonPaintEngine::updatePen(const QPen &pen)
{
	Q_D(QJsonPaintEngine);
#ifdef QT_PICTURE_DEBUG
	qDebug() << " -> updatePen(): width:" << pen.width() << "style:" << pen.style() << "color:" << pen.color().name();
#endif
	d->strokePen = pen;
	setCanvasPen();
}

void QJsonPaintEngine::updateCompositionMode(QPainter::CompositionMode cmode)
{
	Q_D(QJsonPaintEngine);
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
	d->s << QString("\t{\"cm\": {\"m\": \"%1\"}},\r\n").arg(mode);

}

void QJsonPaintEngine::updateClipEnabled(bool enabled)
{
	Q_D(QJsonPaintEngine);
#ifdef QT_PICTURE_DEBUG
	qDebug() << " -> updateClipEnabled():" << enabled;
#endif
	d->s << QString("\t{\"ce\": {\"e\": \"%1\"}},\r\n").arg(enabled);
	Q_UNUSED(d)
}

void QJsonPaintEngine::updateOpacity(qreal opacity)
{
	Q_D(QJsonPaintEngine);
#ifdef QT_PICTURE_DEBUG
	qDebug() << " -> updateOpacity():" << opacity;
#endif
	d->s << QString("\t{\"o\": {\"o\": %1}},\r\n").arg(opacity);
}

void QJsonPaintEngine::setCanvasFillBrush()
{
	Q_D(QJsonPaintEngine);

	d->s << QString("\t{\"fs\": {\"c\": \"%1\", \"o\": %2}},\r\n")
		.arg(d->fillBrush.color().name())
		.arg(d->fillBrush.color().alphaF());
}

void QJsonPaintEngine::updateBrush(const QBrush &brush)
{
	Q_D(QJsonPaintEngine);
#ifdef QT_PICTURE_DEBUG
	qDebug() << " -> updateBrush(): style:" << brush.style() << brush.color().name();
#endif
	d->fillBrush = brush;
	setCanvasFillBrush();
}

void QJsonPaintEngine::updateBrushOrigin(const QPointF &p)
{
	Q_D(QJsonPaintEngine);
#ifdef QT_PICTURE_DEBUG
	qDebug() << " -> updateBrushOrigin(): " << p;
#endif
	d->s << QString("\t{\"bo\": {\"x\": \"%1\", \"y\": %2}},\r\n")
		.arg(p.x())
		.arg(p.y());
	qDebug() << "PREVIOUS FUNCTION NOT IMPLEMENTED";
	Q_UNUSED(d)
}

void QJsonPaintEngine::updateFont(const QFont &font)
{
	Q_D(QJsonPaintEngine);
#ifdef QT_PICTURE_DEBUG
	qDebug() << " -> updateFont(): pt sz:" << font.pointSize();
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

	QString units;
	float size;
	if (font.pixelSize() != -1)
	{
		units="px";
		size = font.pixelSize();
	} else {
		units="pt";
		size = font.pointSize();
	}

	d->s << QString("\t{\"f\": { \"d\": \"%1\", \"s\": %2, \"u\": \"%4\", \"f\": \"%3\"}},\r\n")
			.arg(decorators.join(" "))
			.arg(size)
			.arg(font.family())
			.arg(units);
}

void QJsonPaintEngine::updateBackground(Qt::BGMode bgMode, const QBrush &bgBrush)
{
	Q_D(QJsonPaintEngine);
#ifdef QT_PICTURE_DEBUG
	qDebug() << " -> updateBackground(): mode:" << bgMode << "style:" << bgBrush.style();
#endif
	d->s << QString("\t{\"b\": { \"m\": \"%1\", \"b\": {\"c\": \"%2\", \"s\": \"%3\"}}},\r\n")
			.arg(bgMode)
			.arg(bgBrush.color().name())
			.arg(bgBrush.style());
}

void QJsonPaintEngine::updateMatrix(const QTransform &matrix)
{
	Q_D(QJsonPaintEngine);
#ifdef QT_PICTURE_DEBUG
	qDebug() << " -> updateMatrix():" << matrix;
#endif
	d->s << QString("\t{\"m\": {\"m11\": %1, \"m12\": %2, \"m21\": %3, \"m22\": %4, \"dx\": %5, \"dy\": %6}},\r\n")
			.arg(matrix.m11())
			.arg(matrix.m12())
			.arg(matrix.m21())
			.arg(matrix.m22())
			.arg(matrix.dx())
			.arg(matrix.dy());
}

void QJsonPaintEngine::updateClipRegion(const QRegion &region, Qt::ClipOperation op)
{
	Q_D(QJsonPaintEngine);
#ifdef QT_PICTURE_DEBUG
	qDebug() << " -> updateClipRegion(): op:" << op
			 << "bounding rect:" << region.boundingRect();
#endif
	if (op != Qt::NoClip)
	{
		d->s << QString("\t{\"cr\": {\"x\": %1, \"y\": %2, \"w\": %3, \"h\":%4}},\r\n")
			.arg(region.boundingRect().left())
			.arg(region.boundingRect().top())
			.arg(region.boundingRect().left()+region.boundingRect().width())
			.arg(region.boundingRect().top()+region.boundingRect().height());
	}
}

void QJsonPaintEngine::updateClipPath(const QPainterPath &path, Qt::ClipOperation op)
{
	Q_D(QJsonPaintEngine);
#ifdef QT_PICTURE_DEBUG
	qDebug() << " -> updateClipPath(): op:" << op
			 << "bounding rect:" << path.boundingRect();
#endif
	d->s << QString("\t{\"cp\": {\"x\": \"notimplemented\"}},\r\n");
}

void QJsonPaintEngine::updateRenderHints(QPainter::RenderHints hints)
{
	Q_D(QJsonPaintEngine);
#ifdef QT_PICTURE_DEBUG
	qDebug() << " -> updateRenderHints(): " << hints;
#endif
	qDebug() << "PREVIOUS FUNCTION NOT IMPLEMENTED";
	Q_UNUSED(d)
}

void QJsonPaintEngine::drawEllipse(const QRectF &rect)
{
	Q_D(QJsonPaintEngine);
#ifdef QT_PICTURE_DEBUG
	qDebug() << " -> drawEllipse():" << rect;
#endif
	d->s << QString("\te\"e\": {\"x\": %1, \"y\": %2, \"r\": %3, \"s\": %4, \"e\": %5, \"cw\": %6}},\r\n")
			.arg(rect.center().x())
			.arg(rect.center().y())
			.arg(rect.center().y())
			.arg(0)
			.arg(2*3.14159)
			.arg("true");
}

void QJsonPaintEngine::drawPath(const QPainterPath &path)
{
	Q_D(QJsonPaintEngine);
#ifdef QT_PICTURE_DEBUG
	qDebug() << " -> drawPath():" << path.boundingRect();
#endif
	QString command;
	d->s << QString("\t{\"p\": \r\n\t\t[\r\n");
	 for(int i=0; i< path.elementCount(); i++)
	 {
		 QPainterPath::Element e = path.elementAt(i);
		 switch (e.type)
		 {
		 case QPainterPath::MoveToElement:
			 command = QString("\t\t{\"m\": {\"x\": %1, \"y\":%2}},\r\n").arg(e.x).arg(e.y); break;
		 case QPainterPath::LineToElement:
			 command = QString("\t\t{\"l\": {\"x\": %1, \"y\":%2}},\r\n").arg(e.x).arg(e.y); break;
		 case QPainterPath::CurveToElement:
			 command = QString("\t\t{\"a\": (\"x1\": %1, \"y1\": %2, \"x2\": %3, \"y2\": %4}},\r\n").arg(d->pos.x()).arg(d->pos.y()).arg(e.x).arg(e.y); break;
		 case QPainterPath::CurveToDataElement:
			 command = QString("\t\t{\"ax\": (\"x1\": %1, \"y2\": %2, \"x2\": %3, \"y2\": %4}},\r\n").arg(d->pos.x()).arg(d->pos.y()).arg(e.x).arg(e.y); break;
		 default: break;
		 }

		  d->s << command;
		  d->pos.setX(e.x);
		  d->pos.setX(e.y);
	 }
	 d->s << QString("\t\t]\r\n\t},\r\n");
}

void QJsonPaintEngine::drawPolygon(const QPointF *points, int numPoints, PolygonDrawMode mode)
{
	Q_D(QJsonPaintEngine);
#ifdef QT_PICTURE_DEBUG
	qDebug() << " -> drawPolygon(): size=" << numPoints << mode;
#endif

	d->s << QString("\t{\"pl\": \r\n\t\t{\"pdm\":%1,\r\n\t\t\"p\":[\r\n").arg(mode);
	for(int i=0; i< numPoints; i++)
	 {
		 d->s << QString("\t\t\t{\"l\": {\"x\": %1, \"y\":%2}},\r\n").arg(points[i].x()).arg(points[i].y());
	 }
	 d->s << QString("\t\t]}\r\n\t},\r\n");
}

void QJsonPaintEngine::drawPixmap(const QRectF &r, const QPixmap &pm, const QRectF &sr)
{
	Q_D(QJsonPaintEngine);
#ifdef QT_PICTURE_DEBUG
	qDebug() << " -> drawPixmap():" << r;
#endif
	QString src="img" + QUuid::createUuid().toString().replace("-", "").replace("{", "");

	pm.save(QString("%1.png").arg(src));
	d->s << QString("\t{\"i\": \"s\": \"%1\", \"sx\":%2, \"sy\":%3, \"sw\": %4, \"sh\": %5, \"dx\": %6, \"dy\": %7, \"dw\": %8, \"dh\": %9},\r\n")
			.arg(src)
			.arg(sr.left()).arg(sr.top()).arg(sr.width()).arg(sr.height())
			.arg(r.left()).arg(r.top()).arg(r.width()).arg(r.height());
}

void QJsonPaintEngine::drawTiledPixmap(const QRectF &r, const QPixmap &pixmap, const QPointF &s)
{
	Q_D(QJsonPaintEngine);
#ifdef QT_PICTURE_DEBUG
	qDebug() << " -> drawTiledPixmap():" << r << s;
#endif
	QString src="img" + QUuid::createUuid().toString().replace("-", "").replace("{", "");

	pixmap.save(QString("%1.png").arg(src));
	d->s << QString("\t{\"ti\": \"s\": \"%1\", \"x\":%2, \"y\":%3, \"w\": %4, \"h\": %5, \"px\": %6, \"py\": %7},\r\n")
			.arg(src)
			.arg(r.left()).arg(r.top()).arg(r.width()).arg(r.height())
			.arg(s.x()).arg(s.y());
}

void QJsonPaintEngine::drawImage(const QRectF &r, const QImage &image, const QRectF &sr,
									Qt::ImageConversionFlags flags)
{
	Q_D(QJsonPaintEngine);
#ifdef QT_PICTURE_DEBUG
	qDebug() << " -> drawImage():" << r << sr;
#endif
	QString src="img" + QUuid::createUuid().toString().replace("-", "").replace("{", "");

	image.save(QString("%1.png").arg(src));
	d->s << QString("\t{\"i\": \"s\": \"%1\", \"sx\":%2, \"sy\":%3, \"sw\": %4, \"sh\": %5, \"dx\": %6, \"dy\": %7, \"dw\": %8, \"dh\": %9},\r\n")
			.arg(src)
			.arg(sr.left()).arg(sr.top()).arg(sr.width()).arg(sr.height())
			.arg(r.left()).arg(r.top()).arg(r.width()).arg(r.height());
}

void QJsonPaintEngine::drawTextItem(const QPointF &p , const QTextItem &ti)
{
	Q_D(QJsonPaintEngine);
#ifdef QT_PICTURE_DEBUG
	qDebug() << " -> drawTextItem():" << p << ti.text();
#endif

	updateFont(ti.font());
	d->s << QString("\t{\"t\": {\"t\": \"%1\", \"x\": %2, \"y\": %3}},\r\n").arg(ti.text()).arg(p.x()).arg(p.y());


}

void QJsonPaintEngine::updateState(const QPaintEngineState &state)
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


#ifndef QPAINTENGINE_HTML5_P_H
#define QPAINTENGINE_HTML5_P_H

#ifndef QT_NO_PICTURE

#include <QPaintEngine>

class QHtml5PaintEnginePrivate;
class QBuffer;

class QHtml5PaintEngine : public QPaintEngine
{
	Q_DECLARE_PRIVATE(QHtml5PaintEngine)

	void setCanvasPen();
	void setCanvasFillBrush();

public:
	QHtml5PaintEngine(QIODevice *buffer);
	~QHtml5PaintEngine();

	bool begin(QPaintDevice *pdev);
	bool end();

	void updateState(const QPaintEngineState &state);

	void updatePen(const QPen &pen);
	void updateBrush(const QBrush &brush);
	void updateBrushOrigin(const QPointF &origin);
	void updateFont(const QFont &font);
	void updateBackground(Qt::BGMode bgmode, const QBrush &bgBrush);
	void updateMatrix(const QTransform &matrix);
	void updateClipRegion(const QRegion &region, Qt::ClipOperation op);
	void updateClipPath(const QPainterPath &path, Qt::ClipOperation op);
	void updateRenderHints(QPainter::RenderHints hints);
	void updateCompositionMode(QPainter::CompositionMode cmode);
	void updateClipEnabled(bool enabled);
	void updateOpacity(qreal opacity);

	void drawEllipse(const QRectF &rect);
	void drawPath(const QPainterPath &path);
	void drawPolygon(const QPointF *points, int numPoints, PolygonDrawMode mode);
#ifdef Q_NO_USING_KEYWORD
	inline void drawPolygon(const QPoint *points, int pointCount, PolygonDrawMode mode)
		{ QPaintEngine::drawPolygon(points, pointCount, mode); }
#else
	using QPaintEngine::drawPolygon;
#endif

	void drawPixmap(const QRectF &r, const QPixmap &pm, const QRectF &sr);
	void drawTiledPixmap(const QRectF &r, const QPixmap &pixmap, const QPointF &s);
	void drawImage(const QRectF &r, const QImage &image, const QRectF &sr,
				   Qt::ImageConversionFlags flags = Qt::AutoColor);
	void drawTextItem(const QPointF &p, const QTextItem &ti);

	Type type() const { return Picture; }

protected:
	QHtml5PaintEngine(QHtml5PaintEnginePrivate &dptr);

private:
	Q_DISABLE_COPY(QHtml5PaintEngine)
};

QT_END_NAMESPACE

#endif // QT_NO_PICTURE

#endif // QPAINTENGINE_PIC_P_H

#include "svgview.h"

#include <QtSvg/QSvgRenderer>

#include <QtGui/QWheelEvent>
#include <QtGui/QMouseEvent>
#include <QtWidgets/QGraphicsRectItem>
#include <QtSvgWidgets/QGraphicsSvgItem>
#include <QtGui/QPaintEvent>
#include <QtCore/qmath.h>

//#ifndef QT_NO_OPENGL
//#include <QGLWidget>
//#endif

SvgView::SvgView(QWidget* parent)
	: QGraphicsView(parent)
	, m_renderer(Native)
	, m_svgItem(nullptr)
	, m_backgroundItem(nullptr)
	, m_outlineItem(nullptr)
{
	setScene(new QGraphicsScene(this));
	setTransformationAnchor(AnchorUnderMouse);
	setDragMode(ScrollHandDrag);
	setViewportUpdateMode(FullViewportUpdate);

	// Prepare background check-board pattern
	QPixmap tilePixmap(64, 64);
	tilePixmap.fill(Qt::white);
	QPainter tilePainter(&tilePixmap);
	QColor color(220, 220, 220);
	tilePainter.fillRect(0, 0, 32, 32, color);
	tilePainter.fillRect(32, 32, 32, 32, color);
	tilePainter.end();

	setBackgroundBrush(tilePixmap);
}

void SvgView::drawBackground(QPainter* p, const QRectF&) {
	p->save();
	p->resetTransform();
	p->drawTiledPixmap(viewport()->rect(), backgroundBrush().texture());
	p->restore();
}

QSize SvgView::svgSize() const {
	return m_svgItem ? m_svgItem->boundingRect().size().toSize() : QSize();
}

bool SvgView::openFile(const QString& fileName) {
	QGraphicsScene* s = scene();

	const bool drawBackground = (m_backgroundItem ? m_backgroundItem->isVisible() : false);
	//const bool drawOutline = (m_outlineItem ? m_outlineItem->isVisible() : true);
	const bool drawOutline = false;

	//QScopedPointer<QGraphicsSvgItem> svgItem(new QGraphicsSvgItem(fileName));
	QGraphicsSvgItem* svgItem = new QGraphicsSvgItem(fileName);
	if (!svgItem->renderer()->isValid())
		return false;

	s->clear();
	resetTransform();

	//m_svgItem = svgItem.take();
	m_svgItem = svgItem;
	m_svgItem->setFlags(QGraphicsItem::ItemClipsToShape);
	m_svgItem->setCacheMode(QGraphicsItem::NoCache);
	m_svgItem->setZValue(0);

	m_backgroundItem = new QGraphicsRectItem(m_svgItem->boundingRect());
	m_backgroundItem->setBrush(Qt::white);
	m_backgroundItem->setPen(Qt::NoPen);
	m_backgroundItem->setVisible(drawBackground);
	m_backgroundItem->setZValue(-1);

	m_outlineItem = new QGraphicsRectItem(m_svgItem->boundingRect());
	QPen outline(Qt::black, 2, Qt::DashLine);
	outline.setCosmetic(true);
	m_outlineItem->setPen(outline);
	m_outlineItem->setBrush(Qt::NoBrush);
	m_outlineItem->setVisible(drawOutline);
	m_outlineItem->setZValue(1);

	s->addItem(m_backgroundItem);
	s->addItem(m_svgItem);
	s->addItem(m_outlineItem);

	zoomBy(0.5);
	zoomBy(0.5);
	zoomBy(0.5);
	zoomBy(0.5);

	s->setSceneRect(m_outlineItem->boundingRect().adjusted(-10, -10, 10, 10));
	return true;
}

void SvgView::setRenderer(RendererType type) {
	m_renderer = type;

	if (m_renderer == OpenGL) {
		//#ifndef QT_NO_OPENGL
		//        setViewport(new QGLWidget(QGLFormat(QGL::SampleBuffers)));
		//#endif
	}
	else {
		setViewport(new QWidget);
	}
}

void SvgView::setAntialiasing(bool antialiasing) {
	setRenderHint(QPainter::Antialiasing, antialiasing);
}

void SvgView::setViewBackground(bool enable) {
	if (!m_backgroundItem)
		return;

	m_backgroundItem->setVisible(enable);
}

void SvgView::setViewOutline(bool enable) {
	if (!m_outlineItem)
		return;

	m_outlineItem->setVisible(enable);
}

qreal SvgView::zoomFactor() const {
	return transform().m11();
}

void SvgView::zoomIn() {
	zoomBy(2);
}

void SvgView::zoomOut() {
	zoomBy(0.5);
}

void SvgView::resetZoom() {
	if (!qFuzzyCompare(zoomFactor(), qreal(1))) {
		resetTransform();
		emit zoomChanged();
	}
}

void SvgView::paintEvent(QPaintEvent* event) {
	if (m_renderer == Image) {
		if (m_image.size() != viewport()->size()) {
			m_image = QImage(viewport()->size(), QImage::Format_ARGB32_Premultiplied);
		}

		QPainter imagePainter(&m_image);
		QGraphicsView::render(&imagePainter);
		imagePainter.end();

		QPainter p(viewport());
		p.drawImage(0, 0, m_image);

	}
	else {
		QGraphicsView::paintEvent(event);
	}
}

void SvgView::wheelEvent(QWheelEvent* event) {
	zoomBy(qPow(1.2, event->angleDelta().y() / 240.0));
}

void SvgView::zoomBy(qreal factor) {
	const qreal currentZoom = zoomFactor();
	if ((factor < 1 && currentZoom < 0.1) || (factor > 1 && currentZoom > 10))
		return;
	scale(factor, factor);
	emit zoomChanged();
}

QSvgRenderer* SvgView::renderer() const {
	if (m_svgItem)
		return m_svgItem->renderer();
	return nullptr;
}

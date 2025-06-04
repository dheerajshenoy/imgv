#include "GraphicsView.hpp"

#include <QCursor>

GraphicsView::GraphicsView(QWidget *parent) : QGraphicsView(parent)
{
    setRenderHint(QPainter::Antialiasing, false);
    setDragMode(QGraphicsView::ScrollHandDrag);
    setOptimizationFlags(QGraphicsView::DontSavePainterState);
    setViewportUpdateMode(QGraphicsView::SmartViewportUpdate);
    setTransformationAnchor(QGraphicsView::AnchorUnderMouse); // zoom at cursor position
    setResizeAnchor(QGraphicsView::AnchorUnderMouse);
    setInteractive(true);
    setMouseTracking(true);
    setAcceptDrops(false);
}

void
GraphicsView::wheelEvent(QWheelEvent *event)
{
    if (event->modifiers() & Qt::ControlModifier)
    {
        if (event->angleDelta().y() > 0)
            scale(m_zoomFactor, m_zoomFactor); // Zoom in
        else
            scale(1.0 / m_zoomFactor, 1.0 / m_zoomFactor); // Zoom out
        return;
    }
    QGraphicsView::wheelEvent(event);
}

void
GraphicsView::mousePressEvent(QMouseEvent *e)
{
    QGuiApplication::setOverrideCursor(Qt::CursorShape::ClosedHandCursor);
    QGraphicsView::mousePressEvent(e);
}

void
GraphicsView::mouseReleaseEvent(QMouseEvent *e)
{
    QGraphicsView::mouseReleaseEvent(e);
    QGuiApplication::restoreOverrideCursor();
}

void
GraphicsView::zoomIn() noexcept
{
    scale(m_zoomFactor, m_zoomFactor);
}

void
GraphicsView::zoomOut() noexcept
{
    scale(1.0 / m_zoomFactor, 1.0 / m_zoomFactor);
}

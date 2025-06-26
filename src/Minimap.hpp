#pragma once

#include <QBrush>
#include <QGraphicsPixmapItem>
#include <QVBoxLayout>
#include <QWidget>
#include <qgraphicsitem.h>
#include <qgraphicsscene.h>
#include <qgraphicsview.h>

class Minimap : public QGraphicsView
{
    Q_OBJECT
public:
    Minimap(QGraphicsView *parent = nullptr) : QGraphicsView(parent)
    {
        setScene(m_scene);
        setFixedSize(200, 200);
        setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        // scale(0.5, 0.5);
        m_overlay_rect->setPen(QPen(QColor(255, 0, 0, 100), 5));
        m_overlay_rect->setBrush(QBrush(QColor(255, 0, 0, 75)));
        setFrameShadow(QGraphicsView::Plain);
        setFrameShape(QGraphicsView::NoFrame);
        m_overlay_rect->setZValue(10);

        m_scene->addItem(m_pix_item);
        m_scene->addItem(m_overlay_rect);
    }

    void setPixmap(const QPixmap &pix) noexcept
    {
        qreal dpr = devicePixelRatioF();
        QPixmap scaled_pix = pix;
        if (pix.devicePixelRatioF() != dpr)
        {
            scaled_pix.setDevicePixelRatio(dpr);
        }
        m_pix_item->setPixmap(scaled_pix);
        m_scene->setSceneRect(QRectF(QPointF(0, 0), scaled_pix.size() / dpr));
        fitInView(m_scene->sceneRect(), Qt::KeepAspectRatio);
    }

    inline void setOverlayRect(const QRectF &rect) noexcept
    {
        m_overlay_rect->setRect(rect);
    }

    inline void setOverlayRectColor(const QColor &color) noexcept
    {
        m_overlay_rect->setBrush(color);
    }

    inline bool forceHidden() noexcept
    {
        return m_force_hidden;
    }

    inline void setForceHidden(bool state) noexcept
    {
        m_force_hidden = state;
        if (state)
            setVisible(false);
    }

    inline void setOverlayRectBorderWidth(int width) noexcept
    {
        if (width < 0)
            return;

        QPen p = m_overlay_rect->pen();
        p.setWidth(width);
        m_overlay_rect->setPen(p);
    }

    inline void setOverlayRectBorderColor(const QColor &color) noexcept
    {
        m_overlay_rect->setPen(color);
    }

private:
    bool m_force_hidden{false};
    QGraphicsPixmapItem *m_pix_item{new QGraphicsPixmapItem()};
    QGraphicsScene *m_scene{new QGraphicsScene(this)};
    QGraphicsRectItem *m_overlay_rect{new QGraphicsRectItem()};
};

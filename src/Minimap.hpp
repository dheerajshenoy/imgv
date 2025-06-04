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
        m_overlay_rect->setZValue(10);

        m_scene->addItem(m_pix_item);
        m_scene->addItem(m_overlay_rect);
    }

    void setPixmap(const QPixmap &pix) noexcept
    {
        m_pix_item->setPixmap(pix);
        m_scene->setSceneRect(pix.rect());
        fitInView(m_scene->sceneRect(), Qt::KeepAspectRatio);
    }

    void setOverlayRect(const QRectF &rect) noexcept
    {
        m_overlay_rect->setRect(rect);
    }

private:
    QGraphicsPixmapItem *m_pix_item{new QGraphicsPixmapItem()};
    QGraphicsScene *m_scene{new QGraphicsScene(this)};
    QGraphicsRectItem *m_overlay_rect{new QGraphicsRectItem()};
};

#pragma once

#include <QGraphicsView>
#include <QWheelEvent>

class GraphicsView : public QGraphicsView
{
public:
    GraphicsView(QWidget *parent = nullptr);
    inline void setZoomFactor(float factor) noexcept
    {
        m_zoomFactor = factor;
    }

    void zoomIn() noexcept;
    void zoomOut() noexcept;

protected:
    void wheelEvent(QWheelEvent *e) override;
    void mousePressEvent(QMouseEvent *e) override;
    void mouseReleaseEvent(QMouseEvent *e) override;

private:
    float m_zoomFactor{1.25f};
};

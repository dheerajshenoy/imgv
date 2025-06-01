#pragma once

#include <QGraphicsPixmapItem>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QObject>
#include <QVBoxLayout>
#include <QWidget>
#include <ImageMagick-7/Magick++.h>

class ImageView : public QWidget
{
public:
    ImageView(QWidget *parent = nullptr);

    void openFile(const QString &path) noexcept;
    inline void setDPR(float dpr) noexcept { m_dpr = dpr; render(); }
    void render() noexcept;
    void zoomIn() noexcept;
    void zoomOut() noexcept;
    void rotateClock() noexcept;
    void rotateAnticlock() noexcept;

private:
    float m_dpr{1.0f};
    QImage magickImageToQImage(const std::string &path) noexcept;
    QGraphicsView *m_gview{new QGraphicsView()};
    QGraphicsScene *m_gscene{new QGraphicsScene()};
    QGraphicsPixmapItem *m_pix_item{new QGraphicsPixmapItem()};
    QString m_filepath;
    float m_zoomFactor{1.25};
    int m_rotation{0};
    QPixmap m_pix;
};

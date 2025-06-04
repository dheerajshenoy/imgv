#pragma once

#include "GraphicsView.hpp"
#include "Minimap.hpp"

#include <QMimeData>
#include <QDropEvent>
#include <QDragEnterEvent>
#include <ImageMagick-7/Magick++.h>
#include <QFileInfo>
#include <QGraphicsPixmapItem>
#include <QGraphicsScene>
#include <QMovie>
#include <QObject>
#include <QScrollBar>
#include <QVBoxLayout>
#include <QWidget>

class ImageView : public QWidget
{
    Q_OBJECT
public:
    ImageView(QWidget *parent = nullptr);

    void openFile(const QString &path) noexcept;
    inline void setDPR(float dpr) noexcept
    {
        m_dpr = dpr;
        render();
    }

    void zoomIn() noexcept;
    void zoomOut() noexcept;
    void rotateClock() noexcept;
    void rotateAnticlock() noexcept;
    void fitWidth() noexcept;
    void fitHeight() noexcept;
    void scrollLeft() noexcept;
    void scrollRight() noexcept;
    void scrollUp() noexcept;
    void scrollDown() noexcept;
    QString fileName() noexcept;
    QString baseName() noexcept;
    inline QString filePath() noexcept
    {
        return m_filepath;
    }
    inline QString fileSize() noexcept
    {
        return m_filesize;
    }
    inline QString fileDir() noexcept
    {
        return QFileInfo(m_filepath).absolutePath();
    }

    inline void toggleMinimap() noexcept
    {
        m_minimap->setVisible(!m_minimap->isVisible());
    }

    void updateMinimapPosition() noexcept;

signals:
    void openFilesRequested(const QList<QString> &files);

protected:
    void showEvent(QShowEvent *e) override;
    void hideEvent(QHideEvent *e) override;
    void resizeEvent(QResizeEvent *e) override;
    void dragEnterEvent(QDragEnterEvent *e) override;
    void dropEvent(QDropEvent *e) override;

private:
    void render() noexcept;
    void renderAnimatedImage() noexcept;
    QString humanReadableSize(qint64 bytes) noexcept;
    void updateGifFrame(int frameNumber) noexcept;
    void stopGifAnimation() noexcept;
    void startGifAnimation() noexcept;
    bool hasMoreThanOneFrame() noexcept;
    void updateMinimapRegion() noexcept;

    bool m_isGif{false}, m_isMinimapMode{false};

    float m_dpr{1.0f};
    QImage magickImageToQImage(Magick::Image &image) noexcept;
    GraphicsView *m_gview;
    QGraphicsScene *m_gscene;
    QGraphicsPixmapItem *m_pix_item{new QGraphicsPixmapItem()};
    QString m_filepath, m_filesize;
    float m_zoomFactor{1.25};
    int m_rotation{0};
    QPixmap m_pix;
    QScrollBar *m_vscrollbar, *m_hscrollbar;
    QMovie *m_movie{nullptr};
    Minimap *m_minimap{nullptr};
};

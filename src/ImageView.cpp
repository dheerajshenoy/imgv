#include "ImageView.hpp"
#include "GraphicsView.hpp"

#include <QFileInfo>
#include <QGraphicsProxyWidget>
#include <QtConcurrent/QtConcurrent>
#include <qevent.h>
#include <qscrollbar.h>
#include <qthreadpool.h>

ImageView::ImageView(QWidget *parent) : QWidget(parent)
{
    QVBoxLayout *layout = new QVBoxLayout();
    m_gview             = new GraphicsView();
    m_gscene            = new QGraphicsScene();
    layout->addWidget(m_gview);
    m_gview->setScene(m_gscene);
    m_gscene->addItem(m_pix_item);

    m_minimap = new Minimap(m_gview);
    m_minimap->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
    m_minimap->raise();
    m_minimap->setVisible(true);

    m_gview->setResizeAnchor(QGraphicsView::ViewportAnchor::AnchorUnderMouse);
    m_gview->setTransformationAnchor(QGraphicsView::ViewportAnchor::AnchorViewCenter);
    setLayout(layout);
    m_hscrollbar = m_gview->horizontalScrollBar();
    m_vscrollbar = m_gview->verticalScrollBar();
    m_gview->installEventFilter(this);
    connect(m_hscrollbar, &QScrollBar::valueChanged, this, [&](int /*value */) {
            updateMinimapRegion();
            });

    connect(m_vscrollbar, &QScrollBar::valueChanged, this, [&](int /*value */) {
            updateMinimapRegion();
            });

    connect(m_gview, &GraphicsView::openFilesRequested, this, &ImageView::openFilesRequested);

}

void
ImageView::openFile(const QString &filepath) noexcept
{
    m_filepath           = filepath;
    QFuture<void> future = QtConcurrent::run([&]
    {
        auto bytes  = QFileInfo(m_filepath).size();
        QString str = humanReadableSize(bytes);
        m_filesize  = str;
    });

    m_isGif = false;
    stopGifAnimation();
    render();

    m_minimap->setPixmap(m_pix_item->pixmap());
    m_gview->fitInView(m_pix_item, Qt::KeepAspectRatio);
    m_gview->centerOn(m_pix_item);
}

QImage
ImageView::magickImageToQImage(Magick::Image &image) noexcept
{
    int width  = image.columns();
    int height = image.rows();

    const bool hasAlpha            = image.alpha();
    const std::string format       = hasAlpha ? "RGBA" : "RGB";
    const QImage::Format imgFormat = hasAlpha ? QImage::Format_RGBA8888 : QImage::Format_RGB888;

    const int bytesPerPixel = hasAlpha ? 4 : 3;
    std::vector<unsigned char> buffer(width * height * bytesPerPixel);

    QImage img(width, height, imgFormat);
    image.write(0, 0, width, height, format, Magick::CharPixel, img.bits());

    img.setDevicePixelRatio(m_dpr);

    // Copy to detach from original buffer (Magick might reuse or destroy it)
    return img;
}

void
ImageView::render() noexcept
{
    Magick::Image image;
    image.read(m_filepath.toStdString());

    if (hasMoreThanOneFrame())
        renderAnimatedImage();
    else
    {
        QImage img = magickImageToQImage(image);
        m_pix      = QPixmap::fromImage(img);
        m_pix.setDevicePixelRatio(m_dpr);
        m_pix_item->setPixmap(m_pix);

        int margin    = 100;
        QRectF padded = m_pix_item->boundingRect().adjusted(-margin, -margin, margin, margin);
        m_gview->setSceneRect(padded);
    }
}

void
ImageView::zoomIn() noexcept
{
    m_gview->zoomIn();

    if (m_isMinimapMode)
        updateMinimapRegion();
}

void
ImageView::zoomOut() noexcept
{
    m_gview->zoomOut();
    if (m_isMinimapMode)
        updateMinimapRegion();
}

void
ImageView::rotateClock() noexcept
{
    m_rotation     = (m_rotation + 90) % 360;
    QTransform mat = m_gview->transform();
    mat.rotate(90);
    m_gview->setTransform(mat);
    m_gview->centerOn(m_pix_item);
}

void
ImageView::rotateAnticlock() noexcept
{
    m_rotation     = (m_rotation - 90) % 360;
    QTransform mat = m_gview->transform();
    mat.rotate(-90);
    m_gview->setTransform(mat);
    m_gview->centerOn(m_pix_item);
}

void
ImageView::fitHeight() noexcept
{
    QTransform t;
    t.rotate(m_rotation);
    auto pixHeight  = t.mapRect(m_pix_item->boundingRect()).height();
    auto viewHeight = m_gview->viewport()->rect().height();

    float scaleFactor = viewHeight / pixHeight;

    t.scale(scaleFactor, scaleFactor);
    m_gview->setTransform(t);
    m_gview->centerOn(m_pix_item);
}

void
ImageView::fitWidth() noexcept
{
    QTransform t;
    t.rotate(m_rotation);

    auto pixwidth  = t.mapRect(m_pix_item->boundingRect()).width();
    auto viewwidth = m_gview->viewport()->rect().width();

    float scaleFactor = viewwidth / pixwidth;

    t.scale(scaleFactor, scaleFactor);
    m_gview->setTransform(t);
    m_gview->centerOn(m_pix_item);
}

void
ImageView::scrollLeft() noexcept
{
    m_hscrollbar->setValue(m_hscrollbar->value() - 30);
}

void
ImageView::scrollRight() noexcept
{
    m_hscrollbar->setValue(m_hscrollbar->value() + 30);
}

void
ImageView::scrollUp() noexcept
{
    m_vscrollbar->setValue(m_vscrollbar->value() + 30);
}

void
ImageView::scrollDown() noexcept
{
    m_vscrollbar->setValue(m_vscrollbar->value() - 30);
}

QString
ImageView::fileName() noexcept
{
    return QFileInfo(m_filepath).fileName();
}

QString
ImageView::baseName() noexcept
{
    return QFileInfo(m_filepath).baseName();
}

QString
ImageView::humanReadableSize(qint64 bytes) noexcept
{
    static const char *sizes[] = {"B", "KB", "MB", "GB", "TB"};
    double len                 = bytes;
    int order                  = 0;
    while (len >= 1024 && order < 4)
    {
        order++;
        len /= 1024.0;
    }
    return QString("%1 %2").arg(len, 0, 'f', 2).arg(sizes[order]);
}

void
ImageView::renderAnimatedImage() noexcept
{
    if (m_movie)
    {
        m_movie->stop();
        disconnect(m_movie, &QMovie::frameChanged, this, &ImageView::updateGifFrame);
        delete m_movie;
        m_movie = nullptr;
    }
    m_isGif = true;
    m_movie = new QMovie(m_filepath, QByteArray(), this);
    connect(m_movie, &QMovie::frameChanged, this, &ImageView::updateGifFrame);
    m_movie->start();
}

void
ImageView::updateGifFrame(int /*frameNumber*/) noexcept
{
    if (!m_movie)
        return;

    QPixmap frame = m_movie->currentPixmap();
    m_pix_item->setPixmap(frame);
    m_minimap->setPixmap(frame);
    int margin        = 100;
    QRectF paddedRect = m_pix_item->boundingRect().adjusted(-margin, -margin, margin, margin);
    m_gscene->setSceneRect(paddedRect);
}

void
ImageView::stopGifAnimation() noexcept
{
    if (m_movie)
        m_movie->stop();
}

void
ImageView::startGifAnimation() noexcept
{
    if (m_movie)
        m_movie->start();
}

void
ImageView::showEvent(QShowEvent *e)
{
    if (m_isGif)
    {
        startGifAnimation();
    }

    QWidget::showEvent(e);
}

void
ImageView::hideEvent(QHideEvent *e)
{
    if (m_isGif)
    {
        stopGifAnimation();
    }
    QWidget::hideEvent(e);
}

bool
ImageView::hasMoreThanOneFrame() noexcept
{
    std::list<Magick::Image> frames;

    try
    {
        Magick::pingImages(&frames, m_filepath.toStdString());
        return frames.size() > 1;
    }
    catch (const Magick::Exception &error)
    {
        return false;
    }
}

void
ImageView::updateMinimapRegion() noexcept
{
    QRectF rect = m_gview->mapToScene(m_gview->viewport()->rect()).boundingRect();
    m_minimap->setOverlayRect(rect);
}

void ImageView::updateMinimapPosition() noexcept
{
    m_minimap->move(m_gview->viewport()->width() - m_minimap->width() - 30,
            m_gview->viewport()->height() - m_minimap->height() - 30);
}

void ImageView::resizeEvent(QResizeEvent *e)
{
    updateMinimapPosition();
    QWidget::resizeEvent(e);
}

void
ImageView::dropEvent(QDropEvent *e)
{
    if (e->mimeData()->hasUrls())
    {
        QList<QString> files;
        for (const auto &url : e->mimeData()->urls())
        {
            auto file = url.toLocalFile();
            files.append(file);
        }
        emit openFilesRequested(files);
    }
}

void
ImageView::dragEnterEvent(QDragEnterEvent *e)
{
    if (e->mimeData()->hasUrls() || e->mimeData()->hasText())
        e->acceptProposedAction();
    else
        e->ignore();
}

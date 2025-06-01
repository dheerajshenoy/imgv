#include "ImageView.hpp"

#include <QFileInfo>
#include <QtConcurrent/QtConcurrent>
#include <qthreadpool.h>

ImageView::ImageView(QWidget *parent) : QWidget(parent)
{
    QVBoxLayout *layout = new QVBoxLayout();
    m_gview = new GraphicsView();
    m_gscene = new QGraphicsScene();
    layout->addWidget(m_gview);
    m_gview->setScene(m_gscene);
    m_gscene->addItem(m_pix_item);
    m_gview->setResizeAnchor(QGraphicsView::ViewportAnchor::AnchorUnderMouse);
    m_gview->setTransformationAnchor(QGraphicsView::ViewportAnchor::AnchorViewCenter);
    setLayout(layout);
    m_hscrollbar = m_gview->horizontalScrollBar();
    m_vscrollbar = m_gview->verticalScrollBar();
}

void
ImageView::openFile(const QString &filepath) noexcept
{
    m_filepath = filepath;
    QFuture<void> future = QtConcurrent::run([&] {
            auto bytes = QFileInfo(m_filepath).size();
            QString str = humanReadableSize(bytes);
            m_filesize = str;
            });

    render();
    // m_gview->fitInView(m_pix_item, Qt::KeepAspectRatio);
    // m_gview->centerOn(m_pix_item);


    // Defer fit/center to after layout and rendering
    QTimer::singleShot(0, this, [this]() {
            m_gview->fitInView(m_pix_item, Qt::KeepAspectRatio);
            m_gview->centerOn(m_pix_item);
            });
}

QImage
ImageView::magickImageToQImage(const std::string &file) noexcept
{
    Magick::Image image;
    image.read(file);
    int width  = image.columns();
    int height = image.rows();

    int size = width * height;
    std::string format;
    QImage::Format img_format;

    if (image.alpha())
    {
        size *= 4;
        format     = "RGBA";
        img_format = QImage::Format_RGBA8888;
    }
    else
    {
        size *= 3;
        format     = "RGB";
        img_format = QImage::Format_RGB888;
    }
    std::vector<unsigned char> pixels(size);

    // Export RGBA pixels to buffer
    image.write(0, 0, width, height, format, Magick::CharPixel, pixels.data());

    QImage img(pixels.data(), width, height, img_format);
    img.setDevicePixelRatio(m_dpr);

    // Copy to detach from original buffer (Magick might reuse or destroy it)
    return img.copy();
}

void
ImageView::render() noexcept
{
    QImage img = magickImageToQImage(m_filepath.toStdString());
    m_pix      = QPixmap::fromImage(img);
    m_pix.setDevicePixelRatio(m_dpr);
    m_pix_item->setPixmap(m_pix);
}

void
ImageView::zoomIn() noexcept
{
    m_gview->zoomIn();
}

void
ImageView::zoomOut() noexcept
{
    m_gview->zoomOut();
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


QString ImageView::humanReadableSize(qint64 bytes) noexcept {
    static const char *sizes[] = { "B", "KB", "MB", "GB", "TB" };
    double len = bytes;
    int order = 0;
    while (len >= 1024 && order < 4) {
        order++;
        len /= 1024.0;
    }
    return QString("%1 %2").arg(len, 0, 'f', 2).arg(sizes[order]);
}


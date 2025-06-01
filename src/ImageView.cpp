#include "ImageView.hpp"

ImageView::ImageView(QWidget *parent) : QWidget(parent)
{
    QVBoxLayout *layout = new QVBoxLayout();
    layout->addWidget(m_gview);
    m_gview->setScene(m_gscene);
    m_gscene->addItem(m_pix_item);
    m_gview->setResizeAnchor(QGraphicsView::ViewportAnchor::AnchorUnderMouse);
    m_gview->setTransformationAnchor(QGraphicsView::ViewportAnchor::AnchorViewCenter);
    setLayout(layout);
}

void
ImageView::openFile(const QString &filepath) noexcept
{
    m_filepath = filepath;
    render();
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
        format = "RGBA";
        img_format = QImage::Format_RGBA8888;
    }
    else
    {
        size *= 3;
        format = "RGB";
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

void ImageView::render() noexcept
{
    QImage img = magickImageToQImage(m_filepath.toStdString());
    m_pix = QPixmap::fromImage(img);
    m_pix.setDevicePixelRatio(m_dpr);
    m_pix_item->setPixmap(m_pix);
}

void ImageView::zoomIn() noexcept
{
    m_gview->scale(m_zoomFactor, m_zoomFactor);
}

void ImageView::zoomOut() noexcept
{
    m_gview->scale(1 / m_zoomFactor, 1 / m_zoomFactor);
}

void ImageView::rotateClock() noexcept
{
    QTransform mat = m_gview->transform();
    m_rotation = (m_rotation + 90) % 360;
    mat.rotate(90);
    m_gview->setTransform(mat);
}

void ImageView::rotateAnticlock() noexcept
{
    QTransform mat = m_gview->transform();
    m_rotation = (m_rotation - 90) % 360;
    mat.rotate(-90);
    m_gview->setTransform(mat);
}


#include "Panel.hpp"

#include "ElidableLabel.hpp"

#include <qboxlayout.h>

Panel::Panel(QWidget *parent) : QWidget(parent)
{
    QHBoxLayout *layout = new QHBoxLayout(this);
    m_filename_label    = new ElidableLabel();
    m_filesize_label    = new QLabel();
    m_imgsize_label     = new QLabel();
    layout->addWidget(m_filename_label);
    layout->addWidget(m_imgsize_label);
    layout->addWidget(m_filesize_label);
}

void
Panel::setFileName(const QString &name) noexcept
{
    m_filename_label->setFullText(name);
}

void
Panel::setFileSize(const QString &size) noexcept
{
    m_filesize_label->setText(size);
}

void
Panel::setImageSize(int width, int height) noexcept
{
    QString sizelabel = QString("(W: %1, H: %2)").arg(width).arg(height);
    m_imgsize_label->setText(sizelabel);
}

void
Panel::clear() noexcept
{
    m_filename_label->clear();
    m_imgsize_label->clear();
    m_filesize_label->clear();
}

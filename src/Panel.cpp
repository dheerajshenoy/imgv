#include "Panel.hpp"
#include <qboxlayout.h>

Panel::Panel(QWidget *parent) : QWidget(parent)
{
    QHBoxLayout *layout = new QHBoxLayout(this);
    layout->addWidget(m_filename_label, 1);
    layout->addWidget(m_filesize_label);
}

void Panel::setFileName(const QString &name) noexcept
{
    m_filename_label->setFullText(name);
}

void Panel::setFileSize(const QString &size) noexcept
{
    m_filesize_label->setText(size);
}

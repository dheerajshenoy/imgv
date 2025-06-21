#pragma once

#include "ElidableLabel.hpp"

#include <QHBoxLayout>
#include <QLabel>
#include <QWidget>

class Panel : public QWidget
{
public:
    Panel(QWidget *parent = nullptr);
    void setFileName(const QString &name) noexcept;
    void setFileSize(const QString &size) noexcept;
    void setImageSize(int w, int h) noexcept;
    void clear() noexcept;

private:
    ElidableLabel *m_filename_label{nullptr};
    QLabel *m_filesize_label{nullptr};
    QLabel *m_imgsize_label{nullptr};
};

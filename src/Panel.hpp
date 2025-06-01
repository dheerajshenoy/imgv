#pragma once

#include <QHBoxLayout>
#include <QLabel>
#include <QWidget>
#include "ElidableLabel.hpp"

class Panel : public QWidget
{
public:
    Panel(QWidget *parent = nullptr);
    void setFileName(const QString &name) noexcept;
    void setFileSize(const QString &size) noexcept;

private:
    ElidableLabel *m_filename_label{new ElidableLabel()};
    QLabel *m_filesize_label{new QLabel()};
};

#pragma once

#include <QHBoxLayout>
#include <QLabel>
#include <QWidget>

class Panel : public QWidget
{
public:
    Panel(QWidget *parent = nullptr);
    void setFileName(const QString &name) noexcept;
    void setFileSize(const QString &size) noexcept;

private:
    QLabel *m_filename_label{new QLabel("FILENAME")};
    QLabel *m_filesize_label{new QLabel("FILESIZE")};
};

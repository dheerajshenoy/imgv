#pragma once

#include <QTabWidget>

class TabWidget : public QTabWidget
{
    Q_OBJECT
public:
    TabWidget(QWidget *parent = nullptr);

signals:
    void fileOpenRequested(const QList<QString> &filepaths);
};

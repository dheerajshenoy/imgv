#pragma once

#include <QTabWidget>
#include <qevent.h>

class TabWidget : public QTabWidget
{
public:
    TabWidget(QWidget *parent = nullptr);

signals:
    void fileOpenRequested(const QString &filepath);

protected:
    void dragEnterEvent(QDragEnterEvent *e) override;
    void dropEvent(QDropEvent *e) override;

private:
};

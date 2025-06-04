#pragma once

#include <QTabWidget>
#include <qevent.h>

class TabWidget : public QTabWidget
{
    Q_OBJECT
public:
    TabWidget(QWidget *parent = nullptr);

signals:
    void fileOpenRequested(const QList<QString> &filepaths);

protected:
    void dragEnterEvent(QDragEnterEvent *e) override;
    void dropEvent(QDropEvent *e) override;

private:
};

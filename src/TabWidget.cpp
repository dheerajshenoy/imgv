#include "TabWidget.hpp"

#include <QMimeData>
#include <qevent.h>

TabWidget::TabWidget(QWidget *parent) : QTabWidget(parent)
{
    setStyleSheet(R"(
    QTabbar::pane {
    padding: 0px;
    margin: 0px;
    }
    )");
    setTabsClosable(true);
    setTabBarAutoHide(true);
    setDocumentMode(true);
    setMovable(true);
    setElideMode(Qt::TextElideMode::ElideMiddle);
    setAcceptDrops(true);
}

void
TabWidget::dropEvent(QDropEvent *e)
{
    if (e->mimeData()->hasUrls() || e->mimeData()->hasText())
        e->acceptProposedAction();
    else
        e->ignore();
}

void
TabWidget::dragEnterEvent(QDragEnterEvent *e)
{
    if (e->mimeData()->hasUrls())
    {
        QList<QString> files;
        for(const auto &url : e->mimeData()->urls())
        {
            auto file = url.toLocalFile();
            files.append(file);
        }
    }
}

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


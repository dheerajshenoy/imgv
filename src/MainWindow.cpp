#include "MainWindow.hpp"

#include "ImageView.hpp"

#include <QFileDialog>
#include <QKeySequence>
#include <QScreen>
#include <QShortcut>
#include <QWindow>
#include <qnamespace.h>
#include <qshortcut.h>
#include <QTabBar>
#include <QDesktopServices>

void
MainWindow::readArgs(argparse::ArgumentParser &parser) noexcept
{
    this->construct();
    if (parser.is_used("files"))
    {
        auto files = parser.get<std::vector<std::string>>("files");
        for (const auto &file : files)
        {
            OpenFile(QString::fromStdString(file));
        }
    }
}

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent)
{
    setAttribute(Qt::WA_NativeWindow);
    Magick::InitializeMagick(nullptr);
}

void
MainWindow::construct() noexcept
{
    QVBoxLayout *layout = new QVBoxLayout();

    layout->addWidget(m_tab_widget);
    layout->addWidget(m_panel);
    QWidget *widget = new QWidget();
    widget->setLayout(layout);
    setCentralWidget(widget);
    m_tab_widget->setTabsClosable(true);
    m_tab_widget->setTabBarAutoHide(true);
    m_tab_widget->setDocumentMode(true);
    // m_tab_widget->setTabPosition(QTabWidget::TabPosition::West);
    m_tab_widget->setElideMode(Qt::TextElideMode::ElideMiddle);

    initConnections();
    initKeybinds();
    show();

    this->setContentsMargins(0, 0, 0, 0);
    layout->setContentsMargins(0, 0, 0, 0);
    widget->setContentsMargins(0, 0, 0, 0);
    m_panel->setContentsMargins(0, 0, 0, 0);
    m_panel->layout()->setContentsMargins(0, 5, 0, 5);
}

void
MainWindow::initKeybinds() noexcept
{
    m_commandMap["open_file"] = [this]()
    {
        OpenFile();
    };

    m_commandMap["close_file"] = [this]()
    {
        CloseFile();
    };

    m_commandMap["toggle_tabs"] = [this]()
    {
        auto tabbar = m_tab_widget->tabBar();
        tabbar->setVisible(!tabbar->isVisible());
    };

    m_commandMap["open_file_location"] = [this]()
    {
        QString filedir = qobject_cast<ImageView*>(m_tab_widget->currentWidget())->fileDir();
        QDesktopServices::openUrl(QUrl(filedir));
    };

    m_commandMap["zoom_in"] = [this]()
    {
        ZoomIn();
    };
    m_commandMap["zoom_out"] = [this]()
    {
        ZoomOut();
    };
    m_commandMap["rotate_clock"] = [this]()
    {
        RotateClock();
    };
    m_commandMap["rotate_anticlock"] = [this]()
    {
        RotateAnticlock();
    };
    m_commandMap["fit_width"] = [this]()
    {
        FitWidth();
    };
    m_commandMap["fit_height"] = [this]()
    {
        FitHeight();
    };

    m_commandMap["scroll_left"] = [this]()
    {
        Scroll(ScrollDirection::LEFT);
    };

    m_commandMap["scroll_down"] = [this]()
    {
        Scroll(ScrollDirection::DOWN);
    };

    m_commandMap["scroll_up"] = [this]()
    {
        Scroll(ScrollDirection::UP);
    };

    m_commandMap["scroll_right"] = [this]()
    {
        Scroll(ScrollDirection::RIGHT);
    };

    m_shortcutMap["Ctrl+W"] = "close_file";
    m_shortcutMap["o"] = "open_file";
    m_shortcutMap["q"] = "open_file_location";
    m_shortcutMap["="] = "zoom_in";
    m_shortcutMap["-"] = "zoom_out";
    m_shortcutMap[">"] = "rotate_clock";
    m_shortcutMap["<"] = "rotate_anticlock";
    m_shortcutMap["1"] = "fit_width";
    m_shortcutMap["2"] = "fit_height";
    m_shortcutMap["h"] = "scroll_left";
    m_shortcutMap["j"] = "scroll_down";
    m_shortcutMap["k"] = "scroll_up";
    m_shortcutMap["l"] = "scroll_right";
    m_shortcutMap["t"] = "toggle_tabs";

    for (auto iter = m_shortcutMap.begin(); iter != m_shortcutMap.end(); iter++)
    {
        QShortcut *shortcut = new QShortcut(QKeySequence(iter.key()), this);
        connect(shortcut, &QShortcut::activated, this, [this, iter]() { m_commandMap[iter.value()](); });
    }
}

void
MainWindow::initConnections() noexcept
{
    QWindow *win = this->windowHandle();
    if (win)
    {
        connect(win, &QWindow::screenChanged, this, [&](QScreen *screen)
        {
            m_dpr = this->devicePixelRatioF();
            if (m_imgv)
                m_imgv->setDPR(m_dpr);
        });
    }

    connect(m_tab_widget, &QTabWidget::currentChanged, [&](int index) {
            m_imgv = qobject_cast<ImageView*>(m_tab_widget->widget(index));
            if (m_imgv)
            {
            m_panel->setFileName(m_imgv->fileName());
            m_panel->setFileSize(m_imgv->fileSize());
            }
            });

    connect(m_tab_widget, &QTabWidget::tabCloseRequested, this, &MainWindow::handleTabClose);
}

void MainWindow::handleTabClose(int index) noexcept
{
    auto widget = m_tab_widget->widget(index);
    if (!widget)
        return;
    widget->close();
    widget->deleteLater();
    m_tab_widget->removeTab(index);
}

void
MainWindow::OpenFile(QString filepath) noexcept
{
    if (filepath.isEmpty())
    {
        filepath = QFileDialog::getOpenFileName(this, "Open File");
        if (filepath.isEmpty())
            return;
    }

    filepath = filepath.replace("~", getenv("HOME"));

    m_imgv = new ImageView(m_tab_widget);
    m_imgv->openFile(filepath);
    m_tab_widget->addTab(m_imgv, filepath);

    m_tab_widget->setCurrentWidget(m_imgv);  // Make it the active tab
}

void
MainWindow::CloseFile() noexcept
{
    if (m_tab_widget->currentIndex() >= 0)
        m_tab_widget->tabCloseRequested(m_tab_widget->currentIndex());  // Make it the active tab
}

void
MainWindow::ZoomIn() noexcept
{
    m_imgv->zoomIn();
}

void
MainWindow::ZoomOut() noexcept
{
    if (m_imgv)
        m_imgv->zoomOut();
}

void
MainWindow::RotateClock() noexcept
{
    if (m_imgv)
        m_imgv->rotateClock();

}

void
MainWindow::RotateAnticlock() noexcept
{
    if (m_imgv)
        m_imgv->rotateAnticlock();
}

void
MainWindow::FitHeight() noexcept
{
    if (m_imgv)
        m_imgv->fitHeight();
}

void
MainWindow::FitWidth() noexcept
{
    if (m_imgv)
        m_imgv->fitWidth();
}

void
MainWindow::Scroll(ScrollDirection dir) noexcept
{
    switch (dir)
    {
        case ScrollDirection::LEFT:
            m_imgv->scrollLeft();
            break;

        case ScrollDirection::RIGHT:
            m_imgv->scrollRight();
            break;

        case ScrollDirection::UP:
            m_imgv->scrollUp();
            break;

        case ScrollDirection::DOWN:
            m_imgv->scrollDown();
            break;
    }
}

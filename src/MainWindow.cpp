#include "MainWindow.hpp"
#include "ImageView.hpp"
#include <QFileDialog>
#include <QShortcut>
#include <QKeySequence>
#include <qshortcut.h>
#include <QWindow>
#include <QScreen>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent)
{
    QVBoxLayout *layout = new QVBoxLayout();

    layout->addWidget(m_tab_widget);
    layout->addWidget(m_panel);
    QWidget *widget = new QWidget();
    widget->setLayout(layout);
    setCentralWidget(widget);

    initConnections();
    initKeybinds();
}

void MainWindow::initKeybinds() noexcept
{
    m_commandMap["open"] = [this]() { OpenFile(); };
    m_commandMap["zoom_in"] = [this]() { ZoomIn(); };
    m_commandMap["zoom_out"] = [this]() { ZoomOut(); };
    m_commandMap["rotate_clock"] = [this]() { RotateClock(); };
    m_commandMap["rotate_anticlock"] = [this]() { RotateAnticlock(); };

    m_shortcutMap["o"] = "open";
    m_shortcutMap["="] = "zoom_in";
    m_shortcutMap["-"] = "zoom_out";
    m_shortcutMap[">"] = "rotate_clock";
    m_shortcutMap["<"] = "rotate_anticlock";


    for (auto iter = m_shortcutMap.begin(); iter != m_shortcutMap.end(); iter++)
    {
        QShortcut *shortcut = new QShortcut(QKeySequence(iter.key()), this);
        connect(shortcut, &QShortcut::activated, this, [this, iter]() { m_commandMap[iter.value()](); });
    }
}

void MainWindow::initConnections() noexcept
{
    QWindow *win = this->windowHandle();
    if (win)
    {
        connect(win, &QWindow::screenChanged, this, [&](QScreen *screen) {
                m_imgv->setDPR(this->devicePixelRatioF());
                });
    }
}

void MainWindow::OpenFile() noexcept
{
    auto filename = QFileDialog::getOpenFileName(this, "Open File");
    if (filename.isEmpty())
        return;

    m_imgv = new ImageView(m_tab_widget);
    m_imgv->openFile(filename);
    m_tab_widget->addTab(m_imgv, filename);
}

void MainWindow::ZoomIn() noexcept
{
    if (m_imgv)
    {
        m_imgv->zoomIn();
    }
}

void MainWindow::ZoomOut() noexcept
{
    if (m_imgv)
    {
        m_imgv->zoomOut();
    }
}

void MainWindow::RotateClock() noexcept
{
    if (m_imgv)
    {
        m_imgv->rotateClock();
    }
}

void MainWindow::RotateAnticlock() noexcept
{
    if (m_imgv)
    {
        m_imgv->rotateAnticlock();
    }
}

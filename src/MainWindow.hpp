#pragma once

#include "Panel.hpp"
#include "argparse.hpp"
#include "TabWidget.hpp"

#include <QMimeData>
#include <qevent.h>
#include <QApplication>
#include <QMainWindow>
#include <QTabWidget>
#include <QVBoxLayout>
#include <QWidget>

#define __IMGV_VERSION "0.1.0"

class ImageView;

class MainWindow : public QMainWindow
{
public:
    MainWindow(QWidget *parent = nullptr);

    enum class ScrollDirection
    {
        LEFT = 0,
        RIGHT,
        UP,
        DOWN
    };

    void construct() noexcept;
    void readArgs(argparse::ArgumentParser &parser) noexcept;
    void initKeybinds() noexcept;
    void initConnections() noexcept;
    void handleFileDrop() noexcept;

    void OpenFile(QString filepath = QString()) noexcept;
    void OpenFiles(const QList<QString> &files) noexcept;
    void CloseFile() noexcept;
    void ZoomIn() noexcept;
    void ZoomOut() noexcept;
    void RotateClock() noexcept;
    void RotateAnticlock() noexcept;
    void FitWidth() noexcept;
    void FitHeight() noexcept;
    void Scroll(ScrollDirection dir) noexcept;
    void ToggleMinimap() noexcept;

protected:
    void dragEnterEvent(QDragEnterEvent *e) override;
    void dropEvent(QDropEvent *e) override;

private:
    void handleTabClose(int index) noexcept;

    TabWidget *m_tab_widget{new TabWidget()};
    Panel *m_panel{new Panel()};
    ImageView *m_imgv{nullptr};
    QMap<QString, std::function<void()>> m_commandMap;
    QMap<QString, QString> m_shortcutMap;
    float m_dpr{1.0f};
};

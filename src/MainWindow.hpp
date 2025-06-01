#pragma once

#include <QApplication>
#include <QMainWindow>
#include <QVBoxLayout>
#include <QTabWidget>
#include <QWidget>
#include "Panel.hpp"

class ImageView;

class MainWindow : public QMainWindow
{
public:
    MainWindow(QWidget *parent = nullptr);

    void initKeybinds() noexcept;
    void initConnections() noexcept;

    void OpenFile() noexcept;
    void CloseFile() noexcept;
    void ZoomIn() noexcept;
    void ZoomOut() noexcept;
    void RotateClock() noexcept;
    void RotateAnticlock() noexcept;

private:
    QTabWidget *m_tab_widget{new QTabWidget()};
    Panel *m_panel{new Panel()};
    ImageView *m_imgv{nullptr};
    QMap<QString, std::function<void()>> m_commandMap;
    QMap<QString, QString> m_shortcutMap;
};

#ifndef IMGV_HPP
#define IMGV_HPP

#include <argparse/argparse.hpp>
#include <qt6/QtWidgets/QApplication>
#include <qt6/QtWidgets/QMessageBox>
#include <qt6/QtWidgets/QInputDialog>
#include <qt6/QtWidgets/QMainWindow>
#include <qt6/QtWidgets/QSplitter>
#include <qt6/QtWidgets/QMenuBar>
#include <qt6/QtGui/QAction>
#include <qt6/QtGui/QShortcut>
#include <qt6/QtGui/QKeySequence>
#include <qt6/QtCore/QTimer>
#include <qt6/QtCore/QDateTime>
#include <qt6/QtCore/QStandardPaths>
#include <qt6/QtCore/QProcess>
#include <qt6/QtCore/QString>
#include <qt6/QtWidgets/QFileDialog>
#include <qt6/QtWidgets/QGraphicsView>
#include <qt6/QtWidgets/QGraphicsScene>
#include <qt6/QtWidgets/QGraphicsPixmapItem>
#include <qt6/QtWidgets/QVBoxLayout>
#include <qt6/QtWidgets/QWidget>
#include "ThumbnailWidget.hpp"
#include "ImageWidget.hpp"
#include "StatusBar.hpp"
#include "sol/sol.hpp"

const std::string APP_VERSION = "1.0";

class IMGV : public QMainWindow
{
public:
    IMGV(argparse::ArgumentParser &parser, QWidget *parent = nullptr);
    ~IMGV(){}

private:
    void initConfigDirectory();
    void initMenu();
    void initConnections();
    void initKeybinds();
    void openImage();
    void slideShow();
    void saveSession();
    void openImageInNewWindow();
    void parseCommandLineArguments(argparse::ArgumentParser &);
    QStringList getSessionFiles();
    void readSessionFile(QString filepath);

    ThumbnailWidget *m_thumbnail_widget = new ThumbnailWidget();
    ImageWidget *m_img_widget = new ImageWidget();
    StatusBar *m_statusbar = new StatusBar();

    bool m_slideshow_mode = false;

    QTimer *m_slideshow_timer = new QTimer();

    QMenuBar *m_menuBar;

    QString m_config_dir_path, m_sessions_dir_path;
    sol::state m_lua_state;
};


#endif

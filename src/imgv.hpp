#ifndef IMGV_HPP
#define IMGV_HPP

#include <argparse/argparse.hpp>
#include <qt6/QtWidgets/QApplication>
#include <qt6/QtWidgets/QMessageBox>
#include <qt6/QtWidgets/QTextEdit>
#include <qt6/QtWidgets/QLineEdit>
#include <qt6/QtWidgets/QInputDialog>
#include <qt6/QtWidgets/QMainWindow>
#include <qt6/QtWidgets/QSplitter>
#include <qt6/QtWidgets/QMenuBar>
#include <qt6/QtGui/QAction>
#include <qt6/QtGui/QPixmapCache>
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
#include "ImageWidget.hpp"
#include "StatusBar.hpp"
#include "sol/sol.hpp"
#include "AboutDialog.hpp"
#include "exif.hpp"
#include "ThumbnailView.hpp"
#include "ThumbnailModel.hpp"
#include "rapidjson/rapidjson.h"
#include "rapidjson/document.h"
#include "rapidjson/stringbuffer.h"
#include "rapidjson/writer.h"
#include "rapidjson/reader.h"
#include "rapidjson/istreamwrapper.h"
#include <fstream>
#include "NoteWidget.hpp"

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
    void openSessionInNewWindow(QString &);
    void fullScreen();
    void maximizeImage(bool);
    void closeSession();
    void openSession(QString &);
    void newSession() noexcept;
    void addNote() noexcept;
    void ThumbSearchTextChanged(QString) noexcept;

    ThumbnailView *m_thumbnail_view  = new ThumbnailView();
    ImageWidget *m_img_widget = new ImageWidget();
    StatusBar *m_statusbar = new StatusBar();

    bool m_slideshow_mode = false;
    QTimer *m_slideshow_timer;

    QString m_config_dir_path, m_sessions_dir_path;
    sol::state m_lua_state;

    QMenuBar *m_menuBar = new QMenuBar();
    QMenu *fileMenu = m_menuBar->addMenu("&File");
    QMenu *editMenu = m_menuBar->addMenu("&Edit");
    QMenu *toolsMenu = m_menuBar->addMenu("&Tools");
    QMenu *viewMenu = m_menuBar->addMenu("&View");
    QMenu *helpMenu = m_menuBar->addMenu("&Help");

    QAction *file__newSession = new QAction(QIcon(":/icons/new-session.svg"), "New Session");
    QAction *file__openAction = new QAction(QIcon(":/icons/open-file.svg"), "Open");
    QMenu *file__openRecent = new QMenu("Open Recent Files");
    QMenu *file__openSession = new QMenu("Open Session");
    QAction *file__openNewWindowAction = new QAction("Open in new &Window");
    QAction *file__saveSession = new QAction(QIcon(":/icons/save-session.svg"), "Save session");
    QAction *file__closeSession = new QAction(QIcon(":/icons/close-session.svg"), "Close session");
    QAction *file__exit = new QAction(QIcon(":/icons/exit.svg"), "Exit");

    QAction *help__about = new QAction("About");

    QMenu *edit__rotate = new QMenu("Rotate");
    QMenu *edit__flip = new QMenu("Flip");

    QAction *flip__vertical = new QAction(QIcon(":/icons/vertical-flip.svg"), "Vertical");
    QAction *flip__horizontal = new QAction(QIcon(":/icons/horizontal-flip.svg"), "Horizontal");

    QAction *rotate__clockwise = new QAction(QIcon(":/icons/clockwise.svg"), "Clockwise");
    QAction *rotate__anticlockwise = new QAction(QIcon(":/icons/anticlockwise.svg"), "Anti Clockwise");
    QAction *rotate__custom = new QAction("Custom");
    QAction *rotate__reset = new QAction("Reset");

    QAction *view__thumbnails = new QAction(QIcon(":/icons/thumbnail.svg"), "Thumbnail Panel");
    QAction *view__statusbar = new QAction(QIcon(":/icons/statusbar.svg"), "Statusbar");
    QAction *view__menubar = new QAction(QIcon(":/icons/menubar.svg"), "Menubar");
    QAction *view__maximize_image = new QAction(QIcon(":/icons/maximize.svg"), "Maximize Image");

    QAction *tools__manage_sessions = new QAction("Manage Sessions");

    QString m_session_name;
    QString m_session_date;
    bool m_image_maximize_mode = false;

    ThumbnailModel *m_thumbnail_model = new ThumbnailModel();
    QLineEdit *m_thumbnail_search_edit = new QLineEdit();

    QWidget *m_right_pane = new QWidget();
    QVBoxLayout *m_right_pane_layout = new QVBoxLayout();
    NoteWidget *m_note_holder = new NoteWidget();
    QSplitter *m_right_pane_splitter = new QSplitter(Qt::Orientation::Vertical);
    QPushButton *m_note_save_btn = new QPushButton("Save Note");
};


#endif

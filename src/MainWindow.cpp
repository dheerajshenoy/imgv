#include "MainWindow.hpp"

#include "ImageView.hpp"

#include <QDesktopServices>
#include <QFileDialog>
#include <QKeySequence>
#include <QScreen>
#include <QShortcut>
#include <QTabBar>
#include <QWindow>
#include <qnamespace.h>
#include <qshortcut.h>

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
    QCoreApplication::setAttribute(Qt::AA_UseHighDpiPixmaps);
    setMinimumSize(600, 400);
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

    initConnections();
    initKeybinds();
    show();

    this->setContentsMargins(0, 0, 0, 0);
    layout->setContentsMargins(0, 0, 0, 0);
    widget->setContentsMargins(0, 0, 0, 0);
    m_panel->setContentsMargins(0, 0, 0, 0);
    m_panel->layout()->setContentsMargins(5, 0, 5, 0);
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
        QTabBar *tabbar = m_tab_widget->tabBar();
        tabbar->setVisible(!tabbar->isVisible());
    };

    m_commandMap["open_file_location"] = [this]()
    {
        QString filedir = qobject_cast<ImageView *>(m_tab_widget->currentWidget())->fileDir();
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

    m_commandMap["toggle_minimap"] = [this]()
    {
        ToggleMinimap();
    };

    m_shortcutMap["m"]      = "toggle_minimap";
    m_shortcutMap["Ctrl+W"] = "close_file";
    m_shortcutMap["o"]      = "open_file";
    m_shortcutMap["q"]      = "open_file_location";
    m_shortcutMap["="]      = "zoom_in";
    m_shortcutMap["-"]      = "zoom_out";
    m_shortcutMap[">"]      = "rotate_clock";
    m_shortcutMap["<"]      = "rotate_anticlock";
    m_shortcutMap["1"]      = "fit_width";
    m_shortcutMap["2"]      = "fit_height";
    m_shortcutMap["h"]      = "scroll_left";
    m_shortcutMap["j"]      = "scroll_down";
    m_shortcutMap["k"]      = "scroll_up";
    m_shortcutMap["l"]      = "scroll_right";
    m_shortcutMap["t"]      = "toggle_tabs";

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
    QGuiApplication::setHighDpiScaleFactorRoundingPolicy(Qt::HighDpiScaleFactorRoundingPolicy::Unset);
    m_dpr = this->devicePixelRatioF();
    if (win)
    {
        connect(win, &QWindow::screenChanged, this, [&](QScreen *screen)
        {
            m_dpr = this->devicePixelRatioF();
            if (m_imgv)
                m_imgv->setDPR(m_dpr);
        });
    }

    connect(m_tab_widget, &TabWidget::currentChanged, [&](int index)
    {
        m_imgv = qobject_cast<ImageView *>(m_tab_widget->widget(index));
        if (m_imgv)
        {
            QString filepath = m_imgv->filePath();
            QSize size       = m_imgv->size();
            m_panel->setFileName(filepath);
            m_panel->setImageSize(size.width(), size.height());
            m_panel->setFileSize(m_imgv->fileSize());
            m_imgv->updateMinimapPosition();
            this->setWindowTitle(QString("iv: %1").arg(filepath));
        }
    });

    connect(m_tab_widget, &TabWidget::tabCloseRequested, this, &MainWindow::handleTabClose);
    connect(m_tab_widget, &TabWidget::fileOpenRequested, this, &MainWindow::OpenFiles);
}

void
MainWindow::handleTabClose(int index) noexcept
{
    m_panel->clear();
    QWidget *widget = m_tab_widget->widget(index);
    if (!widget)
        return;
    widget->close();
    widget->deleteLater();
    m_tab_widget->removeTab(index);
}

void
MainWindow::OpenFiles(const QList<QString> &files) noexcept
{
    for (const QString &filepath : files)
        OpenFile(filepath);
}

void
MainWindow::OpenFile(const QString &filepath) noexcept
{
    QString fp = filepath;

    if (fp.isEmpty())
    {

        QStringList extensions = {"*.jpg",  "*.bmp",  "*.cgm", "*.dpx",  "*.emf", "*.exr",  "*.fits", "*.gif",
                                  "*.heic", "*.heif", "*.jp2", "*.jpeg", "*.jxl", "*.pcx",  "*.png",  "*.psd",
                                  "*.sgi",  "*.svg",  "*.tga", "*.tiff", "*.ico", "*.webp", "*.wmf",  "*.xbm",
                                  "*.cr2",  "*.crw",  "*.dds", "*.eps",  "*.raf", "*.jng",  "*.dcr",  "*.mrw",
                                  "*.nef",  "*.orf",  "*.pef", "*.pict", "*.pnm", "*.pbm",  "*.pgm",  "*.ppm",
                                  "*.rgb",  "*.arw",  "*.srf", "*.sr2",  "*.xcf", "*.xpm"};

        QString filter = QString("Image Files (%1);;All Files (*)").arg(extensions.join(' '));

        QStringList filepaths = QFileDialog::getOpenFileNames(this, "Open File", QString(), filter);

        if (filepaths.isEmpty())
            return;

        OpenFiles(filepaths);
        return;
    }

    if (fp.startsWith("~"))
        fp = fp.replace(0, 1, QString::fromLocal8Bit(getenv("HOME")));

    m_imgv = new ImageView(m_tab_widget);
    m_imgv->openFile(filepath);
    m_tab_widget->addTab(m_imgv, fp);
    m_tab_widget->setCurrentWidget(m_imgv); // Make it the active tab
    connect(m_imgv, &ImageView::openFilesRequested, this, &MainWindow::OpenFiles);
}

void
MainWindow::CloseFile() noexcept
{
    if (m_tab_widget->currentIndex() >= 0)
        m_tab_widget->tabCloseRequested(m_tab_widget->currentIndex()); // Make it the active tab
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

void
MainWindow::handleFileDrop() noexcept
{
}

void
MainWindow::ToggleMinimap() noexcept
{
    m_imgv->toggleMinimap();
}

void
MainWindow::dropEvent(QDropEvent *e)
{
    qDebug() << "DD";
    if (e->mimeData()->hasUrls())
    {
        QList<QString> files;
        for (const QUrl &url : e->mimeData()->urls())
        {
            QString file = url.toLocalFile();
            files.append(file);
        }
        OpenFiles(files);
    }
}

void
MainWindow::dragEnterEvent(QDragEnterEvent *e)
{
    if (e->mimeData()->hasUrls() || e->mimeData()->hasText())
        e->acceptProposedAction();
    else
        e->ignore();
}

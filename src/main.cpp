#include "MainWindow.hpp"
#include "argparse.hpp"

int
main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    MainWindow mw;

    argparse::ArgumentParser program("iv", __IV_VERSION);
    program.add_argument("files").remaining();

    try {
    program.parse_args(argc, argv);
    } catch (const std::exception &e) {
        qDebug() << e.what();
    }

    mw.readArgs(program);
    app.exec();
}

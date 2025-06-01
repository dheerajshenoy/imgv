#include "MainWindow.hpp"
#include "argparse.hpp"

argparse::ArgumentParser initArgs(int argc, char *argv[])
{
}

int
main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    MainWindow mw;

    argparse::ArgumentParser program("imgv", __IMGV_VERSION);
    program.add_argument("files")
        .remaining();
    program.parse_args(argc, argv);

    mw.readArgs(program);
    app.exec();
}

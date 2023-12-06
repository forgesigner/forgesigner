#include "MainWindow.h"

#include <QApplication>
#include <QtLogging>

int main(int argc, char* argv[]) {
    QApplication app(argc, argv);
    const auto argList = QApplication::arguments();
    if (argList.size() < 2) {
        qFatal() << "please provide path to the PDF document";
        app.exit(1);
    }
    MainWindow mainWindow(argList[1]);
    mainWindow.show();

    return QApplication::exec();
}

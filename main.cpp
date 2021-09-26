
#include "src/MainWindow.h"

#include <QtWidgets>

void myMessageOutput(QtMsgType type, const QMessageLogContext &context, const QString &msg)
{
    QByteArray localMsg = msg.toLocal8Bit();
    switch (type) {
        case QtDebugMsg:
            fprintf(stdout, "Debug: %s\n", localMsg.constData());
            break;
        case QtInfoMsg:
            fprintf(stdout, "Info: %s\n", localMsg.constData());
            break;
        case QtWarningMsg:
            fprintf(stderr, "Warning: %s\n", localMsg.constData());
            break;
        case QtCriticalMsg:
            fprintf(stderr, "Critical: %s\n", localMsg.constData());
            break;
        case QtFatalMsg:
            fprintf(stderr, "Fatal: %s\n", localMsg.constData());
            break;
    }
}


int main(int argc, char * argv[])
{
    // TODO: changing up cmakelists broke this
//    Q_INIT_RESOURCE(icons);
    qInstallMessageHandler(myMessageOutput);
    QApplication app(argc, argv);

    QCoreApplication::setOrganizationName("Beathan Andersen");
    QCoreApplication::setApplicationName("Budget Tracker");

    MainWindow win;
    win.show();

    return app.exec();
}

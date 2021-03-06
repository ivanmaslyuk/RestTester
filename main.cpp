#include "mainwindow.h"

#include <QApplication>
#include <QFontDatabase>
#include <QStyleFactory>
#include <QFile>
#include <QDebug>

#include "serverapi.h"

int main(int argc, char *argv[])
{
    QApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QApplication::setDesktopSettingsAware(false);

    QApplication a(argc, argv);

    // Set application fonts
    QFontDatabase::addApplicationFont(":/fonts/Roboto-Bold.ttf");
    int fontId = QFontDatabase::addApplicationFont(":/fonts/Roboto-Regular.ttf");
    QString family = QFontDatabase::applicationFontFamilies(fontId).at(0);
    QFont robotoRegular(family);
    a.setFont(robotoRegular);

    // Set application style
    QFile styleSheetFile(":/styles/global.css");
    styleSheetFile.open(QFile::ReadOnly);
    a.setStyleSheet(QString::fromUtf8(styleSheetFile.readAll()));

    ServerAPI api;
    try {
        auto apis = api.getApisListForCurrentUser();
    }
    catch (const QString &error) {
        qDebug() << error;
    }

    MainWindow w;
    w.show();
    return a.exec();
}

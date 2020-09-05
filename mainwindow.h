#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkReply>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    QNetworkAccessManager *networkAccessManager;

    void makeRequest();

private slots:
    void makeRequestButtonPressed();
    void requestFinished(QNetworkReply *reply);
    void paramsChanged(QString &newUrlencoded);
    void urlEdited(const QString &newUrl);
};
#endif // MAINWINDOW_H

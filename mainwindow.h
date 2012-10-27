#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtGui/QStatusBar>
#include <QtGui/QLabel>
#include <QtSql/QSqlDatabase>

struct Options
{
    bool f5;
};

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    static MainWindow* Instance();
    QStatusBar* getStatusBar();
    bool isTestClassique();
    bool isSensFr();
    void setDefault();
    void closeEvent(QCloseEvent *event);
    Options opt;
private:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    Ui::MainWindow *ui;
    static MainWindow* instance;
public slots:
    void openTest();
    void openResult();
    void openIntro();
    void openOpt();
    void openAbout();
    void openModif();
    void openRenum();
    void openSuppr();
    void openImpr();
    void openAide();
};

#endif // MAINWINDOW_H

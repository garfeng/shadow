#ifndef SHADOW_H
#define SHADOW_H
//using QProgress instead
//#include <Windows.h>
//#include <iostream>
#include <QProcess>
#include <QMainWindow>
#include <QFileDialog>
#include <QMessageBox>
#include <QDesktopServices>
#include <QJsonDocument>
#include <QJsonObject>

#define LX "lx="
#define LY "ly="
#define WALL "wall="
#define ATTEN "atten="
#define FN "fn="
#define OUTPUT "out="
#define PY "pd="
#define SCALE "scale="
#define ORIFILE "ori="
#define ABOUT_TEXT "生成影子\nAuthor: garfeng\ncopyright 2016-2016"
#define WEBSITE "http://garfeng.github.io/2016-06-19/Auto-Shadow/"
#define GITHUB "http://github.com/garfeng/shadow"

#define CONF "config.json"

namespace Ui {
class Shadow;
}

class Shadow : public QMainWindow
{
    Q_OBJECT

public:
    explicit Shadow(QWidget *parent = 0);
    ~Shadow();

private slots:
    void on_btnOutputDir_clicked();

    void on_btnInputImg_clicked();

    void on_btnPadding_clicked();

    void on_btnSubmit_clicked();

    QString strPlusScroll(QString str);

    void on_GotoWeb_triggered();

    void on_LocalFile_triggered();

    void on_actionAbout_triggered();

    void on_actionAuthor_s_Blog_triggered();

    void stupConf();
    void saveConf();
    QString GetJsonVal(QString key,QJsonObject jsonObj);

    void on_btnOriFile_clicked();

private:
    Ui::Shadow *ui;
};

#endif // SHADOW_H

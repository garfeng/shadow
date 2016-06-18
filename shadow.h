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

#define LX "lx="
#define LY "lx="
#define WALL "wall="
#define ATTEN "atten="
#define FN "fn="
#define OUTPUT "out="
#define PY "py="
#define SCALE "scale="
#define ABOUT_TEXT "生成影子\nAuthor: garfeng\ncopyright 2016-2016"
#define WEBSITE "http://xxx.xxx"
#define GITHUB "http://github.com/garfeng/xxx"

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

private:
    Ui::Shadow *ui;
};

#endif // SHADOW_H

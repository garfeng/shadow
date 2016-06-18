#ifndef SHADOW_H
#define SHADOW_H

#include <Windows.h>
#include <iostream>
#include <QMainWindow>
#include <QFileDialog>
#include <QMessageBox>



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

private:
    Ui::Shadow *ui;
};

#endif // SHADOW_H

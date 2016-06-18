#include "shadow.h"
#include "ui_shadow.h"

Shadow::Shadow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Shadow)
{
    ui->setupUi(this);
}

Shadow::~Shadow()
{
    delete ui;
}

void Shadow::on_btnOutputDir_clicked()
{
    QString dirname;
    dirname = QFileDialog::getExistingDirectory(this,tr("选择输出文件存放的文件夹"),"",QFileDialog::ShowDirsOnly|QFileDialog::DontResolveSymlinks);
    if(dirname.isEmpty()){
        return;
    }
    ui->outputDir->setText(dirname);
}

void Shadow::on_btnInputImg_clicked()
{
    QStringList Files;
    QString strFiles;
    //this,tr("打开图片"),"","Pictures (*.png)"
    Files = QFileDialog::getOpenFileNames(this,tr("打开图片(多选)"),"","Pictures (*.png)");
    if(Files.isEmpty()){
        return;
    }
    strFiles = Files.join(" ");
    ui->inputImg->setText(strFiles);

}

void Shadow::on_btnPadding_clicked()
{
    QString strFileName;
    strFileName = QFileDialog::getOpenFileName(this,tr("打开图片"),"","Pictures (*.png)");
    if(strFileName.isEmpty()){
        return;
    }
    ui->padding->setText(strFileName);
}

void Shadow::on_btnSubmit_clicked()
{
    QStringList args;
    QString argsStr;
    std::string stdStrargs;
    args << "hello.exe" << ui->inputImg->text() << ui->outputDir->text();
    argsStr = args.join(" ");
    stdStrargs = argsStr.toStdString();
    const char* charArgs = stdStrargs.c_str();
    WinExec(charArgs,1);
}

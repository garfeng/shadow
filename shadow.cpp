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
    strFiles = Files.join("\n");
    ui->inputImg->setText(strFiles);

    //ui->inputImg->setText(ui->textEdit->toPlainText());

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

/*
#define LX "lx="
#define LY "ly="
#define WALL "wall="
#define ATTEN "atten="
#define FN "fn="
#define OUTPUT "out="
#define PY "py="
#define SCALE "scale="
*/

QString Shadow::strPlusScroll(QString str){
    int scrollWidth = ui->scrollWidth->text().toInt();
    int tmpInt;
    float tmpFloat;
    tmpFloat = str.toFloat();
    tmpInt = int(scrollWidth*tmpFloat);
    return QString::number(tmpInt);
}

void Shadow::on_btnSubmit_clicked()
{
/*
    ui.colorButton->setPalette(QPalette(Qt::green));
    ui.colorButton->setAutoFillBackground(true);

    QPalette qpalette = ui->btnSubmit->palette();
    qpalette.setColor(QPalette::ButtonText,QColor(230,70,70,255));

    ui->btnSubmit->setPalette(qpalette);
    ui->btnSubmit->setAutoFillBackground(true);
    ui->btnSubmit->setFlat(true);
    */
    //ui->btnSubmit->set
    //ui->btnSubmit->setStyleSheet("background-color:#FF0000");


    QStringList args;
    QString argsStr;
    std::string stdStrargs;
    QString execFile;
    // = "./hello";
#ifdef Q_OS_WIN32
    execFile = "hello.exe";
#endif
#ifdef Q_OS_LINUX
    execFile = "./hello";
#endif
    QStringList b = ui->inputImg->toPlainText().split("\n");
    args.append(b);

    QString tmp;

    //light x
    tmp.clear();
    tmp.append(LX);
    tmp.append(strPlusScroll(ui->lightX->text()));
    args << tmp;

    //light y
    tmp.clear();
    tmp.append(LY);
    tmp.append(strPlusScroll(ui->lightY->text()));
    args<<tmp;

    tmp.clear();
    tmp.append(WALL);
    tmp.append(strPlusScroll(ui->wall->text()));
    args<<tmp;

    tmp.clear();tmp.append(ATTEN);tmp.append(ui->attenuator->text());args<<tmp;
    tmp.clear();tmp.append(FN);tmp.append(ui->outfilename->text());args<<tmp;
    tmp.clear();tmp.append(OUTPUT);tmp.append(ui->outputDir->text());args<<tmp;
    tmp.clear();tmp.append(PY);tmp.append(ui->padding->text());args<<tmp;

    int scale = ui->scale->text().toInt();
    tmp.clear();tmp.append(SCALE);tmp.append(QString::number(scale));args<<tmp;



    //argsStr = args.join(" ");
    //stdStrargs = argsStr.toStdString();
    //const char* charArgs = stdStrargs.c_str();
    QProcess* process = new QProcess();
    //process->start(execFile);
    process->start(execFile,args);
    process->waitForStarted();
    process->waitForFinished();
    QString strResult = QString::fromLocal8Bit(process->readAllStandardOutput());
    QString strError = QString::fromLocal8Bit(process->readAllStandardError());
    strResult.append(strError);
    ui->coreOutput->setText(strResult);

    //QMessageBox megBox(this);
    //megBox.setText(strResult);
    //megBox.exec();

    //WinExec(charArgs,1);
}

void Shadow::on_GotoWeb_triggered()
{
    QDesktopServices::openUrl(QUrl(WEBSITE));
}

void Shadow::on_LocalFile_triggered()
{
    QString path = QDir::currentPath();
    path.append("/help.html");
    QDesktopServices::openUrl(QUrl(path,QUrl::TolerantMode));
}

void Shadow::on_actionAbout_triggered()
{
    QMessageBox messageBox(this);

    messageBox.setText(ABOUT_TEXT);
    messageBox.exec();
}

void Shadow::on_actionAuthor_s_Blog_triggered()
{
    QDesktopServices::openUrl(QUrl(GITHUB));
}

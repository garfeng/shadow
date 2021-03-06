#include "shadow.h"
#include "ui_shadow.h"

Shadow::Shadow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Shadow)
{
    ui->setupUi(this);
    //ui->inputImg->setText("hhhh");
    stupConf();
}

Shadow::~Shadow()
{
    delete ui;
}

QString Shadow::GetJsonVal(QString key,QJsonObject jsonObj)
{
    if(jsonObj.contains(key))
    {
        QJsonValue value = jsonObj.take(key);
        if(value.isDouble())
        {
            int valueInt = (int)(value.toDouble());
            QString str = QString::number(valueInt);
            return str;
        } else if(value.isString()){
            QString valueString = value.toString();
            return valueString;
        } else {
            return "";
        }
    }
    return "";
}

void Shadow::stupConf(){
    QFile file(CONF);
    if(!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        QMessageBox::warning(this,"IO error!","Can not read config file");
    }
    QJsonParseError json_error;
    QByteArray jsonByteArray = file.readAll();
    file.close();
    QJsonDocument jsonDoc = QJsonDocument::fromJson(jsonByteArray,&json_error);
    QJsonObject jsonObj;
    if(json_error.error == QJsonParseError::NoError){
        if(jsonDoc.isObject()){
            jsonObj = jsonDoc.object();
        }
    }
    QString val;
    val = GetJsonVal("scrollwidth",jsonObj);
    if (val != ""){
        ui->scrollWidth->setText(val);
    }

    val = GetJsonVal("root",jsonObj);
    if(val!="")
    {
        ui->outputDir->setText(val);
    }

    val = GetJsonVal("scale",jsonObj);
    if(val!="")
    {
        ui->scale->setValue(val.toInt());
        //ui->scale->setText(val);
    }

    val = GetJsonVal("atten",jsonObj);
    if(val!="")
    {
        ui->attenuator->setText(val);
    }
}

void Shadow::saveConf(){
    QJsonObject jsonObj;
    jsonObj.insert("scrollwidth",ui->scrollWidth->text().toInt());
    jsonObj.insert("root",ui->outputDir->text());
    jsonObj.insert("scale",ui->scale->text().toInt());
    jsonObj.insert("atten",ui->attenuator->text().toInt());
    QJsonDocument JsonDoc;
    JsonDoc.setObject(jsonObj);
    QByteArray jsonByte = JsonDoc.toJson(QJsonDocument::Compact);
    QFile file(CONF);
    if(!file.open(QIODevice::ReadWrite |QIODevice::Truncate| QIODevice::Text))
    {
        QMessageBox::warning(this,"IO error!","Can not read config file");
    }
    //ui->coreOutput->setText(jsonByte);
    file.write(jsonByte);
    file.close();
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


void Shadow::on_btnOriFile_clicked()
{
    QString strFileName;
    strFileName = QFileDialog::getOpenFileName(this,tr("打开图片"),"","Pictures (*.png)");
    if(strFileName.isEmpty()){
        return;
    }
    ui->oriFile->setText(strFileName);
}



void Shadow::on_btnSubmit_clicked()
{
    saveConf();
    QStringList args;
    QString argsStr;
    std::string stdStrargs;
    QString execFile;
    // = "./hello";
#ifdef Q_OS_WIN32
    execFile = "shadowCore.exe";
#endif
#ifdef Q_OS_LINUX
    execFile = "./shadowCore";
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
    tmp.clear();tmp.append(ORIFILE);tmp.append(ui->oriFile->text());args<<tmp;

    int scale = ui->scale->text().toInt();
    tmp.clear();tmp.append(SCALE);tmp.append(QString::number(scale));args<<tmp;



    //argsStr = args.join(" ");
    //stdStrargs = argsStr.toStdString();
    //const char* charArgs = stdStrargs.c_str();

    QProcess* process = new QProcess();

    process->start(execFile,args);
    process->waitForStarted();
    process->waitForFinished();
    QString strResult = QString::fromLocal8Bit(process->readAllStandardOutput());
    QString strError = QString::fromLocal8Bit(process->readAllStandardError());
    strResult.append(strError);
    //ui->coreOutput->setText(strResult);
    ui->coreOutput->append(strResult);

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


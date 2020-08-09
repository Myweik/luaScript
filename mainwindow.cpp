#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QDir>
#include <QFileDialog>
#include <QProcess>

#include <QDebug>
#include <QMessageBox>

#define COED "_DIST"

//让编译器编译生成程序的执行字符集为utf-8
#if _MSC_VER >= 1600
#pragma execution_character_set("utf-8")
#endif

MainWindow::MainWindow(QWidget *parent)
	: QMainWindow(parent)
	, ui(new Ui::MainWindow)
{
	ui->setupUi(this);
	this->setWindowTitle("LuaTool");

	_p = new QProcess(this);
	connect(_p, SIGNAL(finished(int)), this, SLOT(onReadResult(int)));
}

MainWindow::~MainWindow()
{
	delete ui;
}

void MainWindow::openProcess(QString com)
{

	_p->start("cmd");
	_p->write(com.toUtf8());
	_p->closeWriteChannel();
	if (_p->waitForStarted())
	{
		_p->waitForFinished();
		qDebug() << _p->readAllStandardOutput();
	}
	else
		qDebug() << "Failed to start";
}

void MainWindow::onReadResult(int exitCode)
{
	if(exitCode == 0) {
//		QTextCodec* gbkCodec = QTextCodec::codecForName("GBK");
//		QString result = gbkCodec->toUnicode(p->readAll());
//		QMessageBox::information(this, "dir", result);
	}
}

void MainWindow::luac(QString fileName)	//编码
{
	QFileInfo fileInfo(fileName);
	if(!fileInfo.isFile())
		return;

	bool  check = ui->checkBox->isChecked(); //仅仅处理有后缀的

	QString distFileName;
	QString coed = COED;
	if(fileInfo.baseName().contains(coed)){
		distFileName = fileInfo.path() + "/" + fileInfo.baseName().left(fileInfo.baseName().size()-coed.size()) + "." + fileInfo.suffix();
	}else{	//无后缀的
		if(check)
			return;
		/* 有后缀的存在 则无后缀的也不处理 */
		QString pDistFileName = fileInfo.path() + "/" + fileInfo.baseName() + QString::fromUtf8(COED) + "." + fileInfo.suffix();
		if(QFile::exists(pDistFileName))
			return;

		distFileName = fileName;
	}

	QString com = QCoreApplication::applicationDirPath() + "/script/luac.exe -o " + distFileName + " " + fileName;

	qDebug() << "------<<" << com;

	QProcess p(0);
	p.start(com);

	if (p.waitForStarted())
	{
		p.waitForFinished();
		qDebug() << p.readAllStandardOutput();
	}
	else
		qDebug() << "Failed to start";

	qApp->processEvents();

//	luac.exe -o dist.lua src.lua
}

void MainWindow::java(QString fileName)	//解码
{
	QFileInfo fileInfo(fileName);
	if(!fileInfo.isFile())
		return;

	QString coed = COED;
	if(fileInfo.baseName().contains(coed)){	//说明是已解码文件 不处理
		return;
	}

	QString distFileName = fileInfo.path() + "/" + fileInfo.baseName() + QString::fromUtf8(COED) + "." + fileInfo.suffix();



	QString com = "java -jar " + QCoreApplication::applicationDirPath() + "/script/unluac_2015_06_13.jar --rawstring "  + fileName + " > " + distFileName + "\n";

	qDebug() << "------<<" << com;
	openProcess(com);

	qApp->processEvents();

	//	java -jar unluac_2015_06_13.jar --rawstring src.lua > dist.lua
}

void MainWindow::deleteFile(QString fileName)
{
	QFileInfo fileInfo(fileName);
	if(!fileInfo.isFile())
		return;

	QString distFileName;

	QString coed = COED;

	if(fileInfo.baseName().contains(coed)){
		QFile::remove(fileName);
	}

	qApp->processEvents();
}

void MainWindow::replaceFile(QString fileName)
{
	QFileInfo fileInfo(fileName);
	if(!fileInfo.isFile())
		return;

	QString distFileName;

	QString coed = COED;

	if(fileInfo.baseName().contains(coed)){ //只处理带标志的
		QString srcFileName = fileInfo.path() + "/" + fileInfo.baseName().left(fileInfo.baseName().size()-coed.size()) + "." + fileInfo.suffix();
		QFile::remove(srcFileName);
		QFile::rename(fileName, srcFileName);
	}

	qApp->processEvents();
}

//-----------------------------------------------------------------------------------------------------------

void MainWindow::message(QString text)
{
//	QString result = "执行完毕";
	QMessageBox::information(this, "提示", text);
}

bool MainWindow::isFile(QString fileName)
{
	if(fileName.isEmpty()){
		message("未选择文件");
		return false;
	}

	QFile file(fileName);
	if(!file.exists()){
		message("文件不存在");
		return false;
	}

	return true;
}

bool MainWindow::isDir(QString dirName)
{
	if(dirName.isEmpty()){
		message("未选择文件夹");
		return false;
	}

	QDir dir(dirName);
	if(!dir.exists()){
		message("文件夹不存在");
		return false;
	}
	return true;
}

void MainWindow::setSlider(uint32_t inde, uint32_t max)
{
	QString str = QString("%1/%2").arg(inde).arg(max);
	ui->Slider->setText(str);
}

/* ---------------------------------单一文本---------------------------------------- */

void MainWindow::on_pushButton_clicked()	//文件命
{
	QString fileName = QFileDialog::getOpenFileName(this, tr("Open lua"),
													"/home",
													tr("Images (*.lua)"));
	if(!isFile(fileName))
		return;
	ui->fileName->setText(fileName);
}

void MainWindow::on_pushButton_2_clicked()	//编译
{
	QString fileName = ui->fileName->text();
	if(!isFile(fileName))
		return;

	luac(fileName);
	message("执行完毕");
}

void MainWindow::on_pushButton_3_clicked()  //反编译
{
	QString fileName = ui->fileName->text();
	if(!isFile(fileName))
		return;

	java(fileName);
	message("执行完毕");
}

void MainWindow::on_pushButton_8_clicked()
{
	QString fileName = ui->fileName->text();
	if(!isFile(fileName))
		return;

	deleteFile(fileName);
	message("执行完毕");
}


/* ----------------------------文件夹下所有文本------------------------------------ */

void MainWindow::on_pushButton_4_clicked()	//文件命
{
	QString dirName = QFileDialog::getExistingDirectory(this, tr("Open Directory"),
													"/home",
													QFileDialog::ShowDirsOnly
													| QFileDialog::DontResolveSymlinks);
	if(!isDir(dirName))
		return;
	ui->dirName->setText(dirName);
}

void MainWindow::on_pushButton_5_clicked()	//编译
{
	QString dirName = ui->dirName->text();
	if(!isDir(dirName))
		return;

	QDir dir(dirName);
	QStringList filter;
	filter << "*.lua";
	QFileInfoList fileInfoList = dir.entryInfoList(filter);//put all folder in this list

	setSlider(0, fileInfoList.size());
	uint32_t i = 0;
	foreach(auto fileInfo, fileInfoList){
		luac(fileInfo.filePath());
		setSlider(++i, fileInfoList.size());
	}

	message("执行完毕");
}

void MainWindow::on_pushButton_6_clicked()	//反编译
{
	QString dirName = ui->dirName->text();
	if(!isDir(dirName))
		return;

	QDir dir(dirName);
	QStringList filter;
	filter << "*.lua";
	QFileInfoList fileInfoList = dir.entryInfoList(filter);//put all folder in this list

	setSlider(0, fileInfoList.size());
	uint32_t i = 0;
	foreach(auto fileInfo, fileInfoList){
		java(fileInfo.filePath());
		setSlider(++i, fileInfoList.size());
	}

	message("执行完毕");
}


void MainWindow::on_pushButton_7_clicked()
{
	QString dirName = ui->dirName->text();
	if(!isDir(dirName))
		return;

	QDir dir(dirName);
	QStringList filter;
	filter << "*.lua";
	QFileInfoList fileInfoList = dir.entryInfoList(filter);//put all folder in this list

	setSlider(0, fileInfoList.size());
	uint32_t i = 0;
	foreach(auto fileInfo, fileInfoList){
		deleteFile(fileInfo.filePath());
		setSlider(++i, fileInfoList.size());
	}

	message("执行完毕");
}

void MainWindow::on_pushButton_9_clicked() //替换
{

	QString dirName = ui->dirName->text();
	if(!isDir(dirName))
		return;

	QDir dir(dirName);
	QStringList filter;
	filter << "*.lua";
	QFileInfoList fileInfoList = dir.entryInfoList(filter);//put all folder in this list

	setSlider(0, fileInfoList.size());
	uint32_t i = 0;
	foreach(auto fileInfo, fileInfoList){
		replaceFile(fileInfo.filePath());
		setSlider(++i, fileInfoList.size());
	}

	message("执行完毕");
}

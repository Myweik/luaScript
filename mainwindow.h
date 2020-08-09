#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

class QProcess;

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
	Q_OBJECT

public:
	MainWindow(QWidget *parent = nullptr);
	~MainWindow();

	void openProcess(QString com);
private slots:
	void onReadResult(int exitCode);

	void on_pushButton_clicked();

	void on_pushButton_2_clicked();

	void on_pushButton_3_clicked();

	void on_pushButton_4_clicked();

	void on_pushButton_5_clicked();

	void on_pushButton_6_clicked();

	void on_pushButton_8_clicked();

	void on_pushButton_7_clicked();

	void on_pushButton_9_clicked();

private:
	bool isFile(QString fileName);
	bool isDir(QString dirName);

	void setSlider(uint32_t inde, uint32_t max);

	void luac(QString fileName);	//编码
	void java(QString fileName);	//解码
	void deleteFile(QString fileName); //删除带_DIST的文件
	void replaceFile(QString fileName); //_DIST的文件替换原文件

	void message(QString text);						//提示

private:
	Ui::MainWindow *ui;

private:
	QProcess *_p;
};
#endif // MAINWINDOW_H

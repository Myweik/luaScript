#include "mainwindow.h"

#include <QApplication>
#include <QTextCodec>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);

	//使能高DPI自动缩放 - 兼容分辨率
	QGuiApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

	QTextCodec *codec = QTextCodec::codecForName("UTF-8");//或者"GBK",不分大小写
	QTextCodec::setCodecForLocale(codec);

	MainWindow w;
	w.show();
	return a.exec();
}

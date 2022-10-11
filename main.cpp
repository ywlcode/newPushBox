#include "beginpage.h"
#include <QApplication>
#include <QFile>
#include <QTextStream>

#if _MSC_VER >= 1600
#pragma execution_character_set("utf-8")
#endif

int main(int argc, char* argv[])
{
	QApplication a(argc, argv);

	QFile file(":/res/qss/style-1.qss");/*QSS文件所在的路径*/

	file.open(QFile::ReadOnly);
	QTextStream filetext(&file);
	QString stylesheet = filetext.readAll();
	a.setStyleSheet(stylesheet);
	file.close();

	BeginPage w;
	w.show();

	return a.exec();
}

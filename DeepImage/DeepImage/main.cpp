#include "DeepImage.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	DeepImage w;
	w.show();
	return a.exec();
}

#include <QApplication>
#include "qalarm.h"
//
int main(int argc, char ** argv)
{
	QApplication app( argc, argv );
	app.setApplicationName( "ALARM" );
	Qalarm win;
	app.setQuitOnLastWindowClosed(false);
	return app.exec();
}

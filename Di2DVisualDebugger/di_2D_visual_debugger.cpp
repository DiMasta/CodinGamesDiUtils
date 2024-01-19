#include <QtWidgets/QApplication>
#include <QtCore/QCommandLineParser>
#include <QtCore/QCommandLineOption>

#include "mainwindow.h"

int main(int argc, char** argv) {
	QApplication app(argc, argv);
	QCoreApplication::setApplicationName("SVG Viewer");
	QGuiApplication::setApplicationDisplayName(QCoreApplication::applicationName());
	QCoreApplication::setOrganizationName("QtProject");
	QCoreApplication::setApplicationVersion(QT_VERSION_STR);

	QCommandLineParser parser;
	parser.setApplicationDescription("Qt SVG Viewer");
	parser.addHelpOption();
	parser.addVersionOption();
	parser.addPositionalArgument("file", "The file to open.");
	parser.process(app);

	MainWindow window{ "C:/VLADO/CodinGame/FallChallenge2023/output.txt", "C:/VLADO/CodinGame/CodinGamesDiUtils/output/output.svg" };
	window.show();
	return app.exec();
}

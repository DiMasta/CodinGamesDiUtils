#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtWidgets/QMainWindow>
#include <QtCore/QString>

#include "di_svg_generator.h"

class SvgView;

QT_BEGIN_NAMESPACE
class QAction;
class QGraphicsView;
class QGraphicsScene;
class QGraphicsRectItem;
class QLabel;
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
	Q_OBJECT

public:
	MainWindow(
		const QString& inputTextFile,
		const QString& inputJSONFile,
		const QString& outputSVGFile
	);

	bool loadFile(const QString& path);

public slots:
	void openFile();
	void exportImage();
	void setRenderer(int renderMode);

private slots:
	void updateZoomLabel();

protected:
	void keyPressEvent(QKeyEvent* event) override;

private:
	QAction* m_nativeAction;
	QAction* m_glAction;
	QAction* m_imageAction;
	QAction* m_antialiasingAction;
	QAction* m_backgroundAction;
	QAction* m_outlineAction;

	SvgView* m_view;
	QLabel* m_zoomLabel;

	QString m_currentPath;
	QString inputTextFile;
	QString inputJSONFile;
	QString outputSVGFile;
	DiSVGGenerator diSVGGenerator;
};

#endif
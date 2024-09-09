#include "di_svg_generator.h"

#include <QtWidgets/QApplication>
#include <QtCore/QFile>

DiSVGGenerator::DiSVGGenerator(const QString& inputTextFile, const QString& outputSVGFile) :
	inputTextFile{ inputTextFile },
	outputSVGFile{ outputSVGFile },
	svgGenerator{},
	svgPainter{}
{

}

//*****************************************************************************************************************************
//*****************************************************************************************************************************

bool DiSVGGenerator::generateNextTurn() {
	bool gameTurnDataFound{ false };

	if (generate(currentGameTurn + 1)) {
		++currentGameTurn;
		gameTurnDataFound = true;
	}

	return gameTurnDataFound;

}

//*****************************************************************************************************************************
//*****************************************************************************************************************************

bool DiSVGGenerator::generatePrevTurn() {
	bool gameTurnDataFound{ false };

	if (generate(currentGameTurn - 1)) {
		--currentGameTurn;
		gameTurnDataFound = true;
	}

	return gameTurnDataFound;
}

//*****************************************************************************************************************************
//*****************************************************************************************************************************

bool DiSVGGenerator::generate(const int gameTurn) {
	bool gameTurnDataFound{ false };

	QFile file(inputTextFile);
	if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
		qDebug() << "Could not open the file";
		return false;
	}

	QTextStream in(&file);
	prepareForDrawing(in);

	QString elementType;
	bool skipElement{ false };
	while (!in.atEnd()) {
		in >> elementType;
		if (ELEM_TURN == elementType) {
			int turnIdx;
			in >> turnIdx;
			in.skipWhiteSpace();

			if (gameTurn == turnIdx) {
				drawTurn(turnIdx);
				skipElement = false;
				gameTurnDataFound = true;
			}
			else {
				skipElement = true;
			}
		}
		else if (ELEM_TEXT == elementType) {
			readAndDrawText(in, skipElement);
		}
		else if (ELEM_CIRCLE == elementType) {
			readAndDrawCircle(in, skipElement);
		}
		else if (ELEM_RECT == elementType) {
			readAndDrawRect(in, skipElement);
		}
		else if (ELEM_LINE == elementType) {
			readAndDrawLine(in, skipElement);
		}
	}

	svgPainter->end();

	return gameTurnDataFound;
}

//*****************************************************************************************************************************
//*****************************************************************************************************************************

void DiSVGGenerator::prepareForDrawing(QTextStream& in) {
	int fileVersion;
	in >> fileVersion;
	in.skipWhiteSpace();

	int imageWidth, imageHeight;
	in >> imageWidth >> imageHeight;
	in.skipWhiteSpace();

	QFile file(outputSVGFile);
	if (file.open(QIODevice::WriteOnly | QIODevice::Truncate)) {
		file.close();
	}
	else {
		qDebug() << "Failed to delete contents of" << outputSVGFile;
	}

	svgGenerator.reset(new QSvgGenerator);
	svgPainter.reset(new QPainter);

	svgGenerator->setFileName(outputSVGFile);
	svgGenerator->setSize(QSize(imageWidth, imageHeight));
	svgGenerator->setViewBox(QRect(0, 0, imageWidth, imageHeight));
	svgGenerator->setTitle(QString{ "Di 2D SVG CodinGame Visual Debugger" });
	svgGenerator->setDescription(QString{ "Game state visualizer for a given state data as text" });

	svgPainter->begin(svgGenerator.get());
	svgPainter->fillRect(QRect(0, 0, imageWidth, imageHeight), Qt::black);
}

//*****************************************************************************************************************************
//*****************************************************************************************************************************

void DiSVGGenerator::drawTurn(const int turnIdx) {
	const QString turnStr{ QString("TURN: %1").arg(turnIdx) };

	svgPainter->setFont(QFont(TEXT_FONT, TEXT_FONT_SIZE * 2));
	svgPainter->setPen(Qt::white);
	svgPainter->drawText(TEXT_FONT_SIZE / 2, TEXT_FONT_SIZE * 2, turnStr);

	QList<QPointF> points;
	points.append({ 0, 0 });
	points.append({ 100, 100 });
	points.append({ 200, 100 });
	points.append({ 500, 500 });

	QPainterPath path;
	path.moveTo(points[0]);  // Move to the first point

	for (int i = 1; i < points.size(); ++i) {
		path.lineTo(points[i]);  // Draw lines between points
	}

	svgPainter->setPen(QPen{ Qt::red, static_cast<qreal>(LINES_WIDTH) });
	svgPainter->setBrush(Qt::NoBrush);
	svgPainter->drawPath(path);
}

//*****************************************************************************************************************************
//*****************************************************************************************************************************

void DiSVGGenerator::readAndDrawText(QTextStream & in, const bool skipElement) {
	float textPosX, textPosY;
	in >> textPosX >> textPosY;

	QString colorStr;
	in >> colorStr;
	in.skipWhiteSpace();

	const QString text = in.readLine();
	in.skipWhiteSpace();

	if (!skipElement) {
		svgPainter->setFont(QFont(TEXT_FONT, TEXT_FONT_SIZE));
		QColor color{ colorStr };
		svgPainter->setPen(QPen{ color });
		svgPainter->drawText(textPosX, textPosY, text);
	}
}

//*****************************************************************************************************************************
//*****************************************************************************************************************************

void DiSVGGenerator::readAndDrawCircle(QTextStream& in, const bool skipElement) {
	int filled;
	in >> filled;

	float posX, posY, radius;
	in >> posX >> posY >> radius;

	QString colorStr;
	in >> colorStr;
	in.skipWhiteSpace();


	if (!skipElement) {
		QColor color{ colorStr };
		svgPainter->setPen(QPen{ color, static_cast<qreal>(LINES_WIDTH) });

		svgPainter->setBrush(Qt::transparent);
		if (filled) {
			svgPainter->setBrush(color);
		}

		svgPainter->drawEllipse(QPoint{ (int)posX, (int)posY }, (int)radius, (int)radius);
	}
}

//*****************************************************************************************************************************
//*****************************************************************************************************************************

void DiSVGGenerator::readAndDrawRect(QTextStream& in, const bool skipElement) {
	int filled;
	in >> filled;

	float topLeftX, topLeftY, width, height;
	in >> topLeftX >> topLeftY >> width >> height;

	QString colorStr;
	in >> colorStr;
	in.skipWhiteSpace();

	if (!skipElement) {
		QColor color{ colorStr };
		svgPainter->setPen(QPen{ color, static_cast<qreal>(LINES_WIDTH) });

		svgPainter->setBrush(Qt::transparent);
		if (filled) {
			svgPainter->setBrush(color);
		}

		svgPainter->drawRect(topLeftX, topLeftY, width, height);
	}
}

//*****************************************************************************************************************************
//*****************************************************************************************************************************

void DiSVGGenerator::readAndDrawLine(QTextStream& in, const bool skipElement) {
	int dashed;
	in >> dashed;

	float fromX, formY, toX, toY;
	in >> fromX >> formY >> toX >> toY;

	QString colorStr;
	in >> colorStr;
	in.skipWhiteSpace();

	if (!skipElement) {
		QColor color{ colorStr };
		QPen pen{ color, static_cast<qreal>(LINES_WIDTH / 2) };
		if (dashed) {
			pen.setStyle(Qt::DashLine);
		}
		else {
			pen.setStyle(Qt::SolidLine);
		}
		svgPainter->setPen(pen);
		svgPainter->drawLine(fromX, formY, toX, toY);
	}
}

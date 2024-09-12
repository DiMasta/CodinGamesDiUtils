#include "di_svg_generator.h"

#include <QtWidgets/QApplication>
#include <QtCore/QFile>
#include <string>
#include "memorystream.h"
#include "error/en.h"

using namespace std;
using namespace rapidjson;

const string JSON_FILE_VERSION{ "file_version" };
const string JSON_IMAGE_WIDTH{ "width" };
const string JSON_IMAGE_HEIGHT{ "height" };
const string JSON_TURNS{ "turns" };
const string JSON_TURN{ "turn" };
const string JSON_ENTITIES{ "entities" };
const string JSON_ELEMENTS{ "elements" };
const string JSON_TYPE{ "type" };
const string JSON_CIRCLE{ "CIRCLE" };
const string JSON_RECT{ "RECT" };
const string JSON_LINE{ "LINE" };
const string JSON_TEXT{ "TEXT" };
const string JSON_DASHED{ "dashed" };
const string JSON_FROM{ "from" };
const string JSON_TO{ "to" };
const string JSON_RADIUS{ "radius" };
const string JSON_POSITION{ "pos" };
const string JSON_COLOR{ "color" };
const string JSON_FILLED{ "filled" };
const string JSON_RED{ "red" };
const string JSON_GREEN{ "green" };
const string JSON_BLUE{ "blue" };
const string JSON_YELLOW{ "yellow" };
const string JSON_ORANGE{ "orange" };
const string JSON_PURPLE{ "purple" };
const string JSON_GRAY{ "gray" };
const string JSON_BLACK{ "black" };
const string JSON_WHITE{ "white" };
const string JSON_PINK{ "pink" };

DiSVGGenerator::DiSVGGenerator(
	const QString& inputTextFile,
	const QString& inputJSONFile,
	const QString& outputSVGFile
) :
	inputTextFile{ inputTextFile },
	inputJSONFile{ inputJSONFile },
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

void DiSVGGenerator::readAndDrawText(const Value& jsonElement) {
	assert(jsonElement.HasMember(JSON_TYPE) && jsonElement[JSON_TYPE].IsString() && JSON_TEXT == jsonElement[JSON_TYPE].GetString());

	assert(jsonElement.HasMember(JSON_COLOR) && jsonElement[JSON_COLOR].IsString());
	const QString colorStr{ jsonElement[JSON_COLOR].GetString() };

	assert(jsonElement.HasMember(JSON_TEXT) && jsonElement[JSON_TEXT].IsString());
	const QString text{ jsonElement[JSON_TEXT].GetString() };

	assert(jsonElement.HasMember(JSON_POSITION) && jsonElement[JSON_POSITION].IsObject());
	GenericValue::ConstObject posObj{ jsonElement[JSON_POSITION].GetObject() };

	float textPosX, textPosY;
}

//*****************************************************************************************************************************
//*****************************************************************************************************************************

void DiSVGGenerator::readAndDrawCircle(const Value& jsonElement) {

}

//*****************************************************************************************************************************
//*****************************************************************************************************************************

void DiSVGGenerator::readAndDrawRect(const Value& jsonElement) {

}

//*****************************************************************************************************************************
//*****************************************************************************************************************************

void DiSVGGenerator::readAndDrawLine(const Value& jsonElement) {

}

//*****************************************************************************************************************************
//*****************************************************************************************************************************

bool DiSVGGenerator::generateFromJSON(const int gameTurn) {
	bool gameTurnDataFound{ false };

	QFile file(inputJSONFile);
	if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
		qDebug() << "Could not open the file";
		return false;
	}

	QByteArray jsonData{ file.readAll() };
	file.close();

	const char* jsonStr{ jsonData.constData() };
	MemoryStream ms(jsonStr, jsonData.size());

	Document document;
	document.ParseStream(ms);

	if (document.HasParseError()) {
		qDebug() << "Error parsing JSON:" << GetParseError_En(document.GetParseError())
			<< "at offset" << document.GetErrorOffset();
		return false;
	}

	prepareForDrawing(document);

	const Value& turns{ document[JSON_TURNS] };
	assert(static_cast<int>(turns.Size()) <= gameTurn + 1);

	const Value& turn{ turns[gameTurn] };
	assert(turn.HasMember(JSON_TURN) && turn[JSON_TURN].IsInt() && gameTurn == turn[JSON_TURN].GetInt());

	drawTurn(gameTurn);

	assert(turn.HasMember(JSON_ENTITIES) && turn[JSON_ENTITIES].IsArray());
	const Value& entities{ turn[JSON_ENTITIES] };
	for (SizeType entityIdx{ 0 }; entityIdx < entities.Size(); ++entityIdx) {
		const Value& entity{ entities[entityIdx] };

		assert(entity.HasMember(JSON_ELEMENTS) && entity[JSON_ELEMENTS].IsArray());
		const Value& elements{ entity[JSON_ELEMENTS] };
		for (SizeType elementIdx{ 0 }; elementIdx < elements.Size(); ++elementIdx) {
			const Value& element{ elements[elementIdx] };
			assert(element.HasMember(JSON_TYPE) && element[JSON_TYPE].IsString());

			const string type{ element[JSON_TYPE].GetString() };
			if (JSON_CIRCLE == type) {
				readAndDrawCircle(element);
			}
			else if (JSON_RECT == type) {
				readAndDrawRect(element);
			}
			else if (JSON_LINE == type) {
				readAndDrawLine(element);
			}
			else if (JSON_TEXT == type) {
				readAndDrawText(element);
			}
		}
	}

	return gameTurnDataFound;
}

//*****************************************************************************************************************************
//*****************************************************************************************************************************

void DiSVGGenerator::prepareForDrawing(QTextStream& in) {
	int fileVersion;
	int imageWidth, imageHeight;

	in >> fileVersion;
	in.skipWhiteSpace();

	in >> imageWidth >> imageHeight;
	in.skipWhiteSpace();

	prepareSVGFileForDrawing(imageWidth, imageHeight);
}

//*****************************************************************************************************************************
//*****************************************************************************************************************************

void DiSVGGenerator::prepareForDrawing(Document& document) {
	int imageWidth, imageHeight;

	if (document.HasMember(JSON_IMAGE_WIDTH) && document[JSON_IMAGE_WIDTH].IsInt() &&
		document.HasMember(JSON_IMAGE_HEIGHT) && document[JSON_IMAGE_HEIGHT].IsInt()
	) {
		prepareSVGFileForDrawing(document[JSON_IMAGE_WIDTH].GetInt(), document[JSON_IMAGE_HEIGHT].GetInt());
	}
}

//*****************************************************************************************************************************
//*****************************************************************************************************************************

void DiSVGGenerator::prepareSVGFileForDrawing(const int imageWidth, const int imageHeight) {
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

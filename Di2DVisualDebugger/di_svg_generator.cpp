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
const string JSON_STRING{ "string" };
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
const string JSON_X{ "x" };
const string JSON_Y{ "y" };
const string JSON_DIMS{ "dims" };
const string JSON_WIDTH{ "width" };
const string JSON_HEIGHT{ "height" };
const string JSON_START{ "start" };
const string JSON_END{ "end" };

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

	//if (generate(currentGameTurn + 1)) {
	if (generateFromJSON(currentGameTurn + 1)) {
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

	assert(jsonElement.HasMember(JSON_STRING) && jsonElement[JSON_STRING].IsString());
	const QString text{ jsonElement[JSON_STRING].GetString() };

	assert(jsonElement.HasMember(JSON_POSITION) && jsonElement[JSON_POSITION].IsObject());
	const Value& pos{ jsonElement[JSON_POSITION] };

	assert(pos.HasMember(JSON_X) && pos[JSON_X].IsFloat() && pos.HasMember(JSON_Y) && pos[JSON_Y].IsFloat());
	const float posX{ pos[JSON_X].GetFloat() };
	const float posY{ pos[JSON_Y].GetFloat() };

	drawText(colorStr, posX, posY, text);
}

//*****************************************************************************************************************************
//*****************************************************************************************************************************

void DiSVGGenerator::readAndDrawCircle(const Value& jsonElement) {
	assert(jsonElement.HasMember(JSON_TYPE) && jsonElement[JSON_TYPE].IsString() && JSON_CIRCLE == jsonElement[JSON_TYPE].GetString());

	assert(jsonElement.HasMember(JSON_COLOR) && jsonElement[JSON_COLOR].IsString());
	const QString colorStr{ jsonElement[JSON_COLOR].GetString() };

	assert(jsonElement.HasMember(JSON_FILLED) && jsonElement[JSON_FILLED].IsBool());
	const bool filled{ jsonElement[JSON_FILLED].GetBool() };

	assert(jsonElement.HasMember(JSON_RADIUS) && jsonElement[JSON_RADIUS].IsFloat());
	const float radius{ jsonElement[JSON_RADIUS].GetFloat() };

	assert(jsonElement.HasMember(JSON_POSITION) && jsonElement[JSON_POSITION].IsObject());
	const Value& pos{ jsonElement[JSON_POSITION] };

	assert(pos.HasMember(JSON_X) && pos[JSON_X].IsFloat() && pos.HasMember(JSON_Y) && pos[JSON_Y].IsFloat());
	const float posX{ pos[JSON_X].GetFloat() };
	const float posY{ pos[JSON_Y].GetFloat() };

	drawCircle(colorStr, filled, posX, posY, radius);
}

//*****************************************************************************************************************************
//*****************************************************************************************************************************

void DiSVGGenerator::readAndDrawRect(const Value& jsonElement) {
	assert(jsonElement.HasMember(JSON_TYPE) && jsonElement[JSON_TYPE].IsString() && JSON_RECT == jsonElement[JSON_TYPE].GetString());

	assert(jsonElement.HasMember(JSON_COLOR) && jsonElement[JSON_COLOR].IsString());
	const QString colorStr{ jsonElement[JSON_COLOR].GetString() };

	assert(jsonElement.HasMember(JSON_FILLED) && jsonElement[JSON_FILLED].IsBool());
	const bool filled{ jsonElement[JSON_FILLED].GetBool() };

	assert(jsonElement.HasMember(JSON_POSITION) && jsonElement[JSON_POSITION].IsObject());
	const Value& pos{ jsonElement[JSON_POSITION] };

	assert(pos.HasMember(JSON_X) && pos[JSON_X].IsFloat() && pos.HasMember(JSON_Y) && pos[JSON_Y].IsFloat());
	const float posX{ pos[JSON_X].GetFloat() };
	const float posY{ pos[JSON_Y].GetFloat() };

	assert(jsonElement.HasMember(JSON_DIMS) && jsonElement[JSON_DIMS].IsObject());
	const Value& dims{ jsonElement[JSON_DIMS] };

	assert(dims.HasMember(JSON_WIDTH) && dims[JSON_WIDTH].IsFloat() && dims.HasMember(JSON_HEIGHT) && dims[JSON_HEIGHT].IsFloat());
	const float width{ dims[JSON_WIDTH].GetFloat() };
	const float height{ dims[JSON_HEIGHT].GetFloat() };

	drawRect(colorStr, filled, posX, posY, width, height);
}

//*****************************************************************************************************************************
//*****************************************************************************************************************************

void DiSVGGenerator::readAndDrawLine(const Value& jsonElement) {
	assert(jsonElement.HasMember(JSON_TYPE) && jsonElement[JSON_TYPE].IsString() && JSON_LINE == jsonElement[JSON_TYPE].GetString());

	assert(jsonElement.HasMember(JSON_COLOR) && jsonElement[JSON_COLOR].IsString());
	const QString colorStr{ jsonElement[JSON_COLOR].GetString() };

	assert(jsonElement.HasMember(JSON_DASHED) && jsonElement[JSON_DASHED].IsBool());
	const bool dashed{ jsonElement[JSON_DASHED].GetBool() };

	assert(jsonElement.HasMember(JSON_START) && jsonElement[JSON_START].IsObject());
	const Value& start{ jsonElement[JSON_START] };

	assert(start.HasMember(JSON_X) && start[JSON_X].IsFloat() && start.HasMember(JSON_Y) && start[JSON_Y].IsFloat());
	const float startX{ start[JSON_X].GetFloat() };
	const float startY{ start[JSON_Y].GetFloat() };

	assert(jsonElement.HasMember(JSON_END) && jsonElement[JSON_END].IsObject());
	const Value& end{ jsonElement[JSON_END] };

	assert(end.HasMember(JSON_X) && end[JSON_X].IsFloat() && end.HasMember(JSON_Y) && end[JSON_Y].IsFloat());
	const float endX{ end[JSON_X].GetFloat() };
	const float endY{ end[JSON_Y].GetFloat() };

	drawLine(colorStr, dashed, startX, startY, endX, endY);
}

//*****************************************************************************************************************************
//*****************************************************************************************************************************

void DiSVGGenerator::drawText(const QString& colorStr, const float textPosX, const float textPosY, const QString& text) {
	svgPainter->setFont(QFont(TEXT_FONT, TEXT_FONT_SIZE));
	QColor color{ colorStr };
	svgPainter->setPen(QPen{ color });
	svgPainter->drawText(textPosX, textPosY, text);
}

//*****************************************************************************************************************************
//*****************************************************************************************************************************

void DiSVGGenerator::drawCircle(const QString& colorStr, const bool filled, const float posX, const float posY, const float radius) {
	QColor color{ colorStr };
	svgPainter->setPen(QPen{ color, static_cast<qreal>(LINES_WIDTH) });

	svgPainter->setBrush(Qt::transparent);
	if (filled) {
		svgPainter->setBrush(color);
	}

	svgPainter->drawEllipse(QPoint{ (int)posX, (int)posY }, (int)radius, (int)radius);
}

//*****************************************************************************************************************************
//*****************************************************************************************************************************

void DiSVGGenerator::drawRect(const QString& colorStr, const bool filled, const float topLeftX, const float topLeftY, const float width, const float height) {
	QColor color{ colorStr };
	svgPainter->setPen(QPen{ color, static_cast<qreal>(LINES_WIDTH) });

	svgPainter->setBrush(Qt::transparent);
	if (filled) {
		svgPainter->setBrush(color);
	}

	svgPainter->drawRect(topLeftX, topLeftY, width, height);
}

//*****************************************************************************************************************************
//*****************************************************************************************************************************

void DiSVGGenerator::drawLine(const QString& colorStr, const bool dashed, const float fromX, const float fromY, const float toX, const float toY) {
	QColor color{ colorStr };
	QPen pen{ color, static_cast<qreal>(LINES_WIDTH / 2) };
	if (dashed) {
		pen.setStyle(Qt::DashLine);
	}
	else {
		pen.setStyle(Qt::SolidLine);
	}
	svgPainter->setPen(pen);
	svgPainter->drawLine(fromX, fromY, toX, toY);
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
	assert(gameTurn < static_cast<int>(turns.Size()));

	const Value& turn{ turns[gameTurn] };
	assert(turn.HasMember(JSON_TURN) && turn[JSON_TURN].IsInt() && gameTurn == turn[JSON_TURN].GetInt());

	drawTurn(gameTurn);

	assert(turn.HasMember(JSON_ENTITIES) && turn[JSON_ENTITIES].IsArray());
	const Value& entities{ turn[JSON_ENTITIES] };
	for (SizeType entityIdx{ 0 }; entityIdx < entities.Size(); ++entityIdx) {
		const Value& entity{ entities[entityIdx] };

		assert(entity.HasMember(JSON_ELEMENTS) && entity[JSON_ELEMENTS].IsArray());
		const Value& elements{ entity[JSON_ELEMENTS] };
		draw2DElements(elements);
	}

	draw2DElements(document[JSON_ELEMENTS]);

	svgPainter->end();

	return gameTurnDataFound;
}

//*****************************************************************************************************************************
//*****************************************************************************************************************************

void DiSVGGenerator::draw2DElements(const rapidjson::Value& elements) {
	assert(elements.IsArray());

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

	//QList<QPointF> points;
	//points.append({ 0, 0 });
	//points.append({ 100, 100 });
	//points.append({ 200, 100 });
	//points.append({ 500, 500 });
	//
	//QPainterPath path;
	//path.moveTo(points[0]);  // Move to the first point
	//
	//for (int i = 1; i < points.size(); ++i) {
	//	path.lineTo(points[i]);  // Draw lines between points
	//}
	//
	//svgPainter->setPen(QPen{ Qt::red, static_cast<qreal>(LINES_WIDTH) });
	//svgPainter->setBrush(Qt::NoBrush);
	//svgPainter->drawPath(path);
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
		drawText(colorStr, textPosX, textPosY, text);
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
		drawCircle(colorStr, filled, posX, posY, radius);
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
		drawRect(colorStr, filled, topLeftX, topLeftY, width, height);
	}
}

//*****************************************************************************************************************************
//*****************************************************************************************************************************

void DiSVGGenerator::readAndDrawLine(QTextStream& in, const bool skipElement) {
	int dashed;
	in >> dashed;

	float fromX, fromY, toX, toY;
	in >> fromX >> fromY >> toX >> toY;

	QString colorStr;
	in >> colorStr;
	in.skipWhiteSpace();

	if (!skipElement) {
		drawLine(colorStr, dashed, fromX, fromY, toX, toY);
	}
}

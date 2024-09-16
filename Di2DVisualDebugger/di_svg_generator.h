#pragma once

#include <QtCore/QString>
#include <QtGui/QPainter>
#include <QtGui/QPainterPath>
#include <QtSvg/QSvgGenerator>
#include <memory>
#include "document.h"

static const QString TEXT_FONT{ "Arial" };
static const int TEXT_FONT_SIZE{ 128 };
static const int LINES_WIDTH{ 24 };

/// Tags for drawable elements
static const QString ELEM_TURN		{ "TURN"};
static const QString ELEM_TEXT		{ "TEXT"};
static const QString ELEM_CIRCLE	{ "CIRCLE"};
static const QString ELEM_RECT		{ "RECT"};
static const QString ELEM_LINE		{ "LINE"};

/// Reads a text file representing a state frame and generates the according SVG file
class DiSVGGenerator {
public:

	/// Prepare the SVG state generator using the given files
	/// @param[in] inputTextFile the file, which describes the state and what shapes and text should be drawn
	/// @param[in] inputJSONFile the file, which describes the state and what shapes and text should be drawn in JSON format
	/// @param[in] outputSVGFile the SVG file which will hold the 2D drawn state
	DiSVGGenerator(
		const QString& inputTextFile,
		const QString& inputJSONFile,
		const QString& outputSVGFile
	);

	/// Generate visual debug information for new turns
	bool generateNextTurn();
	bool generatePrevTurn();

	/// Generate the actual SVG file, which will be used to present the 2D game state
	/// @param[in] gameTurn the turn to read and display
	/// @return true if data for the given turn is found
	bool generate(const int gameTurn);

	/// Read the game data from the JSON file and generate the actual SVG file, which will be used to present the 2D game state
	/// @param[in] gameTurn the turn to read and display
	/// @return true if data for the given turn is found
	bool generateFromJSON(const int gameTurn);

private: ///< Function members

	/// Prepare for drawing reading the state data from a normal text file
	/// @param[in/out] in the stream associated with the source text file
	void prepareForDrawing(QTextStream& in);

	/// Prepare for drawing reading the state data from a JSON file document
	/// @param[in/out] document the JSON file document holding the data
	void prepareForDrawing(rapidjson::Document& document);

	/// Create the SVG target file for drawing, given the original resolution for it
	/// @param[in] imageWidth actual width for the state 2D representation
	/// @param[in] imageHeight actual height for the state 2D representation
	void prepareSVGFileForDrawing(const int imageWidth, const int imageHeight);

	/// Draw the given 2D elements in the JSON array object
	void draw2DElements(const rapidjson::Value& elements);

	/// Functions for reading the properties of the 2D elements and drawing them on the SVG file
	void drawTurn(const int turnIdx);
	void readAndDrawText(QTextStream& in, const bool skipElement);
	void readAndDrawCircle(QTextStream& in, const bool skipElement);
	void readAndDrawRect(QTextStream& in, const bool skipElement);
	void readAndDrawLine(QTextStream& in, const bool skipElement);

	void readAndDrawText(const rapidjson::Value& jsonElement);
	void readAndDrawCircle(const rapidjson::Value& jsonElement);
	void readAndDrawRect(const rapidjson::Value& jsonElement);
	void readAndDrawLine(const rapidjson::Value& jsonElement);
	void readAndDrawPath(const rapidjson::Value& jsonElement);

	void drawText(const QString& colorStr, const float textPosX, const float textPosY, const QString& text);
	void drawCircle(const QString& colorStr, const bool filled, const float posX, const float posY, const float radius);
	void drawRect(const QString& colorStr, const bool filled, const float topLeftX, const float topLeftY, const float width, const float height);
	void drawLine(const QString& colorStr, const bool dashed, const float fromX, const float fromY, const float toX, const float toY);

private: ///< Data members
	const QString inputTextFile; ///< Describes the state and what shapes and text should be drawn
	const QString inputJSONFile; ///< Describes the state and what shapes and text should be drawn using JSON file format
	const QString outputSVGFile; ///< Hold the 2D drawn state

	std::unique_ptr<QSvgGenerator> svgGenerator; ///< The generator for the 2D SVG state representation
	std::unique_ptr<QPainter> svgPainter; ///< The painter, which is used to draw on the output SVG file

	int currentGameTurn{ 0 }; ///< The game turn for, which visual debug information is generated
};

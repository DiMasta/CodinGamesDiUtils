#pragma once

#include <QtCore/QString>
#include <QtGui/QPainter>
#include <QtGui/QPainterPath>
#include <QtSvg/QSvgGenerator>
#include <memory>

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
	/// @param[in] outputSVGFile the SVG file which will hold the 2D drawn state
	DiSVGGenerator(const QString& inputTextFile, const QString& outputSVGFile);

	/// Generate visual debug information for new turns
	bool generateNextTurn();
	bool generatePrevTurn();

	/// Generate the actual SVG file, which will be used to present the 2D game state
	/// @param[in] gameTurn the turn to read and display
	/// @return true if data for the given turn is found
	bool generate(const int gameTurn);

private: ///< Function members

	/// Create the SVG target file for drawing, reading the original resolution for it
	/// @param[in/out] in the stream associated with the source text file
	void prepareForDrawing(QTextStream& in);

	/// Functions for reading the properties of the 2D elements and drawing them on the SVG file
	void drawTurn(const int turnIdx);
	void readAndDrawText(QTextStream& in, const bool skipElement);
	void readAndDrawCircle(QTextStream& in, const bool skipElement);
	void readAndDrawRect(QTextStream& in, const bool skipElement);
	void readAndDrawLine(QTextStream& in, const bool skipElement);

private: ///< Data members
	const QString inputTextFile; ///< Describes the state and what shapes and text should be drawn
	const QString outputSVGFile; ///< Hold the 2D drawn state

	std::unique_ptr<QSvgGenerator> svgGenerator; ///< The generator for the 2D SVG state representation
	std::unique_ptr<QPainter> svgPainter; ///< The painter, which is used to draw on the output SVG file

	int currentGameTurn{ 0 }; ///< The game turn for, which visual debug information is generated
};

//-------------------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------------------

class Grid {
public:
	Grid();

	void setHeight(const int height) { this->height = height; }
	void setWidth(const int width) { this->width = width; }

	constexpr int getHeight() const {
		return height;
	}

	constexpr int getWidth() const {
		return width;
	}

	/// Set all possible cells to empty
	void init();

	/// Set the value of the given cell
	/// @param[in] rowIdx the row index of the cell to set
	/// @param[in] colIdx the column index of the cell to set
	/// @param[in] value the value for the cell
	void setCell(const int rowIdx, const int colIdx, const char value);

	/// Set the value of the given cell
	/// @param[in] coords the coordinates of the given cell
	/// @param[in] value the value for the cell
	void setCell(Coords coords, const char value);

	/// Get the value of the given cell
	/// @param[in] rowIdx the row index of the cell to set
	/// @param[in] colIdx the column index of the cell to set
	/// @return the value of the cell
	constexpr char getCell(const int rowIdx, const int colIdx) const;

	/// Get the value of the given cell
	/// @param[in] coords the coordinates of the given cell
	/// @return the value of the cell
	constexpr char getCell(Coords coords) const;

	/// Check if the given coordinates are in the grid
	/// @param[in] coords the coordinates to check
	/// @return true if the coordinates are in the grid ranges
	bool validCoords(Coords coords) const;

private:
	char grid[MAX_HEIGHT][MAX_WIDTH]; ///< Maximum possible grid
	int height; ///< Height in which to operate
	int width; ///< Width in which to operate
};

//*************************************************************************************************************
//*************************************************************************************************************

Grid::Grid() {
	init();
}

//*************************************************************************************************************
//*************************************************************************************************************

void Grid::init() {
	for (int rowIdx = 0; rowIdx < MAX_HEIGHT; ++rowIdx) {
		for (int colIdx = 0; colIdx < MAX_WIDTH; ++colIdx) {
			grid[rowIdx][colIdx] = EMPTY;
		}
	}
}

//*************************************************************************************************************
//*************************************************************************************************************

void Grid::setCell(const int rowIdx, const int colIdx, const char value) {
	grid[rowIdx][colIdx] = value;
}

//*************************************************************************************************************
//*************************************************************************************************************

void Grid::setCell(Coords coords, const char value) {
	grid[coords.rowIdx][coords.colIdx] = value;
}

//*************************************************************************************************************
//*************************************************************************************************************

constexpr char Grid::getCell(const int rowIdx, const int colIdx) const{
	return grid[rowIdx][colIdx];
}

//*************************************************************************************************************
//*************************************************************************************************************

constexpr char Grid::getCell(Coords coords) const{
	return grid[coords.rowIdx][coords.colIdx];
}

//*************************************************************************************************************
//*************************************************************************************************************

bool Grid::validCoords(Coords coords) const {
	return coords.rowIdx >= 0 && coords.rowIdx < height && coords.colIdx >= 0 && coords.colIdx < width;
}

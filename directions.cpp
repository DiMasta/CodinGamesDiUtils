enum class Direction : int {
	UP = 0,
	DOWN = 1,
	LEFT = 2,
	RIGHT = 3,
};

static constexpr int dirInt(const Direction direction) {
	return static_cast<int>(direction);
}

static constexpr Direction directions[] = {
	Direction::UP,
	Direction::DOWN,
	Direction::LEFT,
	Direction::RIGHT
};

static const string DIRECTIONS_STRS[DIRECTIONS_COUNT] = { "UP", "DOWN", "LEFT", "RIGHT" };
static constexpr int MOVE_IN_ROWS[DIRECTIONS_COUNT] = { -1, 1, 0, 0 };
static constexpr int MOVE_IN_COLS[DIRECTIONS_COUNT] = { 0, 0, -1, 1 };

static const map<char, string> charDirStrDirMap = {
	{UP_CHAR, DIRECTIONS_STRS[0]},
	{DOWN_CHAR, DIRECTIONS_STRS[1]},
	{LEFT_CHAR, DIRECTIONS_STRS[2]},
	{RIGHT_CHAR, DIRECTIONS_STRS[3]}
};

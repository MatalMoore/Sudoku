#include "Sudoku.h"

using namespace std;

ostream& operator<<(ostream& out, Entry entry) {
	out << "(" << _FIRST(entry) << "," << _SECOND(entry) << ")";
	return out;
}
ostream& operator<<(ostream& out, IntVector ints) {
	out << "{";
	for (int i = 0; i < ints.size(); i++) {
		out << " " << ints[i] << ",";
	}
	out << "}";
	return out;
}
ostream& operator<<(ostream& out, EntryVector ints) {
	out << "{";
	for (int i = 0; i < ints.size(); i++) {
		out << " " << ints[i] << ",";
	}
	out << "}";
	return out;
}

// Public

Sudoku::Sudoku() {
	for (int i = 0; i < 81; i++) {
		m_data[i] = 0;
	}
	for (int i = 0; i < 9; i++) {
		m_grid[i] = m_data + i * 9;
	}
}
Sudoku::Sudoku(const Sudoku& other) : Sudoku() {
	*this = other;
}
Sudoku::~Sudoku() {
	if (m_solvedGame != nullptr) {
		delete m_solvedGame;
	}
}

void Sudoku::enter(int value, int row, int column, bool isFixed) {
	m_flag_lastEntry = false;
	if (!isValidValue(value, row, column)) {
		return;
	}

	if (value != m_grid[row][column]) {
		deleteSolvedGame();
	}

	m_grid[row][column] = value;
	m_grid[row][column].isFixed = isFixed;
	m_flag_lastEntry = true;
}
void Sudoku::enter(int value, Entry entry, bool isFixed) {
	enter(value, _FIRST(entry), _SECOND(entry), isFixed);
}

void Sudoku::reset() {
	for (int i = 0; i < 81; i++) {
		if (!m_data[i].isFixed) {
			m_data[i] = 0;
		}
	}
}

void Sudoku::clear() {
	for (int i = 0; i < 81; i++) {
		m_data[i] = 0;
		m_data[i].isFixed = false;
	}
}

void Sudoku::load(istream& source, bool flag_forceFlags, bool forceFlagValue) {
	char input_char;

	int input_value;
	int grid_value;

	string input_flag;

	bool flag_checkFlag;

	m_flag_lastLoad = false;
	clear();

	if (source.eof()) {
		return;
	}

	input_char = source.peek();
	switch (input_char) {
	case HEADER_FLAGS:
		flag_checkFlag = true;
		source.get();
		break;
	case HEADER_SIMPLE:
		flag_checkFlag = false;
		source.get();
		break;
	default:
		flag_checkFlag = false;
	}

	for(int i=0; i<81 && !source.eof(); i++){
		// get value
		source >> input_value;
		if (input_value < 0) {
			grid_value = 0;
		}
		else if (input_value > 9) {
			grid_value = 9;
		}
		else {
			grid_value = input_value;
		}
		enter(input_value, i / 9, i % 9);

		// get fixed flag if it exists
		if (flag_checkFlag && !source.eof()) {
			source >> input_flag;

			if (grid_value == 0) {
				m_data[i].isFixed = false;
			}
			else if (flag_forceFlags) {
				m_data[i].isFixed = forceFlagValue;
			}
			else if (input_flag[0] == 'n') {
				m_data[i].isFixed = false;
			}
			else {
				m_data[i].isFixed = forceFlagValue;
			}
		}
		else if (input_value == 0) {
			m_data[i].isFixed = false;
		}
		else {
			m_data[i].isFixed = forceFlagValue;
		}
	}

	m_flag_lastLoad = true;
}

void Sudoku::display(ostream& out) const {
	int value;

	// first number row
	out << "  ";
	for (int i = 0; i < 9; i++) {
		out << "   " << i;
	}
	out << endl;

	// horizontal rows
	for (int j = 0; j < 9; j++) {
		out << "   ";
		// alternate solid and non-solid boundary rows
		for (int i = 0; i < 9; i++) {
			// solid row
			if (j % 3 == 0) {
				out << "+---";
			}
			// non-solid row
			else {
				out << "+   ";
			}
		}
		out << "+" << endl;

		// alternate solid and non-solid value rows
		out << " " << j << " ";
		for (int i = 0; i < 9; i++) {
			// solid row
			if (i % 3 == 0) {
				if (m_grid[j][i].isFixed) {
					out << "|[";
				}
				else {
					out << "| ";
				}
			}
			// non-solid row
			else {
				if (m_grid[j][i].isFixed) {
					out << " [";
				}
				else {
					out << "  ";
				}
			}
			value = m_grid[j][i];
			if (value == 0) {
				out << " ";
			}
			else {
				out << value;
			}
			if (m_grid[j][i].isFixed) {
				out << "]";
			}
			else {
				out << " ";
			}
		}
		out << "|" << endl;
	}

	out << "   ";
	for (int i = 0; i < 9; i++) {
		out << "+---";
	}
	out << "+" << endl;
}

void Sudoku::display_raw(ostream& out, int format) const {
	switch (format) {
	case FORMAT_HEADER_SIMPLE:
		out << HEADER_SIMPLE << endl;
		break;
	case FORMAT_HEADER_FLAGS:
		out << HEADER_FLAGS << endl;
		break;
	}
	for (int j = 0; j < 9; j++) {
		if (j % 3 == 0 && j > 0) {
			out << endl;
		}
		for (int i = 0; i < 9; i++) {
			if (i % 3 == 0 && i > 0) {
				out << " ";
			}
			out << " " << m_grid[j][i];
			if (format == FORMAT_HEADER_FLAGS) {
				if (m_grid[j][i].isFixed) {
					out << " f";
				}
				else {
					out << " n";
				}
			}
		}
		out << endl;
	}
}

const Sudoku& Sudoku::solve() {
	if (m_solvedGame != nullptr) {
		return *m_solvedGame;
	}

	if (m_solvedGame == nullptr) {
		m_solvedGame = new Sudoku(*this);
	}
	else {
		*m_solvedGame = *this;
	}

#if RECURSION_COUNT
	recursionCount = 0;
#endif
	if (solve_recursion()) {
		return *m_solvedGame;
	}
	else {
		return *this;
	}
}

Sudoku& Sudoku::operator=(const Sudoku& other) {
	if (this == &other) {
		return *this;
	}

	for (int i = 0; i < 81; i++) {
		m_data[i] = other.m_data[i];
	}

	if (other.m_solvedGame == nullptr && m_solvedGame != nullptr) {
		delete m_solvedGame;
		m_solvedGame = nullptr;
	}
	else if (other.m_solvedGame != nullptr) {
		if (m_solvedGame == nullptr) {
			m_solvedGame = new Sudoku(*other.m_solvedGame);
		}
		else {
			*m_solvedGame = *other.m_solvedGame;
		}
	}

	m_flag_lastEntry = other.m_flag_lastEntry;
	m_flag_lastLoad = other.m_flag_lastLoad;

	return *this;
}

// Private

bool Sudoku::solve_recursion() {
	vector<IntVector> vector_values;
	EntryVector vector_minEntries;

#if RECURSION_COUNT
	recursionCount++;
	//cout << recursionCount << endl;
#endif

	m_solvedGame->getAllLowestValidEntryCounts(vector_values, vector_minEntries);
	if (vector_minEntries.empty()) {
#if RECURSION_COUNT
		recursionCount--;
		//cout << recursionCount << endl;
#endif
		return false;
	}

	for (int j = 0; j < vector_minEntries.size(); j++) {
		for (int i = 0; i < vector_values[0].size(); i++) {
			m_solvedGame->enter(vector_values[j][i], vector_minEntries[j]);
			if (m_solvedGame->isSolved()) {
				return true;
			}
			if (solve_recursion()) {
				return true;
			}
			m_solvedGame->enter(0, vector_minEntries[j]);
		}
	}

#if RECURSION_COUNT
	recursionCount--;
	//cout << recursionCount << endl;
#endif
	return false;
}

bool Sudoku::isSolved() {
	for (int i = 0; i < 81; i++) {
		if (m_data[i] == 0) {
			return false;
		}
	}
	return true;
}

bool Sudoku::isValidEntry(int value, int row, int column) const {
	return value >= 0 && value <= 9
		&& row >= 0 && row <= 9
		&& column >= 0 && column <= 9
		;
}
bool Sudoku::isValidValue(int value, int row, int column) const {
	int subGrid;
	int subGridStart_j;
	int subGridStart_i;

	if (!isValidEntry(value, row, column)) {
		return false;
	}

	if (value == 0) {
		return true;
	}

	if (m_grid[row][column] != 0) {
		return false;
	}

	// check for same value in row
	for (int i = 0; i < 9; i++) {
		if (m_grid[row][i] == value && i != column) {
			return false;
		}
	}

	// check for same value in column
	for (int j = 0; j < 9; j++) {
		if (m_grid[j][column] == value && j != row) {
			return false;
		}
	}

	subGrid = getSubGridNumber(row, column);
	subGridStart_j = (subGrid / 3) * 3;
	subGridStart_i = (subGrid % 3) * 3;

	// check for same value in subgrid
	for (int j = subGridStart_j; j < subGridStart_j + 3; j++) {
		for (int i = subGridStart_i; i < subGridStart_i + 3; i++) {
			if (m_grid[j][i] == value && j != row && i != column) {
				return false;
			}
		}
	}

	return true;
}
int Sudoku::getSubGridNumber(int row, int column) const {
	if (isValidEntry(0, row, column)) {
		return column / 3 + (row / 3) * 3;
	}
	else {
		return -1;
	}
}

void Sudoku::getValidEntryCount(IntVector& values, int row, int column) const {
	values.clear();
	for (int i = 1; i <= 9; i++) {
		if (isValidValue(i, row, column)) {
			values.push_back(i);
		}
	}
}

void Sudoku::getLowestValidEntryCount(IntVector& a_values, Entry& a_entry, int start_j, int start_i) const {
	int count_min = INT_MAX;
	int count_current;

	IntVector values_min;
	IntVector values_current;
	Entry entry_min = { -1, -1 };

	for (int j = start_j; j < 9; j++) {
		for (int i = start_i; i < 9; i++) {
			values_current.clear();
			getValidEntryCount(values_current, j, i);
			count_current = (int)values_current.size();
			if (count_current == 1) {
				a_values = values_current;
				a_entry = { j, i };
				return;
			}
			else if (count_current > 0 && count_current < count_min) {
				count_min = count_current;
				values_min = values_current;
				entry_min = { j, i };
			}
		}
	}

	a_values = values_min;
	a_entry = entry_min;
}

void Sudoku::getAllLowestValidEntryCounts(vector<IntVector>& a_lists, EntryVector& a_entries) const {
	Entry entry_min;
	IntVector entryValues_min;

	EntryVector vector_entry_min;
	vector<IntVector> vector_entryValues_min;

	Entry entry_next;
	IntVector entryValues_next;

	getLowestValidEntryCount(entryValues_min, entry_min, 0, 0);
	if (entryValues_min.empty()) {
		a_lists = vector_entryValues_min;
		a_entries = vector_entry_min;
	}
	else if (entryValues_min.size() == 1) {
		a_lists = { entryValues_min };
		a_entries = { entry_min };
	}
	else {
		a_lists.push_back(entryValues_min);
		a_entries.push_back(entry_min);

		entry_next = entry_min;

		// loop iteration
		_SECOND(entry_next)++;
		if (_SECOND(entry_next) >= 9) {
			_FIRST(entry_next)++;
			_SECOND(entry_next) = 0;
		}
		getLowestValidEntryCount(entryValues_next, entry_next, _FIRST(entry_next), _SECOND(entry_next) + 1);

		while (entryValues_next.size() == entryValues_min.size()) {
			a_lists.push_back(entryValues_next);
			a_entries.push_back(entry_next);

			// copied loop iteration
			_SECOND(entry_next)++;
			if (_SECOND(entry_next) >= 9) {
				_FIRST(entry_next)++;
				_SECOND(entry_next) = 0;
			}
			getLowestValidEntryCount(entryValues_next, entry_next, _FIRST(entry_next), _SECOND(entry_next) + 1);
		}
	}
}

void Sudoku::deleteSolvedGame() {
	if (m_solvedGame != nullptr) {
		delete m_solvedGame;
		m_solvedGame = nullptr;
	}
}

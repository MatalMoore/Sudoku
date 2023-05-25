#pragma once

#define RECURSION_COUNT 0

#include <iostream>
#include <tuple>
#include <stack>
#include <vector>

#define FORMAT_SIMPLE -1 
#define FORMAT_HEADER_SIMPLE 0
#define FORMAT_HEADER_FLAGS 1

#define HEADER_SIMPLE 'n'
#define HEADER_FLAGS 'f'

#define _FIRST(a) get<0>(a)
#define _SECOND(a) get<1>(a)

typedef std::tuple<int, int> Entry;
typedef std::vector<int> IntVector;
typedef std::vector<Entry> EntryVector;

std::ostream& operator<<(std::ostream& out, Entry entry);
std::ostream& operator<<(std::ostream& out, IntVector ints);
std::ostream& operator<<(std::ostream& out, EntryVector entries);

struct Cell {
	int value;
	bool isFixed;

	Cell(int a_value = 0, bool a_isFixed = false) {
		value = a_value;
		isFixed = a_isFixed;
	}

	operator int() const {
		return value;
	}
	int operator=(int a_value) {
		value = a_value;
		return value;
	}
	std::ostream& operator<<(std::ostream& arg) const {
		arg << value;
		return arg;
	}
};

class Sudoku {
public:
	Sudoku();
	Sudoku(const Sudoku& other);
	~Sudoku();

	const bool& flag_lastEntry = m_flag_lastEntry;
	const bool& flag_lastLoad = m_flag_lastLoad;

	void enter(int value, int row, int column, bool isFixed = false);
	void enter(int value, Entry entry, bool isFixed = false);
	void reset();
	void clear();
	void load(std::istream& source, bool flag_forceFlags = false, bool forceFlagValue = true);

	void display(std::ostream& out = std::cout) const;
	void display_raw(std::ostream& out = std::cout, int format = FORMAT_SIMPLE) const;

	const Sudoku& solve();

	Sudoku& operator=(const Sudoku& other);

private:
	Cell m_data[81];
	Cell* m_grid[9];

	Sudoku* m_solvedGame = nullptr;

	bool m_flag_lastEntry = false;
	bool m_flag_lastLoad = false;

#if RECURSION_COUNT
	int recursionCount;
#endif

	EntryVector entries;

	bool solve_recursion();
	bool isSolved();

	bool isValidEntry(int value, int row, int column) const;
	bool isValidValue(int value, int row, int column) const;
	int getSubGridNumber(int row, int column) const;

	void getValidEntryCount(IntVector& values, int row, int column) const;
	void getLowestValidEntryCount(IntVector& a_lists, Entry& a_entries, int start_j = 0, int start_i = 0) const;
	void getAllLowestValidEntryCounts(std::vector<IntVector>& a_list, EntryVector& a_Entries) const;

	void deleteSolvedGame();
};

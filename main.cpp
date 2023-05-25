//TODO: make getLowestValidEntryCounts() or solve_recursion() accumulate all entries with the min number of valid values
#include <iostream>
#include <sstream>
#include <fstream>

#include "Sudoku.h"
#include "funcs.h"

using namespace std;

int main() {
	Sudoku game;
	Sudoku solution;

	string input;
	string inputNormalized;
	string filename;
	string systemCommand;
	stringstream buffer;

	bool flag_exit = false;
	bool flag_inputFormat = true;

	int input_row;
	int input_column;
	int input_value;

	ifstream source;
	ofstream target;

	cout << "Sudoku Game Interface" << endl;
	cout << endl;
	game.display();
	cout << endl;
	cout << "Type \"help\" and press enter to see all valid commands" << endl;
	cout << endl;

	while (!flag_exit) {
		cout << "Input command" << endl;
		cout << "> ";

		getline(cin, input);
		buffer << input;

		// set input to lower case
		buffer >> inputNormalized;
		inputNormalized = lowercase(inputNormalized);

		if (inputNormalized == "enter") {
			flag_inputFormat = false;
			if (!buffer.eof()) {
				buffer >> input_value;
				if (!buffer.eof()) {
					buffer >> input_row;
					if (!buffer.eof()) {
						flag_inputFormat = true;
						buffer >> input_column;
						game.enter(input_value, input_row, input_column);
						if (!game.flag_lastEntry) {
							cout << "Failed to enter " << input_value << " at position " << input_row << "," << input_column << endl;
						}
						game.display();
					}
				}
			}

			if (!flag_inputFormat) {
				cout << "Invalid input format; type \"help\" and press enter to see valid format" << endl;;
			}
		}
		else if (inputNormalized == "delete") {
			flag_inputFormat = false;
			if (!buffer.eof()) {
				buffer >> input_row;
				if (!buffer.eof()) {
					buffer >> input_column;
					game.enter(0, input_row, input_column);
					if (!game.flag_lastEntry) {
						cout << "Failed to delete value at position " << input_row << "," << input_column << endl;
					}
					game.display();
				}
			}

			if (!flag_inputFormat) {
				cout << "Invalid input format; type \"help\" and press enter to see valid format." << endl;;
			}
		}
		else if (inputNormalized == "display") {
			game.display();
		}
		else if (inputNormalized == "clear") {
			game.clear();
			game.display();
		}
		else if (inputNormalized == "reset") {
			game.reset();
			game.display();
		}
		else if (inputNormalized == "solve") {
			cout << "Now solving..." << endl;
			solution = game.solve();
			solution.display();
			cout << "Solved" << endl;
			target.open("solution.txt");
			if (target.fail()) {
				cout << "Failed to write solution to solution.txt" << endl;
			}
			else {
				solution.display_raw(target, FORMAT_HEADER_FLAGS);
				cout << "Successfully wrote solution to solution.txt" << endl;
			}
			target.close();
			target.clear();
		}
		else if (inputNormalized == "load") {
			if (!buffer.eof()) {
				buffer >> filename;
				input_value = 0;
				if (!buffer.eof()) {
					buffer >> input_value;
				}
				if (filename != "") {
					source.open(filename + ".txt");
					if (source.fail()) {
						cout << "Failed to open " << filename << ".txt for loading" << endl;
					}
					else {
						game.load(source);
						if (!game.flag_lastLoad) {
							cout << "File " << filename << ".txt contained less data than expected" << endl;
						}
						game.display();
					}
					source.close();
					source.clear();
				}
				else {
					cout << "Enter a file name after \"load\"" << endl;
				}
			}
			else {
				cout << "Enter a file name after \"load\"" << endl;
			}
		}
		else if (inputNormalized == "write") {
			if (!buffer.eof()) {
				buffer >> filename;
				if (filename != "") {
					target.open(filename + ".txt");
					if (target.fail()) {
						cout << "Failed to open " << filename << ".txt for writing" << endl;
					}
					else {
						game.display_raw(target);
						cout << "Successfully wrote game to " << filename << ".txt" << endl;
					}
					target.close();
					target.clear();
				}
				else {
					cout << "Enter a file name after \"write\"" << endl;
				}
			}
			else {
				cout << "Enter a file name after \"write\"" << endl;
			}
		}
		else if (inputNormalized == "open") {
			if (!buffer.eof()) {
				buffer >> filename;
				if (filename != "") {
					systemCommand = "start notepad ";
					systemCommand.append(filename);
					system(systemCommand.data());
				}
				else {
					cout << "Enter a file name after \"open\"" << endl;
				}
			}
			else {
				cout << "Enter a file name after \"open\"" << endl;
			}
		}
		else if (inputNormalized == "folder") {
			system("start explorer .");
		}
		else if (inputNormalized == "help") {
			cout << "Valid commands:" << endl;
			cout << "  enter VALUE ROW COLUMN" << endl;
			cout << "  delete ROW COLUMN" << endl;
			cout << "  display" << endl;
			cout << "  clear" << endl;
			cout << "  reset" << endl;
			cout << "  load FILE (loads from FILE.txt)" << endl;
			cout << "  write FILE (writes to FILE.txt)" << endl;
			cout << "  open FILE (opens FILE using the Windows notepad application)" << endl;
			cout << "  folder (opens the folder of the application using Windows explorer)" << endl;
			cout << "  exit" << endl;
		}
		else if (inputNormalized == "exit") {
			flag_exit = true;
		}
		else {
			cout << "Invalid command \"" << inputNormalized << "\"" << endl;
		}

		cout << endl;

		buffer.ignore(INT_MAX);
		buffer.clear();

		cin.seekg(0);
		cin.clear();
	}

	return 0;
}
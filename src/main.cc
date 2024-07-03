// src/main.cc

#include "../include/piece.h"
#include "../include/board.h"

#include <iostream>
#include <vector>
#include <string>
#include <memory>

using namespace std;

const unsigned int WIDTH = 8, HEIGHT = 8;

Board defaultBoard;

int parsePlayer(string &s) {
	if (s == "human") return 0;
	else if (s == "computer[1]") return 1;
	else if (s == "computer[2]") return 2;
	else if (s == "computer[3]") return 3;
	else if (s == "computer[4]") return 4;
	else if (s == "computer[5]") return 5;
	else return -1;
}

bool verifyPiece(char c) {
	return c == 'b' || c == 'B' || c == 'r' || c == 'R' || c == 'n' || c == 'N'
		|| c == 'k' || c == 'K' || c == 'q' || c == 'Q' || c == 'p' || c == 'P';
}

Board::Posn parsePosition(const string &pos) {
	return {pos[0] - 'a', pos[1] - '1'};
}

int main() {
	double whiteWins = 0, blackWins = 0;
	string command, arg1, arg2;
	bool gameActive = false, defaultWhiteTurn = true, whiteTurn;
	Board board;
	//AI *whiteAI, blackAI;

	// Initial default board
	// White pieces
	defaultBoard.addPiece('R', {0, 0});
	defaultBoard.addPiece('N', {0, 1});
	defaultBoard.addPiece('B', {0, 2});
	defaultBoard.addPiece('Q', {0, 3});
	defaultBoard.addPiece('K', {0, 4});
	defaultBoard.addPiece('B', {0, 5});
	defaultBoard.addPiece('N', {0, 6});
	defaultBoard.addPiece('R', {0, 7});

	// Black pieces
	defaultBoard.addPiece('r', {7, 0});
	defaultBoard.addPiece('n', {7, 1});
	defaultBoard.addPiece('b', {7, 2});
	defaultBoard.addPiece('q', {7, 3});
	defaultBoard.addPiece('k', {7, 4});
	defaultBoard.addPiece('b', {7, 5});
	defaultBoard.addPiece('n', {7, 6});
	defaultBoard.addPiece('r', {7, 7});

	// Pawns
	for (int i = 0; i < WIDTH; i++) {
		defaultBoard.addPiece('P', {1, i});
		defaultBoard.addPiece('p', {6, i});
	}

	while (cin >> command) {
		cout << board << endl;

		if (command == "game" && !gameActive) {
			cin >> arg1 >> arg2;
			int p1 = parsePlayer(arg1), p2 = parsePlayer(arg2);
			if (p1 < 0 || p2 < 0) continue;
			//whiteAI = p1 ? new AI{p1} : nullptr;
			//blackAI = p2 ? new AI{p2} : nullptr;
			board = defaultBoard;
			whiteTurn = defaultWhiteTurn;
			gameActive = true;
		} else if (command == "resign" && gameActive) {

			gameActive = false;
		} else if (command == "move" && gameActive) {

		} else if (command == "setup" && !gameActive) {
			while (cin >> command) {
				if (command == "+") {
					char piece;
					cin >> piece >> arg1;
					if (!verifyPiece(piece)) {
						cerr << "Please input a valid piece name." << endl;
						continue;
					}
					Board::Posn p = parsePosition(arg1);
					if (!p.validate()) {
						cerr << "Please input valid board coordinates." << endl;
						continue;
					}
					defaultBoard.addPiece(piece, p);
					cout << defaultBoard << endl;
				} else if (command == "-") {
					cin >> arg1;
					Board::Posn p = parsePosition(arg1);
					if (!p.validate()) {
						cerr << "Please input valid board coordinates." << endl;
						continue;
					}
					defaultBoard.kill(p);
					cout << defaultBoard << endl;
				} else if (command == "=") {
					cin >> arg1;
					if (arg1 == "white") {
						defaultWhiteTurn = true;
						cout << "White will now have the first turn" << endl;
					} else if (arg1 == "black") {
						defaultWhiteTurn = false;
						cout << "Black will now have the first turn" << endl;
					} else {
						cerr << "Please input a valid colour." << endl;
					}
				} else if (command == "done" && defaultBoard.validate()) {
					break;
				} else if (command == "done") {
					cerr << "Please create a valid chess setup." << endl;
				} else {
					cerr << "Please input a valid setup command." << endl;
				} // switch
			} // while
		} else {
			cerr << "Please input a valid command." << endl;
		} // switch
	} // while

	cout << "Final Score:" << endl
		 << "White: " << whiteWins << endl
		 << "Black: " << blackWins << endl;
	return 0;
} // main

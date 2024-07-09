// src/main.cc

#include "../include/AI.h"

#include <iostream>
#include <vector>
#include <string>
#include <memory>

using namespace std;

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

int main() {
	double whiteWins = 0, blackWins = 0;
	string command, arg1, arg2;
	bool gameActive = false, defaultWhiteTurn = true, whiteTurn;
	Board board;
	AI *whiteAI, *blackAI;

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

		if (command == "game") {
			if (gameActive) {
				cerr << "Game is already active." << endl;
				continue;
			}
			cin >> arg1 >> arg2;
			int p1 = parsePlayer(arg1), p2 = parsePlayer(arg2);
			if (p1 < 0 || p2 < 0) continue;
			whiteAI = p1 ? new AI{board, true, p1} : nullptr;
			blackAI = p2 ? new AI{board, false, p2} : nullptr;
			board = defaultBoard;
			whiteTurn = defaultWhiteTurn;
			gameActive = true;
		} else if (command == "resign") {
			if (!gameActive) {
				cerr << "Game is not active." << endl;
				continue;
			}
			// stuff
			gameActive = false;
		} else if (command == "move") {
			if (!gameActive) {
				cerr << "Game is not active." << endl;
				continue;
			}
			if (whiteTurn && whiteAI) {
				board.doMove(whiteAI->think());
			} else if (!whiteTurn && blackAI) {
				board.doMove(blackAI->think());
			} else {
				cin >> arg1 >> arg2;
				Posn start{arg1}, end{arg2};
				if (!start.validate() || !end.validate()) {
					cerr << "Please input valid board coordinates." << endl;
					continue;
				}
				Move move{start, end};
				if (!board.validateMove(move)) {
					cerr << "Please input a valid move action." << endl;
					continue;
				}
				board.doMove(move);
			}
			whiteTurn = !whiteTurn;
		} else if (command == "setup") {
			if (gameActive) {
				cerr << "Game is already active." << endl;
				continue;
			}
			while (cin >> command) {
				if (command == "+") { // add a piece to the board
					char piece;
					cin >> piece >> arg1;
					if (!verifyPiece(piece)) {
						cerr << "Please input a valid piece name." << endl;
						continue;
					}
					Posn p{arg1};
					if (!p.validate()) {
						cerr << "Please input valid board coordinates." << endl;
						continue;
					}
					defaultBoard.addPiece(piece, p);
					cout << defaultBoard << endl;
				} else if (command == "-") { // remove a piece from the board
					cin >> arg1;
					Posn p{arg1};
					if (!p.validate()) {
						cerr << "Please input valid board coordinates." << endl;
						continue;
					}
					defaultBoard.removePiece(p);
					cout << defaultBoard << endl;
				} else if (command == "=") { // change default starting colour
					cin >> arg1;
					if (arg1 == "white") {
						defaultWhiteTurn = true;
						cout << "White will now have the first turn." << endl;
					} else if (arg1 == "black") {
						defaultWhiteTurn = false;
						cout << "Black will now have the first turn." << endl;
					} else {
						cerr << "Please input a valid colour." << endl;
					}
				} else if (command == "done" && defaultBoard.validate()) { // valid board setup
					break;
				} else if (command == "done") { // invalid board setup
					cerr << "Please create a valid chess setup." << endl;
				} else { // invalid command
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

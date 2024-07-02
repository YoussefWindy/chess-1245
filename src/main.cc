// src/main.cc

#include "../include/piece.h"
#include "../include/board.h"

#include <iostream>
#include <vector>
#include <string>
#include <memory>

using namespace std;

	const unsigned int WIDTH = 8, HEIGHT = 8;

Board setup() {
	Board board;

	// Add pieces
	board.addPiece('R', 0, 0);
	board.addPiece('N', 0, 1);
	board.addPiece('B', 0, 2);
	board.addPiece('Q', 0, 3);
	board.addPiece('K', 0, 4);
	board.addPiece('B', 0, 5);
	board.addPiece('N', 0, 6);
	board.addPiece('R', 0, 7);

	board.addPiece('r', 7, 0);
	board.addPiece('n', 7, 1);
	board.addPiece('b', 7, 2);
	board.addPiece('q', 7, 3);
	board.addPiece('k', 7, 4);
	board.addPiece('b', 7, 5);
	board.addPiece('n', 7, 6);
	board.addPiece('r', 7, 7);

	for (unsigned int i = 0; i < WIDTH; i++) {
		board.addPiece('P', 1, i);
		board.addPiece('p', 6, i);
	}

	return board;
}

int parse(string &s) {
	if (s == "human") return 0;
	else if (s == "computer[1]") return 1;
	else if (s == "computer[2]") return 2;
	else if (s == "computer[3]") return 3;
	else if (s == "computer[4]") return 4;
	else if (s == "computer[5]") return 5;
	else return -1;
}

bool verify(char c) {
	return c == 'b' || c == 'B' || c == 'r' || c == 'R' || c == 'n' || c == 'N'
		|| c == 'k' || c == 'K' || c == 'q' || c == 'Q' || c == 'p' || c == 'P';
}

int main() {
	double whiteWins = 0, blackWins = 0;
	string command, arg1, arg2;
	bool gameActive = false, whiteTurn = true;
	Board board = setup();
	//AI *whiteAI, blackAI;

	while (cin >> command) {
		cout << board << endl;

		if (command == "game" && !gameActive) {
			cin >> arg1 >> arg2;
			int p1 = parse(arg1), p2 = parse(arg2);
			if (p1 < 0 || p2 < 0) continue;
			//whiteAI = p1 ? new AI{p1} : nullptr;
			//blackAI = p2 ? new AI{p2} : nullptr;
			gameActive = true;
		} else if (command == "resign" && gameActive) {

		} else if (command == "move" && gameActive) {

		} else if (command == "setup" && !gameActive) {
			while (cin >> command && command != "done") {
				if (command == "+") {
					char piece;
					cin >> piece >> arg1;
					if (!verify(piece)) continue;
					board.addPiece(piece, arg1);
				}
			}
		}
	}

	cout << "Final Score:" << endl
		 << "White: " << whiteWins << endl
		 << "Black: " << blackWins << endl;
	return 0;
}

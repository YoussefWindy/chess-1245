// src/main.cc

#include "../include/piece.h"
#include "../include/board.h"

#include <iostream>
#include <vector>
#include <string>
#include <memory>

using namespace std;

int main() {

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

	board.addPiece('P', 1, 0);
	board.addPiece('P', 1, 1);
	board.addPiece('P', 1, 2);
	board.addPiece('P', 1, 3);
	board.addPiece('P', 1, 4);
	board.addPiece('P', 1, 5);
	board.addPiece('P', 1, 6);
	board.addPiece('P', 1, 7);

	board.addPiece('p', 6, 0);
	board.addPiece('p', 6, 1);
	board.addPiece('p', 6, 2);
	board.addPiece('p', 6, 3);
	board.addPiece('p', 6, 4);
	board.addPiece('p', 6, 5);
	board.addPiece('p', 6, 6);
	board.addPiece('p', 6, 7);

	board.addPiece('r', 7, 0);
	board.addPiece('n', 7, 1);
	board.addPiece('b', 7, 2);
	board.addPiece('q', 7, 3);
	board.addPiece('k', 7, 4);
	board.addPiece('b', 7, 5);
	board.addPiece('n', 7, 6);
	board.addPiece('r', 7, 7);

	cout << board << endl << endl;

	board.movePiece(6, 4, 4, 4);

	cout << board << endl << endl;

	board.movePiece(7, 7, 6, 7);

	cout << board << endl;

	return 0;
}

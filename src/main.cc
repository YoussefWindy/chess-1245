// src/main.cc

#include "../include/piece.h"

#include <iostream>
#include <vector>
#include <string>

using namespace std;

int main() {
	vector<Piece*> pieces;

	pieces.push_back(new Piece(1, "Pawn", 2, 3));
	pieces.push_back(new Piece(0, "King", 4 ,5));
	pieces.push_back(new Piece(0, "Queen", 7, 0));

	
	for (auto p : pieces) {
		cout << *p << endl;
	}

	return 0;
}

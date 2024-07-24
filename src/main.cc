// src/main.cc

#include "../include/ai.h"
#include <ostream>
#include <sstream>

using namespace std;

// Display stuff
bool text, graphics;
unique_ptr<XWindow> xw;

int parsePlayer(string &s) {
	if (s == "human") return 0;
	else if (s.substr(0, 9) == "computer[" && s.back() == ']') {
		return stoi(s.substr(9, s.size() - 10));
	}
	else return -1;
}

bool isInteger(const string& str) {
    if (str.empty()) return false;
    for (char ch : str) {
		if (ch == ' ') continue;
        if (ch < '0' || ch > '9') return false;
    }
    return true;
}

void toLowercase(string& str) {
    for (char& ch : str) {
        if (ch >= 'A' && ch <= 'Z') {
            ch += 'a' - 'A'; // Convert uppercase to lowercase
        }
    }
}

void stripWhitespace(string& str) {
    string result;
    for (char ch : str) {
        if (ch != ' ') {
            result += ch;
        }
    }
    str = result;
}

bool verifyPiece(char c) {
	return c == 'b' || c == 'B' || c == 'r' || c == 'R' || c == 'n' || c == 'N'
		|| c == 'k' || c == 'K' || c == 'q' || c == 'Q' || c == 'p' || c == 'P';
}

void display(const Board &board) {
	if (text) cout << board << endl;
	if (graphics) xw->drawBoard(board);
}

int main() {
	// Declarations
	double whiteWins = 0, blackWins = 0;
	string command, arg1, arg2;
	bool gameActive = false;
	Board board, defaultBoard;
	unique_ptr<AI> whiteAI, blackAI;
	// Initial default board
	// White pieces
	defaultBoard.addPiece<Rook>(true, {"a1"});
	defaultBoard.addPiece<Knight>(true, {"b1"});
	defaultBoard.addPiece<Bishop>(true, {"c1"});
	defaultBoard.addPiece<Queen>(true, {"d1"});
	defaultBoard.addPiece<King>(true, {"e1"});
	defaultBoard.addPiece<Bishop>(true, {"f1"});
	defaultBoard.addPiece<Knight>(true, {"g1"});
	defaultBoard.addPiece<Rook>(true, {"h1"});
	// Black pieces
	defaultBoard.addPiece<Rook>(false, {"a8"});
	defaultBoard.addPiece<Knight>(false, {"b8"});
	defaultBoard.addPiece<Bishop>(false, {"c8"});
	defaultBoard.addPiece<Queen>(false, {"d8"});
	defaultBoard.addPiece<King>(false, {"e8"});
	defaultBoard.addPiece<Bishop>(false, {"f8"});
	defaultBoard.addPiece<Knight>(false, {"g8"});
	defaultBoard.addPiece<Rook>(false, {"h8"});
	// Pawns
	for (unsigned int i = 0; i < WIDTH; i++) {
		defaultBoard.addPiece<Pawn>(true, {i, 1}); // white
		defaultBoard.addPiece<Pawn>(false, {i, HEIGHT - 2}); // black
	}
	cout << "Do you want a Text Display, Graphical Display, or Both? (t/g/b): ";
	while (cin >> arg1) {
		toLowercase(arg1);
		if (arg1 == "t") {
			text = true;
			graphics = false;
			cout << "Text-based display selected." << endl << defaultBoard << endl;
			break;
		} else if (arg1 == "g") {
			text = false;
			graphics = true;
			cout << "Graphical display selected." << endl;
			xw = make_unique<XWindow>(1200, 1000);
			xw->drawBoard(defaultBoard);
			break;
		} else if (arg1 == "b") {
			text = graphics = true;
			cout << "Both displays selected." << endl << defaultBoard << endl;
			xw = make_unique<XWindow>(1200, 1000);
			xw->drawBoard(defaultBoard);
			break;
		}
		cout << endl << "Please input \"t\", \"g\", or \"b\": " << endl;
	}
	cout << "Command: ";
	while (cin >> command) {
		toLowercase(command);
		if (command == "game") {
			if (gameActive) {
				cerr << "Game is already active." << endl << (board.getTurn() ? "White" : "Black") << "'s turn: ";
				continue;
			}
			cin >> arg1 >> arg2;
			int p1 = parsePlayer(arg1);
			int p2 = parsePlayer(arg2);
			if (p1 < 0 || p2 < 0) {
				cerr << "Please input either \"human\" or \"computer[X]\" for the players" << endl;
				continue;
			}
			whiteAI = (p1 ? make_unique<AI>(board, true, p1) : nullptr);
			blackAI = (p2 ? make_unique<AI>(board, false, p2) : nullptr);
			board = defaultBoard;
			gameActive = true;
			board.runCalculations();
			cout << "The game has begun!" << endl;
		} else if (command == "resign") {
			if (!gameActive) {
				cerr << "Game is not active." << endl << "Command: ";
				continue;
			}
			(!board.getTurn() ? whiteWins : blackWins) += 1;
			cout << (!board.getTurn() ? "White" : "Black") << " wins!" << endl;
			gameActive = false;
		} else if (command == "move") {
			if (!gameActive) {
				cerr << "Game is not active." << endl << "Command: ";
				continue;
			}
			if (board.getTurn() && whiteAI) {
				board.movePiece(whiteAI->think());
			} else if (!board.getTurn() && blackAI) {
				board.movePiece(blackAI->think());
			} else {
				cin >> arg1 >> arg2;
				try {
					Posn start{arg1}, end{arg2};
					board.movePiece({start, end});
					// cerr << "we're here now" << endl;
					if (board[end]->getName() == (!board.getTurn() ? 'P' : 'p')) {
						// cerr << "uhhhhh promo!" << endl;
						shared_ptr<Pawn> tmp = static_pointer_cast<Pawn>(board[end]);
						if (tmp->canPromote()) {
							char piece;
							cin >> piece;
							bool white = 'B' <= piece && piece <= 'R';
							// cerr << "piece is " << piece << " and white is " << white << endl;
							if (white == board.getTurn()) {
								cerr << "Please input a valid piece type to promote into." << endl;
							} else {
								piece -= (white ? ('A' - 'a') : 0);
								// cerr << "piece is now " << piece << endl;
								switch (piece) {
									case 'n':
										// cerr << "about to promote to n" << endl;
										board.promote({start, end}, 1);
										break;
									case 'b':
										// cerr << "about to promote to b" << endl;
										board.promote({start, end}, 2);
										break;
									case 'r':
										// cerr << "about to promote to r" << endl;
										board.promote({start, end}, 3);
										break;
									case 'q':
										// cerr << "about to promote to q" << endl;
										board.promote({start, end}, 4);
										break;
									default:
										cerr << "Please input a valid piece type to promote into." << endl;
										board.undoMoves();
								}
							}
						}
					}
				} catch (BadPosn &e) {
					cerr << "Please input valid board coordinates." << endl
						 << (board.getTurn() ? "White" : "Black" ) << "'s move: ";
					continue;
				} catch (BadMove &e) {
					cerr << "Please input a valid move action." << endl
						 << (board.getTurn() ? "White" : "Black" ) << "'s move: ";
					continue;
				}
			}
			// cerr << "over here" << endl;
			switch (board.runCalculations()) {
				case 0:
					whiteWins += 0.5;
					blackWins += 0.5;
					gameActive = false;
					cout << "Stalemate!" << endl;
					break;
				case 1:
					cout << (board.getTurn() ? "White" : "Black") << " is in check." << endl;
					break;
				case 2:
					(!board.getTurn() ? whiteWins : blackWins) += 1;
					gameActive = false;
					cout << "Checkmate! " << (!board.getTurn() ? "White" : "Black") << " wins!" << endl;
					break;
			}
		} else if (command == "undo") {
			if (!gameActive) {
				cerr << "Game is not active." << endl << "Command: ";
				continue;
			}
			int num = 0;
			if (!(cin >> num)) continue;
			cerr << "Are you SURE you want to undo " << num << " moves? (y/n): ";
			while (cin >> arg1) {
				toLowercase(arg1);
				if (arg1 == "y") {
					board.undoMoves(num);
					break;
				} else if (arg1 == "n") {
					break;
				}
				cerr << endl << "Please input \"y\" or \"n\": " << endl;
			}
		} else if (command == "setup") {
			if (gameActive) {
				cerr << "Game is already active." << endl << (board.getTurn() ? "White" : "Black") << "'s turn: ";
				continue;
			}
			display(defaultBoard);
			cout << "Entered setup mode." << endl;
			cerr << "Command: ";
			while (cin >> command) {
				toLowercase(command);
				if (command == "+") { // add a piece to the board
					char piece;
					cin >> piece >> arg1;
					if (!verifyPiece(piece)) {
						cerr << "Please input a valid piece name." << endl << "Command: ";
						continue;
					}
					try {
						Posn p{arg1};
						defaultBoard.removePiece(p);
						bool white = 'B' <= piece && piece <= 'R';
						piece -= (white ? ('A' - 'a') : 0);
						switch (piece) {
							case 'p':
								defaultBoard.addPiece<Pawn>(white, p);
								break;
							case 'n':
								defaultBoard.addPiece<Knight>(white, p);
								break;
							case 'b':
								defaultBoard.addPiece<Bishop>(white, p);
								break;
							case 'r':
		defaultBoard.addPiece<Rook>(white, p);
								break;
							case 'q':
								defaultBoard.addPiece<Queen>(white, p);
								break;
							case 'k':
								if (defaultBoard.hasKing(white)) {
									throw BadSetup{"There cannot be two " + string(white ? "white" : "black") + " kings on the board at once!"
										+ "\n If you are trying to move a king, please remove it first then add it."};
								} else {
									defaultBoard.addPiece<King>(white, p);
									break;
								}
						}
					} catch (BadPosn &e) {
						cerr << "Please input valid board coordinates." << endl << "Command: ";
						continue;
					} catch (BadSetup &e) {
						cerr << e.why() << endl << "Command: ";
						continue;
					}
					display(defaultBoard);
				} else if (command == "-") { // remove a piece from the board
					cin >> arg1;
					try {
						Posn p{arg1};
						defaultBoard.removePiece(p);
						display(defaultBoard);
					} catch (BadPosn &e) {
						cerr << "Please input valid board coordinates." << endl;
					}
				} else if (command == "=") { // change default starting colour
					cin >> arg1;
					if (arg1 == "white" || arg1 == "w") {
						defaultBoard.setTurn(true);
						cout << "White will now have the first turn." << endl;
					} else if (arg1 == "black" || arg1 == "b") {
						defaultBoard.setTurn(false);
						cout << "Black will now have the first turn." << endl;
					} else {
						cerr << "Please input a valid colour." << endl;
					}
				} else if (command == "clear") {
					Board tmp;
					tmp.setTurn(defaultBoard.getTurn());
					defaultBoard = tmp;
					display(defaultBoard);
				} else if (command == "done") { // valid board setup
					try {
						defaultBoard.validate();
						display(defaultBoard);
						cout << "Board setup successful." << endl;
						break;
					} catch (BadSetup &e) {
						cerr << "Board setup invalid: " << e.why() << endl;
					}
				} else { // invalid command
					cerr << "Please input a valid setup command." << endl;
				} // switch
				cerr << "Command: ";
			} // while
		} else if (command == "replay") {
			/*
			COMMANDS FOR REPLAY
			next _/#/all -> play the next move
			       -> if at the end of the moves, do nothing
			prev _/#/all -> undo most recent move played
			     -> if at the beginning of the moves, do nothing
			done -> exit replay mode

      TODO
      - fix printing extra board in looping next/prev
			*/
			if (gameActive) {
				cerr << "You must not be currently playing a game to enter replay mode." << endl
					 << (board.getTurn() ? "White" : "Black") << "'s turn: ";
					 continue;
			} else if (!(whiteWins || blackWins)) {
				cerr << "You must have completed a game to enter replay mode." << endl << "Command: ";
				continue;
			} else if (board.getLog().size() == 0) {
				cerr << "Game log is empty, no moves to be reviewed." << endl << "Command: ";
				continue;
			}
			
			// Get game log
			auto gameLog = board.getLog();
      unsigned int currentMove = 0;

      display(defaultBoard);
			
			cout << "Replay Command: ";
			while (cin >> command) {
				Board replayBoard = defaultBoard;
				replayBoard.runCalculations();
				// Cleanup command input
				toLowercase(command);

				bool atStart = currentMove == 0 ? true : false;
				bool atEnd = currentMove == gameLog.size() ? true : false;

				if (command == "next") {
					// Get argument
					getline(cin, arg1);
					stripWhitespace(arg1);
					toLowercase(arg1);
					
					if (arg1.empty() || isInteger(arg1) || arg1 == "all") {
						// Get number argument
						int numMoves;
						if (arg1.empty()) {
							// If empty argument, default to 1 move
							// not 1 of each colour
							numMoves = 1;
						} else if (!isInteger(arg1)) {
							// All moves
							numMoves = gameLog.size() - currentMove + 1;
						} else {
							// Get user number of moves
							stringstream ss(arg1);
							ss >> numMoves;
						}

						bool atEndInside = false;
						
						// Iterate over the number of moves asked to run
						for (int i = 0; i < numMoves; ++i) {
              // Generate legal moves
				      replayBoard.runCalculations();

							// If already hit the end of the moves
							if (atEnd && atEndInside) continue;

							// Update hitEnd
							if (currentMove < gameLog.size()) atEnd = false;

							// Check if last move
							if (!atEnd) {
								// Get move
								Move tempMove = gameLog[currentMove];

                char pieceName = replayBoard[tempMove.oldPos]->getName();
                bool pieceColour = ('a' <= pieceName && pieceName <= 'z') ? true : false;
                char capturedName = tempMove.capture ? replayBoard[tempMove.newPos]->getName() : '!';
                Posn pieceOldPos = tempMove.oldPos;
                Posn pieceNewPos = tempMove.newPos;

								// Next move
								replayBoard.movePiece(move(tempMove));
								
                // Increment move
								if (!atEnd) {
									++currentMove;
								} else {
									// Reached end
									atEndInside = true;
								}

								// Display move
								display(replayBoard);
                
                // Display Move Info
                if (text) {
                  // Colour
                  cout << "Piece Moved : " << (pieceColour ? "Black " : "White ");
                  // Piece type
                  if (pieceName == 'p' || pieceName == 'P') cout << "Pawn";
                  else if (pieceName == 'r' || pieceName == 'R') cout << "Rook";
                  else if (pieceName == 'n' || pieceName == 'N') cout << "Knight";
                  else if (pieceName == 'b' || pieceName == 'B') cout << "Bishop";
                  else if (pieceName == 'q' || pieceName == 'Q') cout << "Queen";
                  else if (pieceName == 'k' || pieceName == 'K') cout << "King";
                  else cout << "Unknown, an error has occurred";
                  cout << endl;
                
                  // Initial position
                  cout << "Moved From " << static_cast<char>('a' + pieceOldPos.x) << pieceOldPos.y + 1 << endl;
                  cout << "Moved To " << static_cast<char>('a' + pieceNewPos.x) << pieceNewPos.y + 1 << endl;
                 
                  // Captured pieces
                  if (tempMove.capture) {
                    cout << "Captured " << (pieceColour ? "White " : "Black ");
                    if (capturedName == 'p' || capturedName == 'P') cout << "Pawn";
                    else if (capturedName == 'r' || capturedName == 'R') cout << "Rook";
                    else if (capturedName == 'n' || capturedName == 'N') cout << "Knight";
                    else if (capturedName == 'b' || capturedName == 'B') cout << "Bishop";
                    else if (capturedName == 'q' || capturedName == 'Q') cout << "Queen";
                    else if (capturedName == 'k' || capturedName == 'K') cout << "King";
                    else cout << "Unknown, an error has occurred";
                    cout << endl;
                  }
                } // if text based display
              } else {
								cerr << "At the last move." << endl;

								// Display move
								display(replayBoard);

								break;
							}

							// If reached the end of the log
							if (currentMove == gameLog.size()) atEnd = true;
						} // for (0 -> numMoves)
					} else {
						cerr << "Please input a valid next argument." << endl;
					}

					cout << "Replay Command: ";
				} else if (command == "prev") {
					// Get argument
					getline(cin, arg1);
					stripWhitespace(arg1);
					toLowercase(arg1);
					
					if (arg1.empty() || isInteger(arg1) || arg1 == "all") {
						// Get number argument
						int numMoves;
						if (arg1.empty()) {
							// If empty argument, default to 1 move
							// not 1 of each colour
							numMoves = 1;
						} else if (!isInteger(arg1)) {
							// All moves
							numMoves = currentMove + 1;
						} else {
							// Get user number of moves
							stringstream ss(arg1);
							ss >> numMoves;
						}
						bool atStartInside = false;
						
						// Iterate over the number of moves asked to run
						for (int i = 0; i < numMoves; ++i) {
              // If already hit the start of the moves
							if (atStart && atStartInside) continue;

							// Update hitStart
							if (currentMove > 0) atStart = false;

							// Check if last move
							if (!atStart) {
								// Undo move
								replayBoard.undoMoves(1);

                // Decrement move
								if (!atStart) {
									--currentMove;
								} else {
									// Reached start
									atStartInside = true;
								}

								// Display move
								display(replayBoard);

							} else {
								cerr << "At the first move." << endl;

								// Display move
								display(replayBoard);

								break;
							}

							// If reached the start of the log
							if (currentMove == 0) atStart = true;
						} // for (0 -> numMoves)
					} else {
						cerr << "Please input a valid next argument." << endl;
					}
        } else if (command == "done") {
					break;
				} else {
					cerr << "Please input a valid replay command." << endl;
				} // command == ****
			} // while (cin >> command)
		} else if (command == "simplereplay") {
			if (gameActive) {
				cerr << "You must not be currently playing a game to enter replay mode." << endl
					 << (board.getTurn() ? "White" : "Black") << "'s turn: ";
					 continue;
			} else if (!(whiteWins || blackWins)) {
				cerr << "You must have completed a game to enter replay mode." << endl << "Command: ";
				continue;
			}
			int num = -1;
			Board replayBoard;
			if (cin >> num) {
				if (num < 0) {
					cerr << "Please input a non-negative number." << endl << "Command: ";
					continue;
				}
				replayBoard = board; // Starting replayBoard from the back
			} else {
				replayBoard = defaultBoard; // Starting replayBoard from the front
			}
			auto it = (num >= 0 ? replayBoard.begin(board) : replayBoard.end(board));
			for (int i = 0; i < num; i++) {
				try {
					--it;
				} catch (BadMove &e) {
					cerr << "The last game is only " << i << " moves long." << endl;
				}
			}
			while (cin >> command) {
				toLowercase(command);
				if (command == "next") {
					try {
						++it;
					} catch (BadMove &e) {
						cerr << "You have reached the end, you cannot go further." << endl;
					}
				} else if (command == "back") {
					try {
						--it;
					} catch (BadMove &e) {
						cerr << "You are at the beginning, you cannot go back further." << endl;
					}
				} else if (command == "done") {
					break;
				} else {
					cerr << "Please input a valid replay command." << endl << "Command: ";
					continue;
				}
				display(*it);
				cerr << "Command: ";
			}
		} else {
			cerr << "Please input a valid command." << endl
				 << (gameActive ? string(board.getTurn() ? "White" : "Black") + "'s turn: " : "Command: ");
			continue;
		} // switch
		// cerr << "bruh" << endl;
		if (gameActive) {
			display(board);
			cerr << (board.getTurn() ? "White" : "Black") << "'s turn: ";
		} else {
			cerr << "Command: ";
		}
	} // while
	cout << endl << "Final Score:" << endl
		 << "White: " << whiteWins << endl
		 << "Black: " << blackWins << endl;
	return 0;
} // main

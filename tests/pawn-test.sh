#!/bin/bash
sleep 1

# Define the commands to be sent
xdotool type "game human human"
xdotool key Return
sleep 0.05
xdotool type "move a2 a4 "
xdotool key Return
sleep 0.05
xdotool type "move b7 b5 "
xdotool key Return
sleep 0.05
# Capture black pawn
xdotool type "move a4 b5"
xdotool key Return
sleep 0.05
xdotool type "move a7 a5"
xdotool key Return
sleep 0.05
# Capture black pawn in en paassant
xdotool type "move b5 a6"
xdotool key Return
sleep 0.05
xdotool type "move a8 a7"
xdotool key Return
sleep 0.05
xdotool type "move h2 h4"
xdotool key Return
sleep 0.05
xdotool type "move move"
xdotool key Return
sleep 0.05
xdotool type "move a7 b7"
xdotool key Return
sleep 0.05
xdotool type "move a6 a7"
xdotool key Return
sleep 0.05
xdotool type "move g7 g5"
xdotool key Return
sleep 0.05
# Promotion to white queen
xdotool type "move a7 a8 Q"
xdotool key Return
sleep 0.05
# Capture white pawn
xdotool type "move g5 h4"
xdotool key Return
sleep 0.05
xdotool type "move g2 g4"
xdotool key Return
sleep 0.05
# Capture white pawn in en passant
xdotool type "move h4 g3"
xdotool key Return
sleep 0.05
xdotool type "move g1 h3"
xdotool key Return
sleep 0.05
xdotool type "move g3 g2 "
xdotool key Return
sleep 0.05
xdotool type "move h3 g5"
xdotool key Return
sleep 0.05
# Promotion to black Queen
xdotool type "move g2 g1 Q"
xdotool key Return
sleep 0.05
xdotool type "move b7 a7"
xdotool key Return
xdotool type "move h1 g2"
xdotool key Return
sleep 0.05
xdotool type "move a7 a8"
xdotool key Return
sleep 0.05
xdotool type "move f7 f6"
xdotool key Return
sleep 0.05
xdotool type "move e2 e4"
xdotool key Return
sleep 0.05
xdotool type "move f6 f5"
xdotool key Return
sleep 0.05
xdotool type "move e4 e5"
xdotool key Return
sleep 0.05
# FAILURE TEST - SHOULD NOT BE ABLE TO EN PASSANT
xdotool type "move f5 e4"
xdotool key Return
sleep 0.05
xdotool type "move d7 d6"
xdotool key Return
sleep 0.05
# FAILURE TEST - SHOULD NOT BE ABLE TO EN PASSANT
xdotool type "move e5 e6"
xdotool key Return
sleep 0.05
# Caputure black pawn
xdotool type "move e5 d6"
xdotool key Return
sleep 0.05
# Capture white pawn
xdotool type "move c7 d6"
xdotool key Return
sleep 0.05
xdotool type "move c2 c4"
xdotool key Return
sleep 0.05
xdotool type "move e7 e5"
xdotool key Return
sleep 0.05
xdotool type "move d2 d3"
xdotool key Return
sleep 0.05
xdotool type "move d6 d5"
xdotool key Return
sleep 0.05
xdotool type "move b2 b3"
xdotool key Return
sleep 0.05
# Capture white pawn
xdotool type "move b5 c4"
xdotool key Return
sleep 0.05
# Capture black pawn
xdotool type "move b3 c4"
xdotool key Return
sleep 0.05
xdotool type "move e5 e4"
xdotool key Return
sleep 0.05
# Capture black pawn
xdotool type "move d3 e4"
xdotool key Return
sleep 0.05
xdotool type "move f2 f4"
xdotool key Return
sleep 0.05
# FAILURE TEST - SHOULD NOT BE ABLE TO MOVE FORWARD
xdotool type "move f5 f4"
xdotool key Return
sleep 0.05
xdotool type "move h7 h6"
xdotool key Return
sleep 0.05
# Capture black pawn
xdotool type "move e5 e6"
xdotool key Return
sleep 0.05
# Capture white knight
xdotool type "move h6 g5"
xdotool key Return
sleep 0.05
# Capture black pawn
xdotool type "move f4 g5"
xdotool key Return


#!/bin/bash
sleep 1

# Define the commands to be sent
xdotool type "game human human"
xdotool key Return
sleep 0.05
xdotool type "move a2 a4"
xdotool key Return
sleep 0.05
xdotool type "move b7 b5"
xdotool key Return
sleep 0.05
# Capture a black pawn
xdotool type "move a4 b5" 
xdotool key Return
sleep 0.05
xdotool type "move a7 a6"
xdotool key Return
sleep 0.05
xdotool type "move b5 b6"
xdotool key Return
sleep 0.05
xdotool type "move e7 e5"
xdotool key Return
sleep 0.05
xdotool type "move a1 a6"
xdotool key Return
sleep 0.05
# Capture white rook
xdotool type "move c8 a6"
xdotool key Return
sleep 0.05
xdotool type "move b6 b7"
xdotool key Return
sleep 0.05
xdotool type "move e5 e4"
xdotool key Return
sleep 0.05
xdotool type "move f2 f4"
xdotool key Return
sleep 0.05
# Capture white pawn En passant
xdotool type "move e4 f3"
xdotool key Return
sleep 0.05
# Capture black pawn
xdotool type "move e2 f3"
xdotool key Return
sleep 0.05
xdotool type "move b8 c6"
xdotool key Return
sleep 0.05
# Capture rook, Promote to Queen
#xdotool type "move b7 a8"
#xdotool key Return
#sleep 0.05
#xdotool type "resign"
#xdotool key Return
#sleep 0.05
#xdotool type "replay"
#xdotool key Return
#sleep 0.05
#xdotool type "next all"
#xdotool key Return
#sleep 0.05
#xdotool type "next"
#xdotool key Return
#sleep 0.05
#xdotool type "prev"
#xdotool key Return
#sleep 0.05
#xdotool type "prev"
#xdotool key Return
#sleep 0.05
#xdotool type "next 3"
#xdotool key Return
#sleep 0.05
#xdotool type "prev 3"
#xdotool key Return
#sleep 0.05
#xdotool type "next 10"
#xdotool key Return
#sleep 0.05
#xdotool type "prev 10"
#xdotool key Return
#sleep 0.05
#xdotool type "next all"
#xdotool key Return
#sleep 0.05
#xdotool type "prev all"
#xdotool key Return
#sleep 0.05
#xdotool type "next 3"
#xdotool key Return
#sleep 0.05
#xdotool type "prev 10"
#xdotool key Return
#sleep 0.05
#xdotool type "next"
#xdotool key Return
#sleep 0.05
#xdotool type "next 5"
#xdotool key Return

#!/bin/bash
sleep 1

# has both kings
# no pawns in promotion zones
# kings are not in check

# Define the commands to be sent
xdotool type "setup"
xdotool key Return
sleep 0.05
xdotool type "clear"
xdotool key Return
sleep 0.05
xdotool type "+ k a1"
xdotool key Return
sleep 0.05
xdotool type "+ K h8"
xdotool key Return
sleep 0.05
# FAILURE TEST - extra kings
xdotool type "+ K g8"
xdotool key Return
sleep 0.05
xdotool type "+ k b1"
xdotool key Return
sleep 0.05
xdotool type "+ K h7"
xdotool key Return
sleep 0.05
xdotool type "+ k a1"
xdotool key Return
sleep 0.05
# THESE MIGHT SEGFAULT
xdotool type "+ K h8"
xdotool key Return
sleep 0.05
xdotool type "+ k a1"
xdotool key Return
sleep 0.05
# Should be valid, 2 kings, no checks, no pawns in promotion zones
xdotool type "done"
xdotool key Return
sleep 0.05
xdotool type "setup"
xdotool key Return
sleep 0.05
xdotool type "- a1"
xdotool key Return
sleep 0.05
# FAILURE TEST - should not be allowed to exit, as only 1 king
xdotool type "done"
xdotool key Return
sleep 0.05
xdotool type "+ K a1"
xdotool key Return
sleep 0.05
xdotool type "- h8"
xdotool key Return
sleep 0.05
# FAILURE TEST - same as previous
xdotool type "done"
xdotool key Return
sleep 0.05
xdotool type "+ k h8"
xdotool key Return
sleep 0.05

# Pawn
xdotool type "+ p a8"
xdotool key Return
sleep 0.05
xdotool type "+ p b8"
xdotool key Return
sleep 0.05
xdotool type "+ p c8"
xdotool key Return
sleep 0.05
xdotool type "+ P b1"
xdotool key Return
sleep 0.05
xdotool type "+ P c1"
xdotool key Return
sleep 0.05
xdotool type "+ P d1"
xdotool key Return
sleep 0.05
# Should succeed
xdotool type "done"
xdotool key Return
sleep 0.05
xdotool type "setup"
xdotool key Return
sleep 0.05
xdotool type "+ P e8"
xdotool key Return
sleep 0.05
# FAILURE TEST - should not succeed as pawn in promotion zone
xdotool type "done"
xdotool key Return
sleep 0.05
xdotool type "+ p f1"
xdotool key Return
sleep 0.05
# FAILURE TEST - same as previous
xdotool type "done"
xdotool key Return
sleep 0.05
xdotool type "- e8"
xdotool key Return
sleep 0.05
# FAILURE TEST - same as previous
xdotool type "done"
xdotool key Return
sleep 0.05
xdotool type "- f1"
xdotool key Return
sleep 0.05
# should succeed as offending pawns have been removed
xdotool type "done"
xdotool key Return
sleep 0.05
xdotool type "setup"
xdotool key Return
sleep 0.05

# Checking
xdotool type "+ B d4"
xdotool key Return
sleep 0.05
# FAILURE TEST - black king in check
xdotool type "done"
xdotool key Return
sleep 0.05
xdotool type "+ b e5"
xdotool key Return
sleep 0.05
# Should succeed as bishops block eachother
xdotool type "done"
xdotool key Return
sleep 0.05
xdotool type "setup"
xdotool key Return
sleep 0.05
xdotool type "- d4"
xdotool key Return
sleep 0.05
# FAILUIRE TEST - white king in check
xdotool type "done"
xdotool key Return
sleep 0.05
# overwriting position, is valid command
xdotool type "+ B e5"
xdotool key Return
sleep 0.05
xdotool type "+ b d4"
xdotool key Return
sleep 0.05
# FAILURE TEST - both kings in check 
xdotool type "done"
xdotool key Return
sleep 0.05
xdotool type "+ Q c3"
xdotool key Return
sleep 0.05
xdotool type "+ q f6"
xdotool key Return
sleep 0.05
# should succeed as ally queens blocking bishops
xdotool type "done"
xdotool key Return
sleep 0.05
xdotool type "setup"
xdotool key Return
sleep 0.05
xdotool type "+ R h1"
xdotool key Return
sleep 0.05
# FAILURE TEST - white rook checking black king
xdotool type "done"
xdotool key Return
sleep 0.05
xdotool type "+ r h2"
xdotool key Return
sleep 0.05
# black rook defends
xdotool type "done"
xdotool key Return
sleep 0.05
xdotool type "setup"
xdotool key Return
sleep 0.05
xdotool type "+ n b3"
xdotool key Return
sleep 0.05
# FAILURE TEST - black knight checks white king
xdotool type "done"
xdotool key Return
sleep 0.05
xdotool type "+ N b3"
xdotool key Return
sleep 0.05
# should succeed white knight now where black knight was
xdotool type "done"
xdotool key Return
sleep 0.05

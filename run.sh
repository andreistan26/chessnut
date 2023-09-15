#!/bin/bash
cd build && ctest && xboard -fd "/home/andrei/Documents/chess_engine" -fcp "polyglot polyglot.ini" -nameOfDebugFile "logs.txt" -debug 

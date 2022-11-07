#!/bin/bash
cd build && ctest && xboard -fd "." -fcp "polyglot polyglot.ini" -nameOfDebugFile "logs.txt" -debug 

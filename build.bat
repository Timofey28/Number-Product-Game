@echo off
g++ src\main.cc -o Number-Product-Game
del src\main.exe
del src\main.o
rd /s /q src\Records
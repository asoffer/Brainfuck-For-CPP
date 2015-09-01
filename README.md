# Brainfuck-For-CPP
Leveraging C++ compilers to compile Brainfuck.

## What is this?
This is a stand-alone C++ header (weighing in at ~200 lines).
Including it allows you to write Brainfuck inside your C++ programs.

## Why?
There are some languages that you will always be fighting to make them do what you want.
Maybe the syntax is ugly.
Maybe it's hard to reason about the code.
Maybe it's just not wired for your particular project.
This is an attempt to bring the ease of writing Brainfuck programs to C++

## Couldn't you accomplish this with macros?
Yes, but macros are evil. This project uses no macros.

## Known bugs
* You cannot print the contents of a cell twice in a row.
The current work-around is to increment and then decrement the cell before printing again.

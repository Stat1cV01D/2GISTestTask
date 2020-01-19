## Test task for 2GIS
Test task in C++17 

## Description:
===========

Usage: `2GisTest <options>`  
Options are:

Option | Description
|:---|:---|
`-f <filename>` | Specifies the file to use for the task
`-m [word\|checksum]` | Program mode.<br/>- `word` prints the number of appearances of a word in the file, requires `-v`<br/>- `checksum` calculates 32bit checksum of the binary file
`-v <word>` | Specifies the word to search for in `word` mode
`-h` | Displays help

## Requirements:
============
- MSVC 2017 or higher (support for C++17)
- CMake 3.13 or higher

## Installation:
============

1. Clone this repository
2. Run CMake for the cloned folder  
   NOTE: do not use built-in MSVS support for CMake file because it might not create the `Debug_Checksum` or `Debug_Words` configurations
3. Select one of the above mentioned configurations to see the app in action

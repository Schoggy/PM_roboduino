formatter = clang-format
formatter_args = -i -style="LLVM"
src_robo = robo_main/robo_main.ino robo_main/robo_custom.h robo_main/robo_custom.ino robo_main/robo_pins.h
src_ctrl = ctrl_main/ctrl_main.ino


all: help

help:
	echo This makefile can clean this repository of unnecessary backup files
	echo and can format all sourcecode using the beautifier clang-format.
	echo ~
	echo Commands are: help, format, clean, help_clang_format
	echo ~
	
help_clang_format:
	echo You can get clang-format from here: http://llvm.org/releases/download.html
	echo This tool could also work on Windows (I only tested on Linux)
	echo For Linux simply extract the downloaded archive, move into it in a terminal and run this:
	echo "sudo cp -a bin lib share include /usr"
	
clean:
	rm -rf *~ ctrl_main/*~ robo_main/*~
	
format:
	$(formatter) $(formatter_args) $(src_robo) $(src_ctrl)
	
	

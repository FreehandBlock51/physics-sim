#!/bin/sh

# Tells grep to output matches with file name, line number, the 2 surrounding lines, and automatic coloring
grepcmd="grep -Hn --color=auto -A 1 -B 1"

check_todos() {
	$grepcmd "// !TODO" $1; # Use a ! to mark a todo as non-erroneous or in progress
	if $grepcmd -E "TODO\(\);|\/\/ TODO|@todo" $1; then
		echo "TODOs found in file $1!";
		return 1;
	else
		return 0;
	fi
}

found=0;
# hello
if [ -d "$1" ]; then
	echo "$1 is a directory, scanning for C source/header files...";
	prev_dir=$pwd;
	cd $1;
	for src in $(find -name *.c -or -name *.h); do
		check_todos $src;
		found=$(($found+$?));
	done
	cd $prev_dir;
	echo "Check complete";
elif [ -e "$1" ]; then
	check_todos $1;
	found=$?;
else
	echo "syntax: $0 <file/dir> [--no-error]";
	exit 37;
fi

if [ "$2" != "--no-error" ]; then
	exit $found;
fi
exit 0;

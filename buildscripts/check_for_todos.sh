#!/bin/sh

# Tells grep to output matches with file name, line number, the 2 surrounding lines, and automatic coloring
grepcmd="grep -Hn --color=auto -A 1 -B 1"

show_usage() {
	echo "syntax: $0 <file/dir> [--no-error] [--dry-run]";
}

# default values for flags
no_error=false;
dry_run=false;
# check flags
for arg in $2 $3; do
	if [ "$arg" = "--no-error" ]; then
		no_error=true;
	elif [ "$arg" = "--dry-run" ]; then
		dry_run=true;
	fi
done

check_todos() {
	$grepcmd "\/\/ !TODO|@!todo" $1; # Use a ! to mark a todo as non-erroneous or in progress
	if $grepcmd -E "TODO\(\);|\/\/ TODO|@todo" $1; then
		echo "TODOs found in file $1!";
		return 1;
	else
		return 0;
	fi
}

found=0;
if [ -d "$1" ]; then
	echo "$1 is a directory, scanning for C source/header files...";
	for src in $(find -wholename "$1/*.c" -or -wholename "$1*.h"); do
		if $dry_run; then
			echo "found file $src, ignoring because --dry-run specified..."
		else
			check_todos $src;
			found=$(($found+$?));
		fi
	done
	echo "Check complete";
elif [ -e "$1" ]; then
	check_todos $1;
	found=$?;
else
	show_usage;
	exit 37;
fi

if $no_error; then
	exit 0;
else
	exit $found;
fi

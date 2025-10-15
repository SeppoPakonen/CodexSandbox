#! /bin/sh
#
# Convert text files to cr/lf format


echo "Converting GS files to UNIX format..."

find . -type f "(" \
   -name "*.c" -o -name "*.cfg" -o -name "*.cpp" -o -name "*.dep" -o \
   -name "*.h" -o -name "*.ini" -o -name "*.in2" -o \
   -name "*.cfg" -o -name "*.sc" -o -name "*.txt" -o -name "*.sh" \
   ")" \
   -exec sh -c "echo {};
		mv {} _tmpfile;
		tr -d \\\r < _tmpfile > {};
		touch -r _tmpfile {}; 
		rm _tmpfile" \;

chmod +x *.sh

echo
echo "Done!"
echo
echo "Now run make."
echo

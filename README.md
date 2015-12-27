# Injector

This is a small utility program which injects bytes into a file at a specific offset. It supports table files for translating bytes.

## Building

	g++ -std=c++11 injector.cpp -o injector

## Usage

	injector [options] <file> <offset> <string>
	  -t <table>  sets the table file

The following example injects the string `Hello, World!` into a file at the offset `0xF000` using a table to translate the string into non-ASCII characters:

	injector -t table.tbl file.bin 0xF000 "Hello, World!"

The following demonstrates the table file format:

	# This is a comment
	00=A
	01=B
	02=C
	F0=1
	F1=2
	F2=3

Using this table, the string `A1B2C3` would translate into the byte sequence `00 F0 01 F1 02 F2`.

## License

This program is licensed under the GNU General Public License, version 3. See [COPYING](COPYING) for details.

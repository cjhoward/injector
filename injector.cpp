/*
 * Copyright (C) 2015  Christopher J. Howard
 *
 * This file is part of Injector.
 *
 * Injector is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Injector is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Injector.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <fstream>
#include <iostream>

void usage()
{
	std::cerr << "Usage: injector [options] <file> <offset> <string>\n"\
"  -t <table>  sets the table file\n";
}

int main(int argc, const char* argv[])
{
	const char* table_arg = nullptr;
	const char* file_arg = nullptr;
	const char* offset_arg = nullptr;
	const char* string_arg = nullptr;
	const char** arg_table[] =
	{
		&file_arg,
		&offset_arg,
		&string_arg
	};

	// Parse arguments
	int current_arg = 0;
	for (int i = 1; i < argc; ++i)
	{
		if (std::strcmp(argv[i], "-t") == 0)
		{
			if (i + 1 >= argc)
			{
				usage();
				return EXIT_FAILURE;
			}

			table_arg = argv[++i];
		}
		else
		{
			if (current_arg >= 3)
			{
				usage();
				return EXIT_FAILURE;
			}

			*(arg_table[current_arg++]) = argv[i];
		}
	}

	if (current_arg != 3)
	{
		usage();
		return EXIT_FAILURE;
	}

	// Create table
	char table[256];
	for (int i = 0; i < 256; ++i)
		table[i] = i;

	// Load table
	if (table_arg != nullptr)
	{
		std::ifstream table_file(table_arg, std::ios::in);
		if (!table_file.is_open())
		{
			std::cerr << "Failed to open table file \"" << table_arg << "\"" << std::endl;
			return EXIT_FAILURE;
		}

		std::string line;
		int first;
		char second;
		while (std::getline(table_file, line))
		{
			if (line.length() != 4 || line[0] == '#')
				continue;

			// Parse values
			std::sscanf(line.c_str(), "%x=%c", &first, &second);

			// Enter into table
			int index = (int)((unsigned char)second);
			table[index] = first;
		}

		table_file.close();
	}

	// Open file
	std::fstream file(file_arg, std::ios::in | std::ios::out | std::ios::binary);
	if (!file.is_open())
	{
		std::cerr << "Failed to open file \"" << file_arg << "\"" << std::endl;
		return EXIT_FAILURE;
	}

	// Seek to offset
	unsigned long offset = std::stoul(offset_arg, nullptr, 0);
	file.seekp(offset, file.beg);

	// Inject bytes
	std::size_t length = std::strlen(string_arg);
	for (std::size_t i = 0; i < length; ++i)
	{
		int index = (int)((unsigned char)string_arg[i]);	
		file.put(table[index]);
	}

	file.close();

	return EXIT_SUCCESS;
}

// Blago_parser.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <fstream>
#include <regex>
#include <iostream>
#include <string>

std::string fileToParse(std::string fpath = "")
{
	std::string _msg1 = "Enter log file to read: ";

	std::cout << _msg1;
	std::cin >> fpath;
	return fpath;
}

int main()
{
	
// TODO
// Open log files in sequence
	// Find matching text for DL and nonce/file
	// Output matches to CSV file with headers for easy mySQL import

	// Temporarily using just one file for testing
	std::string _filepath1 = fileToParse();

	std::fstream reader(_filepath1);

	// String example to match 
	// 19:56:23 found deadline=2936138 nonce=3895086637946408786 for account: 17903775311260693052 file: 17903775311260693052_3895086637946339176_69632
	// REGEX: ^[\\d:]+\\ found deadline=(\\d+)\\ nonce=(\\d+)\\ for\\ account:\\ (\\d+)\\ file:\\ (\\d+_\\d+_\\d+)$

	while (!reader) 
	{
		std::cout << "Error reading file, is the path correct?" << std::endl;
		_filepath1 = fileToParse(_filepath1);
		std::fstream reader(_filepath1);
		if (reader) break;
	}
	
	std::string logline;
	std::regex lines();
	unsigned int count = 0;
	std::string header = "dl,nonce,file";
	std::string _line;
	std::string output = header;

	// open a file for writing/appending
	std::string _csvfile = _filepath1.substr(0, _filepath1.find_last_of('.')) + ".csv";
	std::ofstream writer(_csvfile, std::ios::app);

	if (!writer)
	{
		std::cout << "Error opening file for writing" << std::endl;
		std:system("Pause");
		return -1;
	}

	writer << header << std::endl;

	std::cout << header << std::endl;

	while ( ! reader.eof() )
	{
		// Get next line
		std::getline(reader, logline);

		try { // Need to confirm length of nonces,accounts etc
			std::regex re("^\\d{2}:\\d{2}:\\d{2}\\ found\\ deadline=(\\d{1,10})\\ nonce=(\\d{1,20})\\ for\\ account:\\ \\d{16,32}\\ file:\\ (\\d{16,32}_\\d+_\\d+)");
			std::smatch match;
			if (std::regex_search(logline, match, re) && match.size() == 4)
			{
				++count;
				_line = match.str(1) + "," + match.str(2) + "," + match.str(3); // dl,nonce,file

				std::cout << _line << std::endl;
				writer << _line << std::endl;
			}
		}
		catch (std::regex_error& e) {
			std::cout << "Error in regex " << std::endl;
			std::system("pause");
			return -1;
		}
	}

	// Numbers of lines containing a DL
	std::cout << count + " Lines found." << std::endl;

	// Close files
	writer.close();
	reader.close();
	
	std::system("pause");
    return 0;
}


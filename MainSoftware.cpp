#include "MainSoftware.hpp"


#include <iostream>

int main (int argc, char *argv[])
{	
	BMPReader::BitmapReader input_file("test_case_1.bmp");
	input_file.openBMP();

	BMPFormat::Bitmap bitmap_struct;
	input_file.readAndFillBitmapStruct(bitmap_struct);

	input_file.closeBMP();

	BMPDisplay::BitmapDisplay outputer(bitmap_struct);
	outputer.displayBMP(bitmap_struct);

	std::cin.get();

	return 0;
}
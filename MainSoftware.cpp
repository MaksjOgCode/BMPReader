#include "MainSoftware.hpp"

int main (int argc, char *argv[])
{	
	BMPReader::BitmapReader input_file("test_case_1.bmp");
	input_file.openBMP();

	BMPFormat::BitmapStruct bitmap_struct;
	input_file.fillBMPStruct(bitmap_struct);

	std::cout << bitmap_struct.bmp_file_header.bf_type			<< std::endl;
	std::cout << bitmap_struct.bmp_file_header.bf_size			<< std::endl;
	std::cout << bitmap_struct.bmp_file_header.bf_off_bits	<< std::endl;

	bitmap_struct.bmp_dib_header.

	input_file.closeBMP();

	return 0;
}
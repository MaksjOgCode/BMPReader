#include "MainSoftware.hpp"



int main (int argc, char *argv[])
{	
	if (argc != 2)
	{
		std::cerr << "Usage: " << argv[0] << " <path_to_bmp_file>" << std::endl;
		return 1;
	}

	try
	{
		BMPReader::Reader reader_object(argv[1]);
		reader_object.openBMP();

		BMPFormat::Bitmap input_bitmap_struct;

		reader_object.readAndFillBMPStruct(input_bitmap_struct);
		reader_object.closeBMP();

		BMPDisplay::BitmapDisplayConsole::displayBMP(input_bitmap_struct);
	}
	catch( const std::invalid_argument &exception )
	{
		std::cerr << exception.what() << "\n";
	}

	std::cin.get();

	return 0;\
}
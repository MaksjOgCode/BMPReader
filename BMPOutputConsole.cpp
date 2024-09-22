#include "BMPOutputConsole.hpp"



void SetConsoleColor(WORD attributes)
{
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(hConsole, attributes);
}

void BMPDisplay::BitmapDisplay::displayBMP(const BMPFormat::Bitmap &struct_object) const
{
	display_BMP_Information(struct_object);
}

void BMPDisplay::BitmapDisplay::display_BMP_Information(const BMPFormat::Bitmap &struct_object) const
{
	/* Type file: */
	if (struct_object.bmp_file_header.bf_type == H_IS_BMP_FORMAT)
	{
		H_SWITCH_RED
		std::cout << std::hex << "Type File: ";
		H_SWITCH_GREEN
		std::cout << ".BMP" << "\n";
	}
	
	/* Size file: */
	if (struct_object.bmp_file_header.bf_size < 1024)
	{
		H_SWITCH_RED
		std::cout << "Size file: (in bytes): ";
		H_SWITCH_GREEN
		std::cout << std::dec << struct_object.bmp_file_header.bf_size << " (Bytes)" << "\n";
	}
	else if ( struct_object.bmp_file_header.bf_size > 1024 )
	{
		H_SWITCH_RED
		std::cout << "Size file: ";
		H_SWITCH_GREEN
		std::cout << std::dec << (struct_object.bmp_file_header.bf_size) / 1024 << "KB" << "\n";
	}

	unsigned char version_bmp_file = struct_object.getVersion();


	switch(version_bmp_file)
	{
	case H_VERSION_DIB_CORE_HEADER:
		H_SWITCH_RED
		std::cout << "Resolution .bmp image: ";
		H_SWITCH_GREEN
		std::cout << struct_object.bmp_dib_header.bmp_core_header.bcWidth << " x " << struct_object.bmp_dib_header.bmp_core_header.bcHeight << "\n";

		H_SWITCH_RED
		std::cout << "Color depth: ";
		H_SWITCH_GREEN
		std::cout << struct_object.bmp_dib_header.bmp_core_header.bcBitCount << "\n";
		break;

	case H_VERSION_DIB_INFO_HEADER:
		H_SWITCH_RED
		std::cout << "Resolution .bmp image: ";
		H_SWITCH_GREEN
		std::cout << struct_object.bmp_dib_header.bmp_info_header.biWidth << " x " << struct_object.bmp_dib_header.bmp_info_header.biHeight << "\n";

		H_SWITCH_RED
		std::cout << "Color depth: ";
		H_SWITCH_GREEN
		std::cout << struct_object.bmp_dib_header.bmp_info_header.biBitCount << "\n";
		break;

	case H_VERSION_DIB_V4_HEADER:
		H_SWITCH_RED
		std::cout << "Resolution .bmp image: ";
		H_SWITCH_GREEN
		std::cout << struct_object.bmp_dib_header.bmp_v4_header.bV4Width << " x " << struct_object.bmp_dib_header.bmp_v4_header.bV4Height << "\n";

		H_SWITCH_RED
		std::cout << "Color depth: ";
		H_SWITCH_GREEN
		std::cout << struct_object.bmp_dib_header.bmp_v4_header.bV4BitCount << "\n";
		break;

	case H_VERSION_DIB_V5_HEADER:
		H_SWITCH_RED
		std::cout << "Resolution .bmp image: ";
		H_SWITCH_GREEN
		std::cout << struct_object.bmp_dib_header.bmp_v5_header.bV5Width << " x " << struct_object.bmp_dib_header.bmp_v5_header.bV5Height << "\n";

		H_SWITCH_RED
		std::cout << "Color depth: ";
		H_SWITCH_GREEN
		std::cout << struct_object.bmp_dib_header.bmp_v5_header.bV5BitCount << "\n";
		break;

	default:
		std::cout << "Not found format:" << "\n";
		break;
	}

	return;

}
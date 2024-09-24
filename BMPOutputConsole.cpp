#include "BMPOutputConsole.hpp"



//--------------------------------------------------------------------------------------------------------------
void BMPDisplay::BitmapDisplayConsole::displayBMP(const BMPFormat::Bitmap &bitmap_struct_object)
{
#if defined(_WIN32) || defined(_WIN64)
	HWND consoleWindow = GetConsoleWindow();
	if (consoleWindow != NULL)
		ShowWindow(consoleWindow, SW_MAXIMIZE);
#endif


	display_BMP_Pixel_Data(bitmap_struct_object);
	display_BMP_Information(bitmap_struct_object);
}
//--------------------------------------------------------------------------------------------------------------
void BMPDisplay::BitmapDisplayConsole::display_BMP_Pixel_Data(const BMPFormat::Bitmap &bitmap_struct_object)
{
	if ( bitmap_struct_object.bmp_data_pixel.empty() )
	{
		throw std::invalid_argument("Pixel Data into .bmp file is empty: [BMPDisplay::BitmapDisplayConsole -3]");
		return;
	}

	uint64_t width_image   {};
	uint64_t height_image  {};
	uint8_t  clr_bit_count {};
	switch ( bitmap_struct_object.getVersion() )
	{
	case H_VERSION_DIB_CORE_HEADER:
		width_image   = bitmap_struct_object.bmp_dib_header.bmp_core_header.bc_width;
		height_image  = bitmap_struct_object.bmp_dib_header.bmp_core_header.bc_height;
		clr_bit_count = bitmap_struct_object.bmp_dib_header.bmp_core_header.bc_bit_count;
		break;
	case H_VERSION_DIB_INFO_HEADER:
		width_image   = bitmap_struct_object.bmp_dib_header.bmp_dib_info_header.bi_width;
		height_image  = bitmap_struct_object.bmp_dib_header.bmp_dib_info_header.bi_height;
		clr_bit_count = bitmap_struct_object.bmp_dib_header.bmp_dib_info_header.bi_bit_count;
		break;
	case H_VERSION_DIB_V4_HEADER:
		width_image   = bitmap_struct_object.bmp_dib_header.bmp_dib_v4_header.bv4_width;
		height_image  = bitmap_struct_object.bmp_dib_header.bmp_dib_v4_header.bv4_height;
		clr_bit_count = bitmap_struct_object.bmp_dib_header.bmp_dib_v4_header.bv4_bit_count;
		break;
	case H_VERSION_DIB_V5_HEADER:
		width_image   = bitmap_struct_object.bmp_dib_header.bmp_dib_v5_header.bv5_width;
		height_image  = bitmap_struct_object.bmp_dib_header.bmp_dib_v5_header.bv5_height;
		clr_bit_count = bitmap_struct_object.bmp_dib_header.bmp_dib_v5_header.bv5_bit_count;
		break;
	default:
		std::cout << "Unknown value";
		return;
	}

	if (width_image > 128 || height_image > 64)
	{
		throw std::invalid_argument("High image resolution - the console does not support this resolution [ONLY 128 x 64]: [BMPDisplay::BitmapDisplayConsole -4]");
		return;
	}

	/* ONLY 24 BYTES || 32 BYTES */
	const uint8_t bits = 8;
	uint8_t bit_to_byte = clr_bit_count;
	if (bit_to_byte % bits == 0)
	{
		if (clr_bit_count != 24 && clr_bit_count != 32)
			return;


		size_t index = bitmap_struct_object.bmp_data_pixel.size();
		for (size_t i = 0, cursor = index - width_image;     i < height_image;     ++i)
		{
			for (size_t j = 0;     j < width_image;     ++j, --index, ++cursor)
			{
				if (i != 0 && cursor % width_image == 0)
				{
					if (cursor != 0)
						cursor -= width_image + width_image;
				}

				if (cursor >= bitmap_struct_object.bmp_data_pixel.size() && cursor <= 0)
					break;

				std::cout << "\033[38;2;" << int(bitmap_struct_object.bmp_data_pixel[cursor][2]) << ";" << int(bitmap_struct_object.bmp_data_pixel[cursor][1]) << ";" << int(bitmap_struct_object.bmp_data_pixel[cursor][0]) << "mO\033[0m";
			}
			std::cout << "\n";
		}
	}

	return;
}
//--------------------------------------------------------------------------------------------------------------
void BMPDisplay::BitmapDisplayConsole::display_BMP_Information(const BMPFormat::Bitmap &bitmap_struct_object) 
{
	if ( bitmap_struct_object.getVersion() == 0b0000)
	{
		throw std::invalid_argument("Unknown version .bmp file: [BMPDisplay::BitmapDisplayConsole -1]");
		return;
	}

	if (bitmap_struct_object.bmp_file_header.bf_size == 0)
	{
		throw std::invalid_argument(".bmp file is empty : [BMPDisplay::BitmapDisplayConsole -2]");
		return;
	}

	std::cout << "\n";
	// R-72, G-209, B-204 (colour: MediumTurquoise)
	std::cout << "\033[38;2;72;209;204m" << "File Type: " << "\033[0m";
	display_BMP_Information_Type(bitmap_struct_object);
	std::cout << "\t";
	// R-72, G-209, B-204 (colour: MediumTurquoise)
	std::cout << "\033[38;2;72;209;204m" << "File Size: " << "\033[0m";
	display_BMP_Information_Size(bitmap_struct_object);
	std::cout << "\t";
	// R-72, G-209, B-204 (colour: MediumTurquoise)
	std::cout << "\033[38;2;72;209;204m" << "Bit Count per Pixel: " << "\033[0m";
	display_BMP_Information_Bit_Count(bitmap_struct_object);

	std::cout << "\t";
	/* Developer's signature */
	std::cout << "\033[38;2;72;209;204m" << "BMPReader: " << "\033[0m";
	std::cout << "\033[38;2;245;222;179m" << "By Maksj.OG" << "\033[0m";

	return;
}
//--------------------------------------------------------------------------------------------------------------
void BMPDisplay::BitmapDisplayConsole::display_BMP_Information_Type(const BMPFormat::Bitmap &bitmap_struct_object)
{
	/*Type File: */
	/* R-245, G-222, B-179 (colour: Wheat) */
	std::cout << std::hex << "\033[38;2;245;222;179m" << '('  << "0x" << bitmap_struct_object.bmp_file_header.bf_type << ')' << " .bmp" << "\033[0m";

	return;
}
//--------------------------------------------------------------------------------------------------------------
void BMPDisplay::BitmapDisplayConsole::display_BMP_Information_Size(const BMPFormat::Bitmap &bitmap_struct_object)
{
	uint64_t bf_size_in_bytes = std::stoul(std::to_string(bitmap_struct_object.bmp_file_header.bf_size), nullptr, 10);
	
	/* Size File: */
	/* R-245, G-222, B-179 (colour: Wheat) */
	if (bf_size_in_bytes > 1000)
		std::cout << std::dec << "\033[38;2;245;222;179m" << (bitmap_struct_object.bmp_file_header.bf_size / 1024) << " kB" << "\033[0m";
	else if (bf_size_in_bytes > 1000000)
		std::cout << std::dec << "\033[38;2;245;222;179m" << (bitmap_struct_object.bmp_file_header.bf_size / 1024) << " MB" << "\033[0m";
	else if (bf_size_in_bytes > 1000000000)
		std::cout << std::dec << "\033[38;2;245;222;179m" << (bitmap_struct_object.bmp_file_header.bf_size / 1024) << " GB" << "\033[0m";
	else
		std::cout << std::dec << "\033[38;2;245;222;179m" << (bitmap_struct_object.bmp_file_header.bf_size) << " Bytes" << "\033[0m";

	return;
}
//--------------------------------------------------------------------------------------------------------------
void BMPDisplay::BitmapDisplayConsole::display_BMP_Information_Bit_Count(const BMPFormat::Bitmap &bitmap_struct_object)
{
	/* Bit Count Per Pixel: */
	/* R-245, G-222, B-179 (colour: Wheat) */
	switch ( bitmap_struct_object.getVersion() )
	{
	case H_VERSION_DIB_CORE_HEADER:
		std::cout << "\033[38;2;245;222;179m" << bitmap_struct_object.bmp_dib_header.bmp_core_header.bc_bit_count << " PX" << "\033[0m";
		break;

	case H_VERSION_DIB_INFO_HEADER:
		std::cout << "\033[38;2;245;222;179m" << bitmap_struct_object.bmp_dib_header.bmp_dib_info_header.bi_bit_count << " PX" << "\033[0m";
		break;

	case H_VERSION_DIB_V4_HEADER:
		std::cout << "\033[38;2;245;222;179m" << bitmap_struct_object.bmp_dib_header.bmp_dib_v4_header.bv4_bit_count << " PX" << "\033[0m";
		break;

	case H_VERSION_DIB_V5_HEADER:
		std::cout << "\033[38;2;245;222;179m" << bitmap_struct_object.bmp_dib_header.bmp_dib_v5_header.bv5_bit_count << " PX" << "\033[0m";
		break;

	default:
		std::cout << "\033[38;2;245;222;179m" << "Unknown value" << "\033[0m";
		return;
	}

	return;
}
//--------------------------------------------------------------------------------------------------------------
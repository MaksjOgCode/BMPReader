#ifndef BMPOUTPUTCONSOLE_HPP
#define BMPOUTPUTCONSOLE_HPP



#if defined(_WIN32) || defined(_WIN64)
#include <Windows.h>
#endif

#include <iostream>
#include <string>
#include "BMPFormat.hpp"



namespace BMPDisplay
{
	class BitmapDisplayConsole
	{
	private:
		BitmapDisplayConsole() = delete;
		BitmapDisplayConsole(const BitmapDisplayConsole&) = delete;
		BitmapDisplayConsole& operator=(const BitmapDisplayConsole&) = delete;
	public:
		static void displayBMP(const BMPFormat::Bitmap &bitmap_struct_object);
	private:
		static void display_BMP_Pixel_Data(const BMPFormat::Bitmap &bitmap_struct_object);
	private:
		static void display_BMP_Information(const BMPFormat::Bitmap &bitmap_struct_object);
		static void display_BMP_Information_Type(const BMPFormat::Bitmap &bitmap_struct_object);
		static void display_BMP_Information_Size(const BMPFormat::Bitmap &bitmap_struct_object);
		static void display_BMP_Information_Bit_Count(const BMPFormat::Bitmap &bitmap_struct_object);
	};
};



#endif /* BMPOUTPUTCONSOLE_HPP! */

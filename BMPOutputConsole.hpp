#ifndef BMPOUTPUTCONSOLE_HPP
#define BMPOUTPUTCONSOLE_HPP



#include <iostream>
#include <Windows.h>
#include "BMPFormat.hpp"
#include "BMPReader.hpp"



namespace BMPDisplay
{
	class BitmapDisplay
	{
	public:
		BitmapDisplay(const BMPFormat::Bitmap &struct_object)
		{
			HWND consoleWindow = GetConsoleWindow();
			if (consoleWindow != NULL)
			{
				ShowWindow(consoleWindow, SW_MAXIMIZE);
			}
		}

		void displayBMP(const BMPFormat::Bitmap &struct_object) const;

		void display_BMP_Information(const BMPFormat::Bitmap &struct_object) const;
	private:
	};
};



#endif /* BMPOUTPUTCONSOLE_HPP! */

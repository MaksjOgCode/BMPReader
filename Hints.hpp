#ifndef HINTS_HPP
#define HINTS_HPP



#define H_BRAA   BitmapReaderAlgorithmsAPI

	/* A mark to distinguish the format from others. Must contain 4D42[16]/424D[16]: */
#define H_IS_BMP_FORMAT		0x4D42

/* Count bytes: */
#define H_BITMAPFILEHEADER	0x000E
/* Count bytes: */
#define H_BITMAPCOREHEADER	0x000C
/* Count bytes: */
#define H_BITMAPINFOHEADER	0x0028
/* Count bytes: */
#define H_BITMAPV4HEADER	0x006C
/* Count bytes: */
#define H_BITMAPV5HEADER	0x007C



#define H_VERSION_DIB_CORE_HEADER	0b0001
#define H_VERSION_DIB_INFO_HEADER	0b0010
#define H_VERSION_DIB_V4_HEADER		0b0100
#define H_VERSION_DIB_V5_HEADER		0b1000



#define H_SWITCH_GREEN 		SetConsoleColor(FOREGROUND_GREEN | FOREGROUND_INTENSITY);
#define H_SWITCH_RED			SetConsoleColor(FOREGROUND_RED  | FOREGROUND_INTENSITY);

#endif /* HINTS_HPP! */
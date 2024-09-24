#ifndef HINTS_HPP
#define HINTS_HPP



#define H_BRAA   BitmapReaderAlgorithmsAPI



/* ************************************************************ */
/* A mark to distinguish the format from others. Must contain 4D42[16]/424D[16]: */
#define H_IS_BMP_FORMAT		0x4D42
/* ************************************************************ */



/* ************************************************************ */
/* Count bytes: */
#define H_BITMAP_FILE_HEADER		0x000E
#define H_BITMAP_DIB_CORE_HEADER	0x000C
#define H_BITMAP_DIB_INFO_HEADER	0x0028
#define H_BITMAP_DIB_V4_HEADER	0x006C
#define H_BITMAP_DIB_V5_HEADER	0x007C
/* ************************************************************ */



/* ************************************************************ */
/* Flag DIB Header version: */
#define H_VERSION_DIB_CORE_HEADER	0b0001
#define H_VERSION_DIB_INFO_HEADER	0b0010
#define H_VERSION_DIB_V4_HEADER		0b0100
#define H_VERSION_DIB_V5_HEADER		0b1000
/* ************************************************************ */



/* ************************************************************ */
/* In BMP, only 1 is allowed. This is used in icons and pictures: */
#define H_COLOR_PLANE					0x01
/* ************************************************************ */



/* ************************************************************ */
/* Compression: */

/* Two-dimensional array: */
#define H_BI_RGB					0x00

/* RLE encoding: */
#define H_BI_RLE8					0x01

/* RLE encoding: */
#define H_BI_RLE4					0x02

/* Two-dimensional array with color channel masks: */
#define H_BI_BITFIELDS			0x03

/* In the embedded JPEG file: */
#define H_BI_JPEG					0x04

/* In the embedded PNG file: */
#define H_BI_PNG					0x05

/* Two-dimensional array with color and alpha channel masks: */
#define H_BI_ALPHABITFIELDS	0x06
/* ************************************************************ */



/* ************************************************************ */
/* bi_ñlr_used || bv4_ñlr_used || bv5_ñlr_used: */
#define H_USED_CLR	0x00
/* ************************************************************ */



/* ************************************************************ */
/* bi_ñlr_used || bv4_ñlr_used || bv5_ñlr_used: */
#define H_ALL_IMPORTANT_CLR	0x00
/* ************************************************************ */

#define H_SWITCH_GREEN 		SetConsoleColor(FOREGROUND_GREEN | FOREGROUND_INTENSITY);
#define H_SWITCH_RED			SetConsoleColor(FOREGROUND_RED  | FOREGROUND_INTENSITY);

#endif /* HINTS_HPP! */
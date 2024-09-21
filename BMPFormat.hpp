#ifndef BMPFORMAT_HPP
#define BMPFORMAT_HPP



#include <vector>



namespace BMPFormat
{
	struct BitmapStruct
	{
		//****************************************************************************************************
	private:
		struct Bitmap_File_Header
		{
		public:
					uint16_t bf_type				{};		/* Signature:										*/
					uint32_t bf_size				{};		/* File Size (in bytes):						*/
					uint32_t bf_off_bits			{};		/* Bytes begining PixelArray:					*/
		private:			   
					uint16_t BF_RESERVED1	=	0x0000;	/* Reserved - must contained NULL:			*/
					uint16_t BF_RESERVED2	=	0x0000;	/* Reserved - must contained NULL:			*/
		};
		//****************************************************************************************************
	private:
		struct CIEXYZ
		{
			float ciexyzX;
			float ciexyzY;
			float ciexyzZ;
		};

		struct CIEXYZTRIPLE
		{
			CIEXYZ ciexyzRed;
			CIEXYZ ciexyzGreen;
			CIEXYZ ciexyzBlue;
		};
	private:
		struct Bitmap_DIB_Header
		{
		//****************************************************************************************************
		private:
			struct Bitmap_CORE_Header
			{
			public:
				uint32_t bcSize		{};	/* The size of this structure in bytes:																									*/
				uint16_t	bcWidth		{};	/* The width (bcWidth) of the raster in pixels. Specified as an unsigned integer. The value 0 is not documented:	*/
				uint16_t	bcHeight		{};	/* The height (bcHeight) of the raster in pixels. Specified as an unsigned integer. The value 0 is not documented:*/
				uint16_t	bcPlanes		{};	/* In BMP, only the value 1 is allowed. This field is used in Windows icons and cursors:									*/
				uint16_t	bcBitCount	{};	/* Number of bits per pixel:																													*/
			};
		//****************************************************************************************************
		private:
			struct Bitmap_INFO_Header
			{
			public:
				uint32_t biSize				{};	/* The size of this structure in bytes, which also indicates the version of the structure:											*/
				int32_t	biWidth				{};	/* The width of the raster in pixels. Specified as a signed integer. Zero and negative are not documented:						*/
				int32_t	biHeight				{};	/* Two parameters: the height of the raster in pixels and the order of the rows in two-dimensional arrays:						*/
				uint16_t	biPlanes				{};	/* In BMP, only the value 1 is allowed. This field is used in Windows icons and cursors:												*/
				uint16_t biBitCount			{};	/* Number of bits per pixel:																																*/
				uint32_t	biCompression		{};	/* Indicates the way pixels are stored:																												*/
				uint32_t	biSizeImage			{};	/* The size of the pixel data in bytes. It can be reset to zero if the storage is carried out by a two-dimensional array:	*/
				int32_t	biXPelsPerMeter	{};	/* The number of pixels per meter horizontally and vertically:																					*/
				uint32_t	biClrUsed			{};	/* The size of the color table in cells:																												*/
				uint32_t	biClrImportant		{};	/* The number of cells from the beginning of the color table to the last one used (including itself):								*/
			};
		//****************************************************************************************************
		private:
			struct Bitmap_V4_Header
			{
			public:
				uint32_t			bV4Size				{};	/* The size of this structure in bytes, which also indicates the version of the structure:																													*/
				int32_t			bV4Width				{};	/* The width of the raster in pixels. Specified as a signed integer. Zero and negative are not documented:																								*/
				int32_t			bV4Height			{};	/* Two parameters: the height of the raster in pixels and the order of the rows in two-dimensional arrays:																								*/
				uint16_t			bV4Planes			{};	/* In BMP, only the value 1 is allowed. This field is used in Windows icons and cursors:																														*/
				uint16_t			bV4BitCount			{};	/* Number of bits per pixel:																																																		*/
				uint32_t			bV4V4Compression	{};	/* Indicates the way pixels are stored:																																														*/
				uint32_t			bV4SizeImage		{};	/* The size of the pixel data in bytes. It can be reset to zero if the storage is carried out by a two-dimensional array:																			*/
				int32_t			bV4XPelsPerMeter	{};	/* The number of pixels per meter horizontally and vertically:																																							*/
				uint32_t			bV4ClrUsed			{};	/* The size of the color table in cells:																																														*/
				uint32_t			bV4ClrImportant	{};	/* The number of cells from the beginning of the color table to the last one used (including itself):																										*/
				uint32_t			bV4RedMask			{};	/* Bitmasks for extracting channel values: RED intensity:																																								*/
				uint32_t			bV4GreenMask		{};	/* Bitmasks for extracting channel values: GREEN intensity:																																								*/
				uint32_t			bV4BlueMask			{};	/* Bitmasks for extracting channel values: BLUE intensity:																																								*/
				uint32_t			bV4AlphaMask		{};	/* Bitmasks for extracting channel values: ALPHA channels:																																								*/
				uint32_t			bV4CSType			{};	/* A view of the color space:																																																		*/
				CIEXYZTRIPLE	bV4Endpoints		{};	/* The value of the field is taken into account only if the Tour field contains 0 (LCS_CALIBRATED_RGB). Then the endpoints are indicated in this field:									*/
				uint32_t			bV4GammaRed			{};	/* The value of the field is taken into account only if the Type field contains 0 (LCS_CALIBRATED_RGB). Then the gamma value for the RED component is indicated in the field:		*/
				uint32_t			bV4GammaGreen		{};	/* The value of the field is taken into account only if the Type field contains 0 (LCS_CALIBRATED_RGB). Then the gamma value for the GREEN component is indicated in the field:	*/
				uint32_t			bV4GammaBlue		{};	/* The value of the field is taken into account only if the Type field contains 0 (LCS_CALIBRATED_RGB). Then the gamma value for the BLUE component is indicated in the field:	*/
			};
		//****************************************************************************************************
		private:
			struct Bitmap_V5_Header
			{
			public:
				uint32_t			bV5Size				{};	/* The size of this structure in bytes, which also indicates the version of the structure:																													*/
				int32_t			bV5Width				{};	/* The width of the raster in pixels. Specified as a signed integer. Zero and negative are not documented:																								*/
				int32_t			bV5Height			{};	/* Two parameters: the height of the raster in pixels and the order of the rows in two-dimensional arrays:																								*/
				uint16_t			bV5Planes			{};	/* In BMP, only the value 1 is allowed. This field is used in Windows icons and cursors:																														*/
				uint16_t			bV5BitCount			{};	/* Number of bits per pixel:																																																		*/
				uint32_t			bV5Compression		{};	/* Indicates the way pixels are stored:																																														*/
				uint32_t			bV5SizeImage		{};	/* The size of the pixel data in bytes. It can be reset to zero if the storage is carried out by a two-dimensional array:																			*/
				int32_t			bV5XPelsPerMeter	{};	/* The number of pixels per meter horizontally and vertically:																																							*/
				uint32_t			bV5ClrUsed			{};	/* The size of the color table in cells:																																														*/
				uint32_t			bV5ClrImportant	{};	/* The number of cells from the beginning of the color table to the last one used (including itself):																										*/
				uint32_t			bV5RedMask			{};	/* Bitmasks for extracting channel values: RED intensity:																																								*/
				uint32_t			bV5GreenMask		{};	/* Bitmasks for extracting channel values: GREEN intensity:																																								*/
				uint32_t			bV5BlueMask			{};	/* Bitmasks for extracting channel values: BLUE intensity:																																								*/
				uint32_t			bV5AlphaMask		{};	/* Bitmasks for extracting channel values: ALPHA channels:																																								*/
				uint32_t			bV5CSType			{};	/* A view of the color space:																																																		*/
				CIEXYZTRIPLE	bV5Endpoints		{};	/* The value of the field is taken into account only if the Tour field contains 0 (LCS_CALIBRATED_RGB). Then the endpoints are indicated in this field:									*/
				uint32_t			bV5GammaRed			{};	/* The value of the field is taken into account only if the Type field contains 0 (LCS_CALIBRATED_RGB). Then the gamma value for the RED component is indicated in the field:		*/
				uint32_t			bV5GammaGreen		{};	/* The value of the field is taken into account only if the Type field contains 0 (LCS_CALIBRATED_RGB). Then the gamma value for the GREEN component is indicated in the field:	*/
				uint32_t			bV5GammaBlue		{};	/* The value of the field is taken into account only if the Type field contains 0 (LCS_CALIBRATED_RGB). Then the gamma value for the BLUE component is indicated in the field:	*/
				uint32_t			bV5Intent			{};	/* Preferences for rendering the raster:																																														*/
				uint32_t			bV5ProfileData		{};	/* The byte offset of the color profile from the beginning of the BITMAPINFO:																																		*/
				uint32_t			bV5ProfileSize		{};	/* If the color profile is directly included in the BMP, then its size in bytes is indicated here:																											*/
				uint32_t			bV5Reserved			{};	/* Reserved and must be reset to zero:																																															*/
			};
		//****************************************************************************************************
		public:
			Bitmap_CORE_Header	bmp_dib_CORE_header;
			Bitmap_INFO_Header	bmp_dib_INFO_header;
			Bitmap_V4_Header		bmp_dib_V4_header;
			Bitmap_V5_Header		bmp_dib_V5_header;
		};
		//****************************************************************************************************
	public:
		Bitmap_File_Header	bmp_file_header;
		Bitmap_DIB_Header		bmp_dib_header;
		//****************************************************************************************************
	public:
		/* Alignment: The compiler rounds the size of the structure to 16 bytes: */
		static const unsigned int getSizeofFileHeader() { return sizeof(BMPFormat::BitmapStruct::Bitmap_File_Header) - 2; }

		static const std::vector <unsigned int> getSizeofTypesFileHeader() { return {2, 4, 2, 2, 4}; }
		//****************************************************************************************************
	};
};



#endif /* BMPFORMAT_HPP! */
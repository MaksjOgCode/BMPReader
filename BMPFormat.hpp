#ifndef BMPFORMAT_HPP
#define BMPFORMAT_HPP



#include "Hints.hpp"
#include <vector>



namespace BMPFormat
{
//------------------------------------------------------------------------------------------------------------------------------------------------------
#pragma pack(push, 1)
	struct CIEXYZ
	{
	public:
		float ciexyzX;
		float ciexyzY;
		float ciexyzZ;
	};
#pragma pack(pop)	

#pragma pack(push, 1)
	struct CIEXYZTRIPLE : private CIEXYZ
	{
	public:
		CIEXYZ ciexyzRed;
		CIEXYZ ciexyzGreen;
		CIEXYZ ciexyzBlue;
	};
#pragma pack(pop)	
//------------------------------------------------------------------------------------------------------------------------------------------------------
#pragma pack(push, 1)
	class Bitmap_File_Header
	{
	public:
		const uint16_t bf_type	= H_IS_BMP_FORMAT;		/* Signature:										*/
				uint32_t bf_size						{};	/* File Size (in bytes):						*/
				uint32_t bf_off_bits					{};	/* Bytes begining PixelArray:					*/
	private:			   
		uint16_t BF_RESERVED1	=	0x0000;				/* Reserved - must contained NULL:			*/
		uint16_t BF_RESERVED2	=	0x0000;				/* Reserved - must contained NULL:			*/
	public:
		static inline const unsigned int getSizeofFILEHeader();
		static inline const std::vector <unsigned int> getSizeofTypesFILEHeader();
	};
#pragma pack(pop)	
	inline const unsigned int BMPFormat::Bitmap_File_Header::getSizeofFILEHeader() { return sizeof(BMPFormat::Bitmap_File_Header); }
	/* We return the byte order of this structure: */
	inline const std::vector <unsigned int> BMPFormat::Bitmap_File_Header::getSizeofTypesFILEHeader() { return {2, 4, 2, 2, 4}; }
//------------------------------------------------------------------------------------------------------------------------------------------------------
#pragma pack(push, 1)
	struct Bitmap_CORE_Header
	{
	public:
		uint32_t bcSize		{};	/* The size of this structure in bytes:																									*/
		uint16_t	bcWidth		{};	/* The width (bcWidth) of the raster in pixels. Specified as an unsigned integer. The value 0 is not documented:	*/
		uint16_t	bcHeight		{};	/* The height (bcHeight) of the raster in pixels. Specified as an unsigned integer. The value 0 is not documented:*/
		uint16_t	bcPlanes		{};	/* In BMP, only the value 1 is allowed. This field is used in Windows icons and cursors:									*/
		uint16_t	bcBitCount	{};	/* Number of bits per pixel:																													*/
	public:
		static inline const unsigned int getSizeofCOREHeader();
		static inline const std::vector <unsigned int> getSizeofTypesCOREHeader();
	};
#pragma pack(pop)
	inline const unsigned int BMPFormat::Bitmap_CORE_Header::getSizeofCOREHeader() { return sizeof(BMPFormat::Bitmap_CORE_Header); }
	/* We return the byte order of this structure: */
	inline const std::vector <unsigned int> BMPFormat::Bitmap_CORE_Header::getSizeofTypesCOREHeader() { return {4, 2, 2, 2, 2}; }
//------------------------------------------------------------------------------------------------------------------------------------------------------
#pragma pack(push, 1)
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
		int32_t	biXPelsPerMeter	{};	/* The number of pixels per meter horizontally:																										*/
		int32_t	biYPelsPerMeter	{};	/* The number of pixels per meter vertically:																										*/
		uint32_t	biClrUsed			{};	/* The size of the color table in cells:																												*/
		uint32_t	biClrImportant		{};	/* The number of cells from the beginning of the color table to the last one used (including itself):								*/
	public:
		static inline const unsigned int getSizeofINFOHeader();
		static inline const std::vector <unsigned int> getSizeofTypesINFOHeader();
	};
#pragma pack(pop)
	inline const unsigned int BMPFormat::Bitmap_INFO_Header::getSizeofINFOHeader() { return sizeof(BMPFormat::Bitmap_INFO_Header); }
	/* We return the byte order of this structure: */
	inline const std::vector <unsigned int> BMPFormat::Bitmap_INFO_Header::getSizeofTypesINFOHeader() { return { 4, 4, 4, 2, 2, 4, 4, 4, 4, 4, 4}; }
//------------------------------------------------------------------------------------------------------------------------------------------------------
#pragma pack(push, 1)
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
		int32_t			bV4XPelsPerMeter	{};	/* The number of pixels per meter horizontally:																																												*/
		int32_t			bV4YPelsPerMeter	{};	/* The number of pixels per meter vertically:																																												*/
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
	public:
		static inline const unsigned int getSizeofV4Header();
		static inline const std::vector <unsigned int> getSizeofTypesV4Header();
	};
#pragma pack(pop)
	inline const unsigned int BMPFormat::Bitmap_V4_Header::getSizeofV4Header() { return sizeof(BMPFormat::Bitmap_V4_Header); }
	/* We return the byte order of this structure: */
	inline const std::vector <unsigned int> BMPFormat::Bitmap_V4_Header::getSizeofTypesV4Header() { return { 4, 4, 4, 2, 2, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 36, 4, 4, 4 }; }
//------------------------------------------------------------------------------------------------------------------------------------------------------
#pragma pack(push, 1)
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
		int32_t			bV5XPelsPerMeter	{};	/* The number of pixels per meter horizontally:																																							*/
		int32_t			bV5YPelsPerMeter	{};	/* The number of pixels per meter vertically:																																							*/
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
	public:
		static inline const unsigned int getSizeofV5Header();
		static inline const std::vector <unsigned int> getSizeofTypesV5Header();
	};
#pragma pack(pop)
	inline const unsigned int BMPFormat::Bitmap_V5_Header::getSizeofV5Header() { return sizeof(BMPFormat::Bitmap_V5_Header); }
	/* We return the byte order of this structure: */
	inline const std::vector <unsigned int> BMPFormat::Bitmap_V5_Header::getSizeofTypesV5Header() { return {4, 4, 4, 2, 2, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 36, 4, 4, 4, 4, 4, 4, 4 }; }
//------------------------------------------------------------------------------------------------------------------------------------------------------
	struct Bitmap_DIB_Header
	{
	public:
		Bitmap_CORE_Header	bmp_core_header;
		Bitmap_INFO_Header	bmp_info_header;
		Bitmap_V4_Header		bmp_v4_header;
		Bitmap_V5_Header		bmp_v5_header;
	};
//------------------------------------------------------------------------------------------------------------------------------------------------------
	struct Bitmap
	{
	public:
		Bitmap_File_Header	bmp_file_header;
		Bitmap_DIB_Header		bmp_dib_header;

	public:
		inline void setVersion(uint8_t _flag_version_dib);
		inline uint8_t getVersion() const;

	private:
		uint8_t  flag_version_dib = 0b0000; /* 0001 - Bitmap_CORE_Header || 0010 - Bitmap_INFO_Header || 0100 - Bitmap_V4_Header || 1000 - Bitmap_V5_Header: */
	};

	inline void BMPFormat::Bitmap::setVersion(uint8_t _flag_version_dib) {
		if (_flag_version_dib == 0b0001 || _flag_version_dib == 0b0010 || _flag_version_dib == 0b0100 || _flag_version_dib == 0b1000)
			this->flag_version_dib = _flag_version_dib;

		return;
	}

	inline uint8_t BMPFormat::Bitmap::getVersion() const { return this->flag_version_dib; }
//------------------------------------------------------------------------------------------------------------------------------------------------------
};



#endif /* BMPFORMAT_HPP! */
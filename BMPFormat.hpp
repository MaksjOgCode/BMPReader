#ifndef BMPFORMAT_HPP
#define BMPFORMAT_HPP



#include "Hints.hpp"
#include <vector>
#include <cstdint>



/*
            More Information:
            Link: https://en.wikipedia.org/wiki/BMP_file_format
*/



namespace BMPFormat
{
//------------------------------------------------------------------------------------------------------------------------------------------------------
#pragma pack(push, 1)
	struct CIEXYZ
	{
	public:
		float ciexyz_x;
		float ciexyz_y;
		float ciexyz_z;
	};
#pragma pack(pop)	

#pragma pack(push, 1)
	struct CIEXYZTRIPLE
	{
	public:
		CIEXYZ ciexyz_red;
		CIEXYZ ciexyz_green;
		CIEXYZ ciexyz_blue;
	};
#pragma pack(pop)	

#pragma pack(push, 1)
	struct RGBQUAD
	{
		uint8_t rgb_blue     {}; 
		uint8_t rgb_green    {}; 
		uint8_t rgb_red      {};  
		uint8_t rgb_reserved {}; /* Reserved for alpha_channel: */
	};
#pragma pack(pop)

#pragma pack(push, 1)
	struct RGBTRIPLE
	{
		uint8_t rgb_t_blue     {}; 
		uint8_t rgb_t_green    {};
		uint8_t rgb_t_red      {};  
	};
#pragma pack(pop)
//------------------------------------------------------------------------------------------------------------------------------------------------------
#pragma pack(push, 1)
	struct Bitmap_FILE_Header
	{ /* "bf" = bitmap file: */
	public:
		const uint16_t bf_type	= H_IS_BMP_FORMAT; /* Signature:                                    */
	public:		
		      uint32_t bf_size		            {}; /* File Size (in bytes):                         */
	public:
				uint32_t bf_off_bits	            {}; /* The position of the pixels in this structure: */
	private:			   
		uint16_t BF_RESERVED_1	=	        0x0000; /* Reserved - must contained NULL:               */
		uint16_t BF_RESERVED_2	=	        0x0000; /* Reserved - must contained NULL:               */
	public:
		static inline const uint64_t getSizeof_FILE_Header ();
		static inline const std::vector <uint8_t> getSizeof_Types_FILE_Header ();
	};
#pragma pack(pop)	
	inline const uint64_t BMPFormat::Bitmap_FILE_Header::getSizeof_FILE_Header() { return sizeof(BMPFormat::Bitmap_FILE_Header); }
	inline const std::vector <uint8_t> BMPFormat::Bitmap_FILE_Header::getSizeof_Types_FILE_Header() { return {2, 4, 2, 2, 4}; }
//------------------------------------------------------------------------------------------------------------------------------------------------------
#pragma pack(push, 1)
	struct Bitmap_DIB_CORE_Header
	{ /* "bc" = bitmap core: */
	public:
				uint32_t bc_size                {}; /* The size of this structure in bytes:                                                   */
	public:								           
				uint16_t	bc_width               {}; /* The width of the picture in pixels. It is an number. Zero is not allowed:              */
				uint16_t	bc_height              {}; /* The height of the picture in pixels. A number without signs. The number 0 is not used: */
	public:
		const uint16_t	bc_planes = H_COLOR_PLANE; /* In BMP, only 1 is allowed. This is used in icons and pictures:                         */
	public:
				uint16_t	bc_bit_count           {}; /* Number of bits per pixel:    min 1 - max 64    0 - pixels into png or jpeg             */
	public:
		static inline const uint64_t getSizeof_DIB_CORE_Header();
		static inline const std::vector <uint8_t> getSizeof_Types_DIB_CORE_Header();
	};
#pragma pack(pop)
	inline const uint64_t BMPFormat::Bitmap_DIB_CORE_Header::getSizeof_DIB_CORE_Header() { return sizeof(BMPFormat::Bitmap_DIB_CORE_Header); }
	inline const std::vector <uint8_t> BMPFormat::Bitmap_DIB_CORE_Header::getSizeof_Types_DIB_CORE_Header() { return {4, 2, 2, 2, 2}; }
//------------------------------------------------------------------------------------------------------------------------------------------------------
#pragma pack(push, 1)
	struct Bitmap_DIB_INFO_Header
	{ /* "bi" = bitmap info: */
	public:
		      uint32_t bi_size                             {}; /* The size of this structure in bytes:                                                                                                                            */
	public:
				int32_t	bi_width                            {}; /* The width of the picture in pixels. It is an number. Zero is not allowed:                                                                                       */
		      int32_t	bi_height                           {}; /* Two parts: [1] is a number that shows how many pixels are in height. [2] is a sign that tells you how the rows in the table are arranged. Zero is not allowed: */
	public:
	   const uint16_t	bi_planes        =       H_COLOR_PLANE; /* In BMP, only 1 is allowed. This is used in icons and pictures:                                                                                                 */
	public:
				uint16_t bi_bit_count                        {}; /* Number of bits per pixel:    min 1 - max 64    0 - pixels into png or jpeg                                                                                     */
	public:
				uint32_t	bi_compression   =            H_BI_RGB; /* Indicates the way pixels are stored:                                                                                                                           */
	public:
				uint32_t	bi_size_image                       {}; /* The size of the pixel data in bytes. It can be reset to zero if the storage is carried out by a two-dimensional array:                                         */
	public:
				int32_t	bi_x_pels_per_meter                 {}; /* The number of pixels per meter horizontally:                                                                                                                   */
		      int32_t	bi_y_pels_per_meter                 {}; /* The number of pixels per meter vertically:                                                                                                                     */
	public:
				uint32_t	bi_ñlr_used      =          H_USED_CLR; /* The size of the color table in cells:                                                                                                                          */
				uint32_t	bi_ñlr_important = H_ALL_IMPORTANT_CLR; /* The number of important colors from the beginning of the color table to the last used one:                                                                     */
	public:
		static inline const uint64_t getSizeof_DIB_INFO_Header();
		static inline const std::vector <uint8_t> getSizeof_Types_DIB_INFO_Header();
	};
#pragma pack(pop)
	inline const uint64_t BMPFormat::Bitmap_DIB_INFO_Header::getSizeof_DIB_INFO_Header() { return sizeof(BMPFormat::Bitmap_DIB_INFO_Header); }
	inline const std::vector <uint8_t> BMPFormat::Bitmap_DIB_INFO_Header::getSizeof_Types_DIB_INFO_Header() { return { 4, 4, 4, 2, 2, 4, 4, 4, 4, 4, 4}; }
//------------------------------------------------------------------------------------------------------------------------------------------------------
#pragma pack(push, 1)
	struct Bitmap_DIB_V4_Header
	{
	public:
		      uint32_t     bv4_size                              {}; /* The size of this structure in bytes:                                                                                                                                         */ 
	public:
				int32_t      bv4_width                             {}; /* The width of the picture in pixels. It is an number. Zero is not allowed:                                                                                                    */ 
		      int32_t	    bv4_height                            {}; /* Two parts: [1] is a number that shows how many pixels are in height. [2] is a sign that tells you how the rows in the table are arranged. Zero is not allowed:               */ 
	public:
		const uint16_t     bv4_planes         =       H_COLOR_PLANE; /* In BMP, only 1 is allowed. This is used in icons and pictures:                                                                                                               */ 
	public:
		      uint16_t     bv4_bit_count                         {}; /* Number of bits per pixel:    min 1 - max 64    0 - pixels into png or jpeg                                                                                                   */ 
	public:
				uint32_t     bv4_v4_compression =            H_BI_RGB; /* Indicates the way pixels are stored:                                                                                                                                         */ 
	public:
				uint32_t     bv4_size_image                        {}; /* The size of the pixel data in bytes. It can be reset to zero if the storage is carried out by a two-dimensional array:                                                       */ 
	public:
				int32_t      bv4_x_pels_per_meter                  {}; /* The number of pixels per meter horizontally:                                                                                                                                 */ 
		      int32_t      bv4_y_pels_per_meter                  {}; /* The number of pixels per meter vertically:                                                                                                                                   */ 
	public:
				uint32_t     bv4_clr_used       =          H_USED_CLR; /* The size of the color table in cells:                                                                                                                                        */ 
		      uint32_t     bv4_clr_important  = H_ALL_IMPORTANT_CLR; /* The number of important colors from the beginning of the color table to the last used one:                                                                                   */ 
	public:
				uint32_t     bv4_red_mask                          {}; /* Bitmasks for extracting channel values: RED intensity:                                                                                                                       */
		      uint32_t     bv4_green_mask                        {}; /* Bitmasks for extracting channel values: GREEN intensity:                                                                                                                     */
		      uint32_t     bv4_blue_mask                         {}; /* Bitmasks for extracting channel values: BLUE intensity:                                                                                                                      */
		      uint32_t     bv4_alpha_mask                        {}; /* Bitmasks for extracting channel values: ALPHA channels:                                                                                                                      */
	public:
				uint32_t     bv4_cs_type                           {}; /* A view of the color space:                                                                                                                                                   */
	public:
				CIEXYZTRIPLE bv4_end_points                        {}; /* The value of the field is taken into account only if the Tour field contains 0 (LCS_CALIBRATED_RGB). Then the endpoints are indicated in this field:                         */
	public:
				uint32_t     bv4_gamma_red                         {}; /* The value of the field is taken into account only if the Type field contains 0 (LCS_CALIBRATED_RGB). Then the gamma value for the RED component is indicated in the field:   */
		      uint32_t     bv4_gamma_green                       {}; /* The value of the field is taken into account only if the Type field contains 0 (LCS_CALIBRATED_RGB). Then the gamma value for the GREEN component is indicated in the field: */
		      uint32_t     bv4_gamma_blue                        {}; /* The value of the field is taken into account only if the Type field contains 0 (LCS_CALIBRATED_RGB). Then the gamma value for the BLUE component is indicated in the field:  */
	public:
		static inline const uint64_t getSizeof_DIB_V4_Header();
		static inline const std::vector <uint8_t> getSizeof_Types_DIB_V4_Header();
	};
#pragma pack(pop)
	inline const uint64_t BMPFormat::Bitmap_DIB_V4_Header::getSizeof_DIB_V4_Header() { return sizeof(BMPFormat::Bitmap_DIB_V4_Header); }
	inline const std::vector <uint8_t> BMPFormat::Bitmap_DIB_V4_Header::getSizeof_Types_DIB_V4_Header() { return { 4, 4, 4, 2, 2, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 36, 4, 4, 4 }; }
//------------------------------------------------------------------------------------------------------------------------------------------------------
#pragma pack(push, 1)
	struct Bitmap_DIB_V5_Header
	{
	public:
		      uint32_t     bv5_size                                {};	/* The size of this structure in bytes, which also indicates the version of the structure:																													*/
	public:				 														  
		      int32_t      bv5_width                               {};	/* The width of the raster in pixels. Specified as a signed integer. Zero and negative are not documented:																								*/
		      int32_t      bv5_height                              {};	/* Two parameters: the height of the raster in pixels and the order of the rows in two-dimensional arrays:																								*/
	public:				
		const uint16_t     bv5_planes           =       H_COLOR_PLANE;	/* In BMP, only the value 1 is allowed. This field is used in Windows icons and cursors:																														*/
	public:				 
		      uint16_t     bv5_bit_count                           {};	/* Number of bits per pixel:																																																		*/
	public:		    		 
		      uint32_t	    bv5_compression      =            H_BI_RGB;	/* Indicates the way pixels are stored:																																														*/
	public:		     
		      uint32_t	    bv5_size_image                          {};	/* The size of the pixel data in bytes. It can be reset to zero if the storage is carried out by a two-dimensional array:																			*/
	public:		     															  
		      int32_t	    bv5_x_pels_per_meter                    {};	/* The number of pixels per meter horizontally:																																							*/
		      int32_t	    bv5_y_pels_per_meter                    {};	/* The number of pixels per meter vertically:																																							*/
	public:		     
		      uint32_t	    bv5_clr_used			 =          H_USED_CLR;	/* The size of the color table in cells:																																														*/
				uint32_t	    bv5_clr_important	 = H_ALL_IMPORTANT_CLR;	/* The number of cells from the beginning of the color table to the last one used (including itself):																										*/
	public:		     
		      uint32_t	    bv5_red_mask                            {};	/* Bitmasks for extracting channel values: RED intensity:																																								*/
		      uint32_t	    bv5_green_mask                          {};	/* Bitmasks for extracting channel values: GREEN intensity:																																								*/
		      uint32_t	    bv5_blue_mask                           {};	/* Bitmasks for extracting channel values: BLUE intensity:																																								*/
		      uint32_t	    bv5_alpha_mask                          {};	/* Bitmasks for extracting channel values: ALPHA channels:																																								*/
	public:		     															  
		      uint32_t     bv5_cs_type                             {};	/* A view of the color space:																																																		*/
	public:				 														  
		      CIEXYZTRIPLE bv5_end_points                          {};	/* The value of the field is taken into account only if the Tour field contains 0 (LCS_CALIBRATED_RGB). Then the endpoints are indicated in this field:									*/
	public:				 														  
		      uint32_t     bv5_gamma_red                           {};	/* The value of the field is taken into account only if the Type field contains 0 (LCS_CALIBRATED_RGB). Then the gamma value for the RED component is indicated in the field:		*/
		      uint32_t     bv5_gamma_green                         {};	/* The value of the field is taken into account only if the Type field contains 0 (LCS_CALIBRATED_RGB). Then the gamma value for the GREEN component is indicated in the field:	*/
		      uint32_t     bv5_gamma_blue                          {};	/* The value of the field is taken into account only if the Type field contains 0 (LCS_CALIBRATED_RGB). Then the gamma value for the BLUE component is indicated in the field:	*/
	public:			       													  
		      uint32_t     bv5_intent                              {};	/* Preferences for rendering the raster:																																														*/
	public:			       													  
		      uint32_t     bv5_profile_data                        {};	/* The byte offset of the color profile from the beginning of the BITMAPINFO:																																		*/
		      uint32_t     bv5_profile_size                        {};	/* If the color profile is directly included in the BMP, then its size in bytes is indicated here:																											*/
	public:			       													  
		      uint32_t     bv5_reserved                            {};	/* Reserved and must be reset to zero:																																															*/
	public:
		static inline const uint64_t getSizeof_DIB_V5_Header();
		static inline const std::vector <uint8_t> getSizeof_Types_DIB_V5_Header();
	};
#pragma pack(pop)
	inline const uint64_t BMPFormat::Bitmap_DIB_V5_Header::getSizeof_DIB_V5_Header() {return sizeof(BMPFormat::Bitmap_DIB_V5_Header); }
	inline const std::vector <uint8_t> BMPFormat::Bitmap_DIB_V5_Header::getSizeof_Types_DIB_V5_Header() { return {4, 4, 4, 2, 2, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 36, 4, 4, 4, 4, 4, 4, 4 }; }
//------------------------------------------------------------------------------------------------------------------------------------------------------
	struct Bitmap_DIB_Header
	{
	public:
		Bitmap_DIB_CORE_Header	bmp_core_header;
		Bitmap_DIB_INFO_Header	bmp_dib_info_header;
		Bitmap_DIB_V4_Header		bmp_dib_v4_header;
		Bitmap_DIB_V5_Header		bmp_dib_v5_header;

		std::vector <RGBTRIPLE> bmp_dib_clr_palette_24b;
		std::vector <RGBQUAD>	bmp_dib_clr_palette_32b;
	};
//------------------------------------------------------------------------------------------------------------------------------------------------------
	struct Bitmap
	{
	public:
		Bitmap_FILE_Header	bmp_file_header;
		Bitmap_DIB_Header		bmp_dib_header;
	public:
		std::vector < std::vector <uint8_t> > bmp_data_pixel {};
	public:
		inline void setVersion(uint8_t _flag_version_dib);
		inline uint8_t getVersion() const;
	private:
		uint8_t  flag_version_dib = 0b0000; /* 0001 - Bitmap_DIB_CORE_Header || 0010 - Bitmap_DIB_INFO_Header || 0100 - Bitmap_DIB_V4_Header || 1000 - Bitmap_DIB_V5_Header: */
	};
	inline void BMPFormat::Bitmap::setVersion(uint8_t _flag_version_dib)
	{
		if ( _flag_version_dib == H_VERSION_DIB_CORE_HEADER	||
			  _flag_version_dib == H_VERSION_DIB_INFO_HEADER	||
			  _flag_version_dib == H_VERSION_DIB_V4_HEADER		||
			  _flag_version_dib == H_VERSION_DIB_V5_HEADER         )
		{
			this->flag_version_dib = _flag_version_dib;
		}

		return;
	}
	inline uint8_t BMPFormat::Bitmap::getVersion() const { return this->flag_version_dib; }
//------------------------------------------------------------------------------------------------------------------------------------------------------
};



#endif /* BMPFORMAT_HPP! */
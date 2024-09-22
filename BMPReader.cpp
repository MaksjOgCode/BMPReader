#include "BMPReader.hpp"



//--------------------------------------------------------------------------------------------------------------
const bool BMPReader::BitmapReader::openBMP()
{
   bmp_file.open(path_to_file, std::ios::binary);
   return bmp_file.is_open();
}
//--------------------------------------------------------------------------------------------------------------
const bool BMPReader::BitmapReader::closeBMP()
{
   if ( bmp_file.is_open() )
   {
      bmp_file.close();
      return true;
   }

   return false;
}
//--------------------------------------------------------------------------------------------------------------
std::vector <BMPReader::H_BRAA::Bytes> BMPReader::BitmapReader::readBMP_FILE_Header()
{
   BMPReader::H_BRAA::HelperRefactor_xd helper_refactor_xd( BMPFormat::Bitmap_File_Header::getSizeofFILEHeader() );

   if ( !bmp_file )  /* The File .bmp must be opened: */
      return helper_refactor_xd.set_of_bytes_header;

   /* Reading 14 bytes from .bmp file and put into bytes_buffer: */
   bmp_file.read( reinterpret_cast<char*>( helper_refactor_xd.bytes_buffer.data() ), helper_refactor_xd.sizeof_CURRENT_header );

   /* Move the cursor to the number of bytes read (14): */
   bmp_file.seekg( helper_refactor_xd.sizeof_CURRENT_header, std::ios::beg );

   /* We convert the read data into a convenient format for further work: */
   BMPReader::H_BRAA::ConvertToBitmapData(helper_refactor_xd);

   return helper_refactor_xd.set_of_bytes_header;
}
//--------------------------------------------------------------------------------------------------------------
std::vector <BMPReader::H_BRAA::Bytes>  BMPReader::BitmapReader::readBMP_DIB_Header()
{
   std::vector <unsigned char> bitmapinfo_version_flag (4);
   bmp_file.read ( reinterpret_cast<char*>( bitmapinfo_version_flag.data() ), 4 );

   unsigned long long char_flag_version {};
   char_flag_version = BMPReader::H_BRAA::getResultValueFromFile(bitmapinfo_version_flag);

   unsigned int _sizeof_CURRENT_header {};
   switch(char_flag_version)
   {
   case H_BITMAPCOREHEADER:
      _sizeof_CURRENT_header = BMPFormat::Bitmap_CORE_Header::getSizeofCOREHeader();
      break;

   case H_BITMAPINFOHEADER:
      _sizeof_CURRENT_header = BMPFormat::Bitmap_INFO_Header::getSizeofINFOHeader();
      break;

   case H_BITMAPV4HEADER:
      _sizeof_CURRENT_header = BMPFormat::Bitmap_V4_Header::getSizeofV4Header();
      break;

   case H_BITMAPV5HEADER:
      _sizeof_CURRENT_header = BMPFormat::Bitmap_V5_Header::getSizeofV5Header();
      break;

   default:
      break;
   }

   BMPReader::H_BRAA::HelperRefactor_xd helper_refactor_xd( _sizeof_CURRENT_header );

   bmp_file.seekg( BMPFormat::Bitmap_File_Header::getSizeofFILEHeader(), std::ios::beg );

   /* Reading ... bytes from .bmp file and put into bytes_buffer: */
   bmp_file.read( reinterpret_cast<char*>( helper_refactor_xd.bytes_buffer.data() ), helper_refactor_xd.sizeof_CURRENT_header );

   /* NEED TO PIXEL DATA! */
   //bmp_file.seekg( helper_refactor_xd.sizeof_CURRENT_header, std::ios::beg );

   BMPReader::H_BRAA::ConvertToBitmapData(helper_refactor_xd);

   return helper_refactor_xd.set_of_bytes_header;
}
//--------------------------------------------------------------------------------------------------------------
std::vector <BMPReader::H_BRAA::Bytes> BMPReader::BitmapReader::readBMPFile()
{
   std::vector <BMPReader::H_BRAA::Bytes>  temp_data_from_bmp_file  {};

   data_from_bmp_file = BMPReader::BitmapReader::readBMP_FILE_Header();

   if ( data_from_bmp_file.empty() )
      return temp_data_from_bmp_file;

   temp_data_from_bmp_file = BMPReader::BitmapReader::readBMP_DIB_Header();
   if ( !temp_data_from_bmp_file.empty() )
   {
      if ( 
         BMPReader::H_BRAA::getResultValueFromFile(temp_data_from_bmp_file[0]) != H_BITMAPCOREHEADER   &&
         BMPReader::H_BRAA::getResultValueFromFile(temp_data_from_bmp_file[0]) != H_BITMAPINFOHEADER   &&
         BMPReader::H_BRAA::getResultValueFromFile(temp_data_from_bmp_file[0]) != H_BITMAPV4HEADER     &&
         BMPReader::H_BRAA::getResultValueFromFile(temp_data_from_bmp_file[0]) != H_BITMAPV5HEADER
         )
      {
         temp_data_from_bmp_file.clear();
         return temp_data_from_bmp_file;
      }
   }
   data_from_bmp_file.insert( data_from_bmp_file.end(), temp_data_from_bmp_file.begin(), temp_data_from_bmp_file.end() );

   return data_from_bmp_file;
}
//--------------------------------------------------------------------------------------------------------------
std::vector <BMPReader::H_BRAA::Bytes>  BMPReader::BitmapReader::readBMP_PIXEL_DATA(BMPFormat::Bitmap &struct_object)
{
   return data_from_bmp_file;
}
//--------------------------------------------------------------------------------------------------------------
BMPFormat::Bitmap& BMPReader::BitmapReader::readAndFillBitmapStruct(BMPFormat::Bitmap &struct_object)
{
   data_from_bmp_file = readBMPFile();

   BMPReader::BitmapReader::fillBitmapStruct(struct_object, data_from_bmp_file);

   return struct_object;
}
//--------------------------------------------------------------------------------------------------------------
BMPFormat::Bitmap& BMPReader::BitmapReader::fill_FILE_header(BMPFormat::Bitmap &struct_object, const std::vector <BMPReader::H_BRAA::Bytes> &data_from_bmp_file)
{
   /* BMP FILE HEADER: */

   /*
   data_from_bmp_file:
   [0] { 0x01, 0x02 },              = A mark to distinguish the format from others. Must contain 4D42[16]/424D[16]:
   [1] { 0x01, 0x02, 0x03, 0x04 },  = The file size in bytes:
   [2] { 0x01, 0x02 },              = Reserved and must contain zero:
   [3] { 0x01, 0x02 },              = Reserved and must contain zero:
   [4] { 0x01, 0x02, 0x03, 0x04 }   = The position of the pixel data relative to the beginning of this structure:
   */

   /* A mark to distinguish the format from others (format signature): */
   if (BMPReader::H_BRAA::getResultValueFromFile(data_from_bmp_file[0]) != H_IS_BMP_FORMAT)
      return struct_object;

   //struct_object.bmp_file_header.bf_type   = static_cast <uint16_t>( BMPReader::BRAA::getResultValueFromFile(data_from_bmp_file[0]) ); CONST INTO STRUCT:
   struct_object.bmp_file_header.bf_size     = static_cast <uint32_t>( BMPReader::H_BRAA::getResultValueFromFile(data_from_bmp_file[1]) );
   /* data_from_bmp_file[2]: Reserved and must contain zero: */
   /* data_from_bmp_file[3]: Reserved and must contain zero: */
   struct_object.bmp_file_header.bf_off_bits = static_cast <uint32_t>( BMPReader::H_BRAA::getResultValueFromFile(data_from_bmp_file[4]) );

   return struct_object;
}
//--------------------------------------------------------------------------------------------------------------
BMPFormat::Bitmap& BMPReader::BitmapReader::fill_DIB_header(BMPFormat::Bitmap &struct_object, const std::vector <BMPReader::H_BRAA::Bytes> &data_from_bmp_file)
{
   /* An additional check is whether the file is readable.bmp file: */
   unsigned long long bmp_dib_type = BMPReader::H_BRAA::getResultValueFromFile(data_from_bmp_file[5]);
   if ( !data_from_bmp_file.empty() )
   {
      if ( bmp_dib_type != H_BITMAPCOREHEADER && bmp_dib_type != H_BITMAPINFOHEADER && bmp_dib_type != H_BITMAPV4HEADER && bmp_dib_type != H_BITMAPV5HEADER )
         return struct_object;
   }

   switch (bmp_dib_type)
   {
   case H_BITMAPCOREHEADER:
      fill_DIB_CORE_header(struct_object, data_from_bmp_file);
      break;

   case H_BITMAPINFOHEADER:
      fill_DIB_INFO_header(struct_object, data_from_bmp_file);
      break;

   case H_BITMAPV4HEADER:
      fill_DIB_V4_header(struct_object, data_from_bmp_file);
      break;

   case H_BITMAPV5HEADER:
      fill_DIB_V5_header(struct_object, data_from_bmp_file);
      break;

   default:
      return struct_object;
      break;
   }

   return struct_object;
}
//--------------------------------------------------------------------------------------------------------------
BMPFormat::Bitmap& BMPReader::BitmapReader::fill_DIB_CORE_header (BMPFormat::Bitmap &struct_object, const std::vector <BMPReader::H_BRAA::Bytes> &data_from_bmp_file)
{
   /* BMP DIB CORE HEADER: */

   /*
   data_from_bmp_file:
   [5]   { 0x01, 0x02, 0x03, 0x04 },   = The size of this structure in bytes:																									
   [6]   { 0x01, 0x02 },               = The width (bcWidth) of the raster in pixels. Specified as an unsigned integer. The value 0 is not documented:	
   [7]   { 0x01, 0x02 },               = The height (bcHeight) of the raster in pixels. Specified as an unsigned integer. The value 0 is not documented:
   [8]   { 0x01, 0x02 },               = In BMP, only the value 1 is allowed. This field is used in Windows icons and cursors:									
   [9]   { 0x01, 0x02 }                = Number of bits per pixel:																													
   */

   struct_object.setVersion(H_VERSION_DIB_CORE_HEADER);

   struct_object.bmp_dib_header.bmp_core_header.bcSize      =  static_cast <uint32_t> ( BMPReader::H_BRAA::getResultValueFromFile(data_from_bmp_file[5]) );
   struct_object.bmp_dib_header.bmp_core_header.bcWidth     =  static_cast <uint16_t> ( BMPReader::H_BRAA::getResultValueFromFile(data_from_bmp_file[6]) );
   struct_object.bmp_dib_header.bmp_core_header.bcHeight    =  static_cast <uint16_t> ( BMPReader::H_BRAA::getResultValueFromFile(data_from_bmp_file[7]) );
   struct_object.bmp_dib_header.bmp_core_header.bcPlanes    =  static_cast <uint16_t> ( BMPReader::H_BRAA::getResultValueFromFile(data_from_bmp_file[8]) );
   struct_object.bmp_dib_header.bmp_core_header.bcBitCount  =  static_cast <uint16_t> ( BMPReader::H_BRAA::getResultValueFromFile(data_from_bmp_file[9]) );

   return struct_object;
}
//--------------------------------------------------------------------------------------------------------------
BMPFormat::Bitmap& BMPReader::BitmapReader::fill_DIB_INFO_header (BMPFormat::Bitmap &struct_object, const std::vector <BMPReader::H_BRAA::Bytes> &data_from_bmp_file)
{
   /* BMP DIB INFO HEADER: */

   /*
   data_from_bmp_file:
   [5]   { 0x01, 0x02, 0x03, 0x04 },   =  The size of this structure in bytes, which also indicates the version of the structure:
   [6]   { 0x01, 0x02, 0x03, 0x04 },   =  The width of the raster in pixels. Specified as a signed integer. Zero and negative are not documented:
   [7]   { 0x01, 0x02, 0x03, 0x04 },   =  Two parameters: the height of the raster in pixels and the order of the rows in two-dimensional arrays:	
   [8]   { 0x01, 0x02 },               =  In BMP, only the value 1 is allowed. This field is used in Windows icons and cursors:			
   [9]   { 0x01, 0x02 },               =  Number of bits per pixel:					
   [10]  { 0x01, 0x02, 0x03, 0x04 },   =  Indicates the way pixels are stored:	
   [11]  { 0x01, 0x02, 0x03, 0x04 },   =  The size of the pixel data in bytes. It can be reset to zero if the storage is carried out by a two-dimensional array:
   [12]  { 0x01, 0x02, 0x03, 0x04 },   =  The number of pixels per meter horizontally:	
   [13]  { 0x01, 0x02, 0x03, 0x04 },   =  The number of pixels per meter vertically:		
   [14]  { 0x01, 0x02, 0x03, 0x04 },   =  The size of the color table in cells:		
   [15]  { 0x01, 0x02, 0x03, 0x04 }    =  The number of cells from the beginning of the color table to the last one used (including itself):	
   */
	
   struct_object.setVersion(H_VERSION_DIB_INFO_HEADER);

   struct_object.bmp_dib_header.bmp_info_header.biSize			   =  static_cast <uint32_t>  ( BMPReader::H_BRAA::getResultValueFromFile(data_from_bmp_file[5])  ); 
   struct_object.bmp_dib_header.bmp_info_header.biWidth			   =  static_cast <int32_t>	( BMPReader::H_BRAA::getResultValueFromFile(data_from_bmp_file[6])  );   
   struct_object.bmp_dib_header.bmp_info_header.biHeight			   =  static_cast <int32_t>   ( BMPReader::H_BRAA::getResultValueFromFile(data_from_bmp_file[7])  ); 
   struct_object.bmp_dib_header.bmp_info_header.biPlanes			   =  static_cast <uint16_t>	( BMPReader::H_BRAA::getResultValueFromFile(data_from_bmp_file[8])  );   
   struct_object.bmp_dib_header.bmp_info_header.biBitCount		   =  static_cast <uint16_t>  ( BMPReader::H_BRAA::getResultValueFromFile(data_from_bmp_file[9])  ); 
   struct_object.bmp_dib_header.bmp_info_header.biCompression	   =  static_cast <uint32_t>	( BMPReader::H_BRAA::getResultValueFromFile(data_from_bmp_file[10]) );   
   struct_object.bmp_dib_header.bmp_info_header.biSizeImage		   =  static_cast <uint32_t>	( BMPReader::H_BRAA::getResultValueFromFile(data_from_bmp_file[11]) );   
   struct_object.bmp_dib_header.bmp_info_header.biXPelsPerMeter   =  static_cast <int32_t>   ( BMPReader::H_BRAA::getResultValueFromFile(data_from_bmp_file[12]) ); 
   struct_object.bmp_dib_header.bmp_info_header.biYPelsPerMeter   =  static_cast <int32_t>   ( BMPReader::H_BRAA::getResultValueFromFile(data_from_bmp_file[13]) ); 
   struct_object.bmp_dib_header.bmp_info_header.biClrUsed		   =  static_cast <uint32_t>	( BMPReader::H_BRAA::getResultValueFromFile(data_from_bmp_file[14]) );   
   struct_object.bmp_dib_header.bmp_info_header.biClrImportant	   =  static_cast <uint32_t>	( BMPReader::H_BRAA::getResultValueFromFile(data_from_bmp_file[15]) );   

   return struct_object;
}
//--------------------------------------------------------------------------------------------------------------
BMPFormat::Bitmap& BMPReader::BitmapReader::fill_DIB_V4_header (BMPFormat::Bitmap &struct_object, const std::vector <BMPReader::H_BRAA::Bytes> &data_from_bmp_file)
{
   /* BMP DIB V4 HEADER: */

   /*
   data_from_bmp_file:
   [5]   { 0x01, 0x02, 0x03, 0x04 },   =  The size of this structure in bytes, which also indicates the version of the structure:																												
   [6]   { 0x01, 0x02, 0x03, 0x04 },   =  The width of the raster in pixels. Specified as a signed integer. Zero and negative are not documented:																							
   [7]   { 0x01, 0x02, 0x03, 0x04 },   =  Two parameters: the height of the raster in pixels and the order of the rows in two-dimensional arrays:																							
   [8]   { 0x01, 0x02 },               =  In BMP, only the value 1 is allowed. This field is used in Windows icons and cursors:																													
   [9]   { 0x01, 0x02 },               =  Number of bits per pixel:																																																	
   [10]  { 0x01, 0x02, 0x03, 0x04 },   =  Indicates the way pixels are stored:																																													
   [11]  { 0x01, 0x02, 0x03, 0x04 },   =  The size of the pixel data in bytes. It can be reset to zero if the storage is carried out by a two-dimensional array:																		
   [12]  { 0x01, 0x02, 0x03, 0x04 },   =  The number of pixels per meter horizontally:																																											
   [13]  { 0x01, 0x02, 0x03, 0x04 },   =  The number of pixels per meter vertically:																																											
   [14]  { 0x01, 0x02, 0x03, 0x04 },   =  The size of the color table in cells:																																													
   [15]  { 0x01, 0x02, 0x03, 0x04 }    =  The number of cells from the beginning of the color table to the last one used (including itself):																									
   [16]  { 0x01, 0x02, 0x03, 0x04 },   =  Bitmasks for extracting channel values: RED intensity:																																							
   [17]  { 0x01, 0x02, 0x03, 0x04 },   =  Bitmasks for extracting channel values: GREEN intensity:																																							
   [18]  { 0x01, 0x02, 0x03, 0x04 },   =  Bitmasks for extracting channel values: BLUE intensity:																																							
   [19]  { 0x01, 0x02, 0x03, 0x04 },   =  Bitmasks for extracting channel values: ALPHA channels:																																							
   [20]  { 0x01, 0x02, 0x03, 0x04 },   =  A view of the color space:																																																	
   [21]  { (0x00) MULTIPLY (36)   },   =  The value of the field is taken into account only if the Tour field contains 0 (LCS_CALIBRATED_RGB). Then the endpoints are indicated in this field:								
   [22]  { 0x01, 0x02, 0x03, 0x04 },   =  The value of the field is taken into account only if the Type field contains 0 (LCS_CALIBRATED_RGB). Then the gamma value for the RED component is indicated in the field:	
   [23]  { 0x01, 0x02, 0x03, 0x04 },   =  The value of the field is taken into account only if the Type field contains 0 (LCS_CALIBRATED_RGB). Then the gamma value for the GREEN component is indicated in the field:
   [24]  { 0x01, 0x02, 0x03, 0x04 },   =  The value of the field is taken into account only if the Type field contains 0 (LCS_CALIBRATED_RGB). Then the gamma value for the BLUE component is indicated in the field:
   */

   struct_object.setVersion(H_VERSION_DIB_V4_HEADER);

   struct_object.bmp_dib_header.bmp_v4_header.bV4Size				=  static_cast <uint32_t>     ( BMPReader::H_BRAA::getResultValueFromFile(data_from_bmp_file[5])  ); 
   struct_object.bmp_dib_header.bmp_v4_header.bV4Width			=  static_cast <int32_t>	   ( BMPReader::H_BRAA::getResultValueFromFile(data_from_bmp_file[6])  );	
   struct_object.bmp_dib_header.bmp_v4_header.bV4Height			=  static_cast <int32_t>      ( BMPReader::H_BRAA::getResultValueFromFile(data_from_bmp_file[7])  ); 
   struct_object.bmp_dib_header.bmp_v4_header.bV4Planes			=  static_cast <uint16_t>	   ( BMPReader::H_BRAA::getResultValueFromFile(data_from_bmp_file[8])  ); 
   struct_object.bmp_dib_header.bmp_v4_header.bV4BitCount		=  static_cast <uint16_t>     ( BMPReader::H_BRAA::getResultValueFromFile(data_from_bmp_file[9])  );	
   struct_object.bmp_dib_header.bmp_v4_header.bV4V4Compression =	static_cast <uint32_t>	   ( BMPReader::H_BRAA::getResultValueFromFile(data_from_bmp_file[10]) );   
   struct_object.bmp_dib_header.bmp_v4_header.bV4SizeImage     =	static_cast <uint32_t>	   ( BMPReader::H_BRAA::getResultValueFromFile(data_from_bmp_file[11]) ); 	  
   struct_object.bmp_dib_header.bmp_v4_header.bV4XPelsPerMeter	=  static_cast <int32_t>      ( BMPReader::H_BRAA::getResultValueFromFile(data_from_bmp_file[12]) );
   struct_object.bmp_dib_header.bmp_v4_header.bV4YPelsPerMeter =	static_cast <int32_t>      ( BMPReader::H_BRAA::getResultValueFromFile(data_from_bmp_file[13]) );   
   struct_object.bmp_dib_header.bmp_v4_header.bV4ClrUsed			=  static_cast <uint32_t>	   ( BMPReader::H_BRAA::getResultValueFromFile(data_from_bmp_file[14]) );    
   struct_object.bmp_dib_header.bmp_v4_header.bV4ClrImportant	=  static_cast <uint32_t>	   ( BMPReader::H_BRAA::getResultValueFromFile(data_from_bmp_file[15]) ); 
   struct_object.bmp_dib_header.bmp_v4_header.bV4RedMask			=  static_cast <uint32_t>	   ( BMPReader::H_BRAA::getResultValueFromFile(data_from_bmp_file[16]) ); 
   struct_object.bmp_dib_header.bmp_v4_header.bV4GreenMask		=  static_cast <uint32_t>	   ( BMPReader::H_BRAA::getResultValueFromFile(data_from_bmp_file[17]) ); 
   struct_object.bmp_dib_header.bmp_v4_header.bV4BlueMask		=  static_cast <uint32_t>	   ( BMPReader::H_BRAA::getResultValueFromFile(data_from_bmp_file[18]) );	
   struct_object.bmp_dib_header.bmp_v4_header.bV4AlphaMask		=  static_cast <uint32_t>	   ( BMPReader::H_BRAA::getResultValueFromFile(data_from_bmp_file[19]) ); 
   struct_object.bmp_dib_header.bmp_v4_header.bV4CSType			=  static_cast <uint32_t>	   ( BMPReader::H_BRAA::getResultValueFromFile(data_from_bmp_file[20]) ); 
   //struct_object.bmp_dib_header.bmp_v4_header.bV4Endpoints	=  static_cast <CIEXYZTRIPLE>	( BMPReader::H_BRAA::getResultValueFromFile(data_from_bmp_file[21]) ); 
   struct_object.bmp_dib_header.bmp_v4_header.bV4GammaRed		=  static_cast <uint32_t>	   ( BMPReader::H_BRAA::getResultValueFromFile(data_from_bmp_file[22]) );	
   struct_object.bmp_dib_header.bmp_v4_header.bV4GammaGreen		=  static_cast <uint32_t>	   ( BMPReader::H_BRAA::getResultValueFromFile(data_from_bmp_file[23]) ); 
   struct_object.bmp_dib_header.bmp_v4_header.bV4GammaBlue		=  static_cast <uint32_t>	   ( BMPReader::H_BRAA::getResultValueFromFile(data_from_bmp_file[24]) ); 

   return struct_object;
}
//--------------------------------------------------------------------------------------------------------------
BMPFormat::Bitmap& BMPReader::BitmapReader::fill_DIB_V5_header (BMPFormat::Bitmap &struct_object, const std::vector <BMPReader::H_BRAA::Bytes> &data_from_bmp_file)
{
   /* BMP DIB V5 HEADER: */

   /*
   data_from_bmp_file:
   [5]   { 0x01, 0x02, 0x03, 0x04 },   =  The size of this structure in bytes, which also indicates the version of the structure:																												
   [6]   { 0x01, 0x02, 0x03, 0x04 },   =  The width of the raster in pixels. Specified as a signed integer. Zero and negative are not documented:																							
   [7]   { 0x01, 0x02, 0x03, 0x04 },   =  Two parameters: the height of the raster in pixels and the order of the rows in two-dimensional arrays:																							
   [8]   { 0x01, 0x02 },               =  In BMP, only the value 1 is allowed. This field is used in Windows icons and cursors:																													
   [9]   { 0x01, 0x02 },               =  Number of bits per pixel:																																																	
   [10]  { 0x01, 0x02, 0x03, 0x04 },   =  Indicates the way pixels are stored:																																													
   [11]  { 0x01, 0x02, 0x03, 0x04 },   =  The size of the pixel data in bytes. It can be reset to zero if the storage is carried out by a two-dimensional array:																		
   [12]  { 0x01, 0x02, 0x03, 0x04 },   =  The number of pixels per meter horizontally:																																											
   [13]  { 0x01, 0x02, 0x03, 0x04 },   =  The number of pixels per meter vertically:																																											
   [14]  { 0x01, 0x02, 0x03, 0x04 },   =  The size of the color table in cells:																																													
   [15]  { 0x01, 0x02, 0x03, 0x04 }    =  The number of cells from the beginning of the color table to the last one used (including itself):																									
   [16]  { 0x01, 0x02, 0x03, 0x04 },   =  Bitmasks for extracting channel values: RED intensity:																																							
   [17]  { 0x01, 0x02, 0x03, 0x04 },   =  Bitmasks for extracting channel values: GREEN intensity:																																							
   [18]  { 0x01, 0x02, 0x03, 0x04 },   =  Bitmasks for extracting channel values: BLUE intensity:																																							
   [19]  { 0x01, 0x02, 0x03, 0x04 },   =  Bitmasks for extracting channel values: ALPHA channels:																																							
   [20]  { 0x01, 0x02, 0x03, 0x04 },   =  A view of the color space:																																																	
   [21]  { (0x01) MULTIPLY (36)   },   =  The value of the field is taken into account only if the Tour field contains 0 (LCS_CALIBRATED_RGB). Then the endpoints are indicated in this field:								
   [22]  { 0x01, 0x02, 0x03, 0x04 },   =  The value of the field is taken into account only if the Type field contains 0 (LCS_CALIBRATED_RGB). Then the gamma value for the RED component is indicated in the field:	
   [23]  { 0x01, 0x02, 0x03, 0x04 },   =  The value of the field is taken into account only if the Type field contains 0 (LCS_CALIBRATED_RGB). Then the gamma value for the GREEN component is indicated in the field:
   [24]  { 0x01, 0x02, 0x03, 0x04 },   =  The value of the field is taken into account only if the Type field contains 0 (LCS_CALIBRATED_RGB). Then the gamma value for the BLUE component is indicated in the field:
   [25]  { 0x01, 0x02, 0x03, 0x04 },   =  Preferences for rendering the raster:
   [26]  { 0x01, 0x02, 0x03, 0x04 },   =  The byte offset of the color profile from the beginning of the BITMAPINFO:
   [27]  { 0x01, 0x02, 0x03, 0x04 },   =  If the color profile is directly included in the BMP, then its size in bytes is indicated here:	
   [28]  { 0x01, 0x02, 0x03, 0x04 },   =  Reserved and must be reset to zero:	
   */

   struct_object.setVersion(H_VERSION_DIB_V5_HEADER);

   struct_object.bmp_dib_header.bmp_v5_header.bV5Size				   =  static_cast <uint16_t>     ( BMPReader::H_BRAA::getResultValueFromFile(data_from_bmp_file[5])    );
   struct_object.bmp_dib_header.bmp_v5_header.bV5Width				=  static_cast <int32_t>      ( BMPReader::H_BRAA::getResultValueFromFile(data_from_bmp_file[6])    );
   struct_object.bmp_dib_header.bmp_v5_header.bV5Height			   =  static_cast <int32_t>      ( BMPReader::H_BRAA::getResultValueFromFile(data_from_bmp_file[7])    );
   struct_object.bmp_dib_header.bmp_v5_header.bV5Planes			   =  static_cast <uint32_t>     ( BMPReader::H_BRAA::getResultValueFromFile(data_from_bmp_file[8])    );
   struct_object.bmp_dib_header.bmp_v5_header.bV5BitCount			=  static_cast <uint32_t>     ( BMPReader::H_BRAA::getResultValueFromFile(data_from_bmp_file[9])    );
   struct_object.bmp_dib_header.bmp_v5_header.bV5Compression		=  static_cast <uint32_t>     ( BMPReader::H_BRAA::getResultValueFromFile(data_from_bmp_file[10])   );
   struct_object.bmp_dib_header.bmp_v5_header.bV5SizeImage		   =  static_cast <uint32_t>     ( BMPReader::H_BRAA::getResultValueFromFile(data_from_bmp_file[11])   );
   struct_object.bmp_dib_header.bmp_v5_header.bV5XPelsPerMeter    = 	static_cast <int32_t>      ( BMPReader::H_BRAA::getResultValueFromFile(data_from_bmp_file[12])   );
   struct_object.bmp_dib_header.bmp_v5_header.bV5YPelsPerMeter	   =  static_cast <int32_t>      ( BMPReader::H_BRAA::getResultValueFromFile(data_from_bmp_file[13])   );
   struct_object.bmp_dib_header.bmp_v5_header.bV5ClrUsed			   =  static_cast <uint32_t>     ( BMPReader::H_BRAA::getResultValueFromFile(data_from_bmp_file[14])   );
   struct_object.bmp_dib_header.bmp_v5_header.bV5ClrImportant	   =  static_cast <uint32_t>     ( BMPReader::H_BRAA::getResultValueFromFile(data_from_bmp_file[15])   );
   struct_object.bmp_dib_header.bmp_v5_header.bV5RedMask			   =  static_cast <uint32_t>     ( BMPReader::H_BRAA::getResultValueFromFile(data_from_bmp_file[16])   );
   struct_object.bmp_dib_header.bmp_v5_header.bV5GreenMask		   =  static_cast <uint32_t>     ( BMPReader::H_BRAA::getResultValueFromFile(data_from_bmp_file[17])   );
   struct_object.bmp_dib_header.bmp_v5_header.bV5BlueMask			=  static_cast <uint32_t>     ( BMPReader::H_BRAA::getResultValueFromFile(data_from_bmp_file[18])   );
   struct_object.bmp_dib_header.bmp_v5_header.bV5AlphaMask		   =  static_cast <uint32_t>     ( BMPReader::H_BRAA::getResultValueFromFile(data_from_bmp_file[19])   );
   struct_object.bmp_dib_header.bmp_v5_header.bV5CSType			   =  static_cast <uint32_t>     ( BMPReader::H_BRAA::getResultValueFromFile(data_from_bmp_file[20])   );
   //struct_object.bmp_dib_header.bmp_v5_header.bV5Endpoints		   =  static_cast <CIEXYZTRIPLE> ( BMPReader::H_BRAA::getResultValueFromFile(data_from_bmp_file[21])   );
   struct_object.bmp_dib_header.bmp_v5_header.bV5GammaRed			=  static_cast <uint32_t>     ( BMPReader::H_BRAA::getResultValueFromFile(data_from_bmp_file[22])   );
   struct_object.bmp_dib_header.bmp_v5_header.bV5GammaGreen		   =  static_cast <uint32_t>     ( BMPReader::H_BRAA::getResultValueFromFile(data_from_bmp_file[23])   );
   struct_object.bmp_dib_header.bmp_v5_header.bV5GammaBlue		   =  static_cast <uint32_t>     ( BMPReader::H_BRAA::getResultValueFromFile(data_from_bmp_file[24])   );
   struct_object.bmp_dib_header.bmp_v5_header.bV5Intent			   =  static_cast <uint32_t>     ( BMPReader::H_BRAA::getResultValueFromFile(data_from_bmp_file[25])   );
   struct_object.bmp_dib_header.bmp_v5_header.bV5ProfileData		=  static_cast <uint32_t>     ( BMPReader::H_BRAA::getResultValueFromFile(data_from_bmp_file[26])   );
   struct_object.bmp_dib_header.bmp_v5_header.bV5ProfileSize		=  static_cast <uint32_t>     ( BMPReader::H_BRAA::getResultValueFromFile(data_from_bmp_file[27])   );
   struct_object.bmp_dib_header.bmp_v5_header.bV5Reserved			=  static_cast <uint32_t>     ( BMPReader::H_BRAA::getResultValueFromFile(data_from_bmp_file[28])   );

   return struct_object;
}
//--------------------------------------------------------------------------------------------------------------
BMPFormat::Bitmap& BMPReader::BitmapReader::fillBitmapStruct(BMPFormat::Bitmap &struct_object, const std::vector <BMPReader::H_BRAA::Bytes> &data_from_bmp_file)
{
   fill_FILE_header(struct_object, data_from_bmp_file);
   fill_DIB_header(struct_object, data_from_bmp_file);

   return struct_object;
}
//--------------------------------------------------------------------------------------------------------------
const unsigned long long BMPReader::H_BRAA::getResultValueFromFile(const BMPReader::H_BRAA::Bytes &set_of_bytes_object)
{
   unsigned long result_value {};
   size_t bytes_size = set_of_bytes_object.set_of_bytes.size();
   for (size_t i = 0; i < bytes_size; ++i)
      result_value |= set_of_bytes_object.set_of_bytes[i] << (i * 8);

   return result_value;
}
//--------------------------------------------------------------------------------------------------------------
const unsigned long long BMPReader::H_BRAA::getResultValueFromFile(const std::vector <unsigned char> &bitmapinfo_version_flag)
{
   unsigned long result_value {};
   size_t bytes_size = bitmapinfo_version_flag.size();
   for (size_t i = 0; i < bytes_size; ++i)
      result_value |= bitmapinfo_version_flag[i] << (i * 8);

   return result_value;
}
//--------------------------------------------------------------------------------------------------------------
inline void BMPReader::H_BRAA::ConvertToBitmapData (BMPReader::H_BRAA::HelperRefactor_xd &_helper_refactor_xd)
{
   std::vector <unsigned int> sizeof_types_header = _helper_refactor_xd.sizeof_types_CURRENT_header;

   size_t s_t_h_size = sizeof_types_header.size();
   for (size_t readed_fields = 0, readed_bytes = 0;    readed_fields < s_t_h_size;    ++readed_fields)
   {
      for (size_t unreaded_bytes = sizeof_types_header[readed_fields];    unreaded_bytes > 0;    --unreaded_bytes, ++readed_bytes)
         _helper_refactor_xd.set_of_bytes_object.set_of_bytes.push_back( _helper_refactor_xd.bytes_buffer[readed_bytes] );

      _helper_refactor_xd.set_of_bytes_header.push_back( _helper_refactor_xd.set_of_bytes_object );
      _helper_refactor_xd.set_of_bytes_object.set_of_bytes.clear();
   }
}
//--------------------------------------------------------------------------------------------------------------
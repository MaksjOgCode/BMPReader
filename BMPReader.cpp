#include "BMPReader.hpp"



/* class BMPReader::Reader: */
//--------------------------------------------------------------------------------------------------------------
BMPReader::Reader::Reader (const std::string &_input_path_to_file) : input_path_to_file(_input_path_to_file)
{
   if ( _input_path_to_file.empty() )
      throw std::invalid_argument("The file path is empty: [BMPReader::Reader -1]");

   if ( _input_path_to_file.length() < 4 )
      throw std::invalid_argument("Supported only .bmp format: [OR] The file has no name: [BMPReader::Reader -2]");

   if ( _input_path_to_file.substr( input_path_to_file.length() - 4 ) != ".bmp" )
      throw std::invalid_argument("Supported only .bmp format [BMPReader::Reader -3]");
}
//--------------------------------------------------------------------------------------------------------------
const bool BMPReader::Reader::openBMP ()
{
   input_bmp_file.open(input_path_to_file, std::ios::binary);
   if ( !input_bmp_file.is_open() )
      throw std::invalid_argument("Failed to open the file: [BMPReader::Reader -4]");

   return input_bmp_file.is_open();
}
//--------------------------------------------------------------------------------------------------------------
const bool BMPReader::Reader::closeBMP ()
{
   if ( input_bmp_file.is_open() )
   {
      input_bmp_file.close();
      return true;
   }

   return false;
}
//--------------------------------------------------------------------------------------------------------------
void BMPReader::Reader::readAndFillBMPStruct(BMPFormat::Bitmap &bitmap_struct_object)
{
   if ( !input_bmp_file.is_open() )
   {
      throw std::invalid_argument("The incoming file is not open: [BMPReader::Reader -5]");
      return;
   }

   try
   {
      std::vector <BMPReader::H_BRAA::Bytes> temp_readed_data1 {};
      temp_readed_data1 = BMPReader::Reader_FILE_Header::readBMP_FILE_Header(*this);
      if ( temp_readed_data1.empty() )
         throw std::invalid_argument("File reading error [FILE header]: [BMPReader::Reader -6]");

      if (BMPReader::H_BRAA::convertToOneByteFromFile(temp_readed_data1[0].byte_vector) != H_IS_BMP_FORMAT)
         throw std::invalid_argument("Supported only .bmp format [BMPReader::Reader -3]");

      std::vector <BMPReader::H_BRAA::Bytes> temp_readed_data2 {};
      temp_readed_data2 = BMPReader::Reader_DIB_Header::readBMP_DIB_Header(*this);
      if ( temp_readed_data2.empty() )
         throw std::invalid_argument("This one has a non-standard DIB Header .bmp file [DIB Header]: [BMPReader::Reader -6]");

      /* Move the cursor to the place indicating the beginning of the pixel data: */
      input_bmp_file.seekg(  BMPReader::H_BRAA::convertToOneByteFromFile(temp_readed_data1[4].byte_vector), std::ios::beg);
      std::vector <BMPReader::H_BRAA::Bytes> temp_readed_data3 {};
      temp_readed_data3 = BMPReader::Reader_Data_Pixel::readBMP_PIXEL_DATA(*this, temp_readed_data2);
      if ( temp_readed_data3.empty() )
         throw std::invalid_argument("Only supported 24 bits [OR] 32 bits on color: [DATA PIXEL]: [BMPReader::Reader -7]");

      readed_FILE_DIB_headers = temp_readed_data1;
      readed_FILE_DIB_headers.insert( readed_FILE_DIB_headers.end(), temp_readed_data2.begin(), temp_readed_data2.end() );

      readed_PIXEL_DATA = temp_readed_data3;

      BMPReader::Filler::fillBMP_FILE_Header(*this, bitmap_struct_object);
      BMPReader::Filler::fillBMP_DIB_CURRENT_Header(*this, bitmap_struct_object);
      BMPReader::Filler::fillBMP_PIXEL_DATA(*this, bitmap_struct_object);
   }
   catch (const std::invalid_argument &exception)
   {
      std::cerr << exception.what() << "\n";
      return;
   }
}
//--------------------------------------------------------------------------------------------------------------



/* class BMPReader::Reader_FILE_Header: */
//--------------------------------------------------------------------------------------------------------------
std::vector <BMPReader::H_BRAA::Bytes> BMPReader::Reader_FILE_Header::readBMP_FILE_Header(Reader &reader_object)
{
   uint64_t sizeof_FILE_header = BMPFormat::Bitmap_FILE_Header::getSizeof_FILE_Header();
   BMPReader::HelperRefactorXD helper_refactor_xd_object ( sizeof_FILE_header );

   /* Resetting the cursor to the beginning of the file: */
   reader_object.input_bmp_file.seekg(0, std::ios::beg);
   reader_object.input_bmp_file.read( reinterpret_cast<char*>( helper_refactor_xd_object.bytes_buffer.data() ), helper_refactor_xd_object.sizeof_CURRENT_header );

   /* Move the cursor to the number of bytes read (sizeof_FILE_header): */
   reader_object.input_bmp_file.seekg(sizeof_FILE_header, std::ios::beg);

   BMPReader::H_BRAA::ConvertReadedData(helper_refactor_xd_object);

   return helper_refactor_xd_object.set_of_bytes;
}
//--------------------------------------------------------------------------------------------------------------



/* class BMPReader::Reader_DIB_Header: */
//--------------------------------------------------------------------------------------------------------------
std::vector <BMPReader::H_BRAA::Bytes> BMPReader::Reader_DIB_Header::readBMP_DIB_Header(Reader &reader_object)
{
   std::vector <uint8_t> temp_flag_version (4);
   reader_object.input_bmp_file.read ( reinterpret_cast<char*>( temp_flag_version.data() ), 4 );
   uint64_t flag_version = BMPReader::H_BRAA::convertToOneByteFromFile(temp_flag_version);

   switch (flag_version)
   {
   case H_BITMAP_DIB_CORE_HEADER:
      return readBMP_DIB_CURRENT_Header(reader_object, H_BITMAP_DIB_CORE_HEADER);
      break;

   case H_BITMAP_DIB_INFO_HEADER:
      return readBMP_DIB_CURRENT_Header(reader_object, H_BITMAP_DIB_INFO_HEADER);
      break;

   case H_BITMAP_DIB_V4_HEADER:
      return readBMP_DIB_CURRENT_Header(reader_object, H_BITMAP_DIB_V4_HEADER);
      break;

   case H_BITMAP_DIB_V5_HEADER:
      return readBMP_DIB_CURRENT_Header(reader_object, H_BITMAP_DIB_V5_HEADER);
      break;

   default:
      return { {} };
      break;
   }

   return { {} };
}
//--------------------------------------------------------------------------------------------------------------
std::vector <BMPReader::H_BRAA::Bytes> BMPReader::Reader_DIB_Header::readBMP_DIB_CURRENT_Header(Reader &reader_object, const uint8_t  &flag_version_dib)
{
   /* Return the cursor for correct further reading: */
   reader_object.input_bmp_file.seekg( BMPFormat::Bitmap_FILE_Header::getSizeof_FILE_Header(), std::ios::beg);

   if (flag_version_dib == H_BITMAP_DIB_CORE_HEADER)
   {
      uint64_t sizeof_DIB_CORE_header = BMPFormat::Bitmap_DIB_CORE_Header::getSizeof_DIB_CORE_Header();
      BMPReader::HelperRefactorXD helper_refactor_xd_object ( sizeof_DIB_CORE_header );
      reader_object.input_bmp_file.read( reinterpret_cast<char*>( helper_refactor_xd_object.bytes_buffer.data() ), helper_refactor_xd_object.sizeof_CURRENT_header );
      BMPReader::H_BRAA::ConvertReadedData(helper_refactor_xd_object);
      return helper_refactor_xd_object.set_of_bytes;

   } else if (flag_version_dib == H_BITMAP_DIB_INFO_HEADER)
   {
      uint64_t sizeof_DIB_INFO_header = BMPFormat::Bitmap_DIB_INFO_Header::getSizeof_DIB_INFO_Header();
      BMPReader::HelperRefactorXD helper_refactor_xd_object ( sizeof_DIB_INFO_header );
      reader_object.input_bmp_file.read( reinterpret_cast<char*>( helper_refactor_xd_object.bytes_buffer.data() ), helper_refactor_xd_object.sizeof_CURRENT_header );
      BMPReader::H_BRAA::ConvertReadedData(helper_refactor_xd_object);
      return helper_refactor_xd_object.set_of_bytes;

   } else if (flag_version_dib == H_BITMAP_DIB_V4_HEADER)
   {
      uint64_t sizeof_DIB_V4_header = BMPFormat::Bitmap_DIB_V4_Header::getSizeof_DIB_V4_Header();
      BMPReader::HelperRefactorXD helper_refactor_xd_object ( sizeof_DIB_V4_header );
      reader_object.input_bmp_file.read( reinterpret_cast<char*>( helper_refactor_xd_object.bytes_buffer.data() ), helper_refactor_xd_object.sizeof_CURRENT_header );
      BMPReader::H_BRAA::ConvertReadedData(helper_refactor_xd_object);
      return helper_refactor_xd_object.set_of_bytes;

   } else if (flag_version_dib == H_BITMAP_DIB_V5_HEADER)
   {
      uint64_t sizeof_DIB_V5_header = BMPFormat::Bitmap_DIB_V5_Header::getSizeof_DIB_V5_Header();
      BMPReader::HelperRefactorXD helper_refactor_xd_object ( sizeof_DIB_V5_header );
      reader_object.input_bmp_file.read( reinterpret_cast<char*>( helper_refactor_xd_object.bytes_buffer.data() ), helper_refactor_xd_object.sizeof_CURRENT_header );
      BMPReader::H_BRAA::ConvertReadedData(helper_refactor_xd_object);
      return helper_refactor_xd_object.set_of_bytes;

   } else {
      return { {} };
   }

   return { {} };
}
//--------------------------------------------------------------------------------------------------------------



/* class BMPReader::Reader_Data_Pixel: */
//--------------------------------------------------------------------------------------------------------------
inline std::vector <BMPReader::H_BRAA::Bytes> BMPReader::Reader_Data_Pixel::readBMP_PIXEL_DATA (Reader &reader_object, std::vector <BMPReader::H_BRAA::Bytes> &readed_DIB_Header)
{

   uint64_t image_width        = BMPReader::H_BRAA::convertToOneByteFromFile(readed_DIB_Header[1].byte_vector);
   uint64_t image_height       = BMPReader::H_BRAA::convertToOneByteFromFile(readed_DIB_Header[2].byte_vector);
   uint64_t clr_bit_count      = BMPReader::H_BRAA::convertToOneByteFromFile(readed_DIB_Header[4].byte_vector);
   uint64_t copy_clr_bit_count = clr_bit_count;

   /* ¬–≈Ã≈ÕÕ¿ﬂ ÀŒ√» ¿ (“≈—“Œ¬¿ﬂ ◊¿—“‹). (¬ ƒ¿À‹Õ≈…ÿ≈Ã ƒŒ–¿¡Œ“¿“‹) */
   /* ONLY 24 BYTES || 32 BYTES */
   const uint8_t bits = 8;
   if (copy_clr_bit_count % bits == 0)
   {
      if (clr_bit_count != 24 && clr_bit_count != 32)
         return {};

      copy_clr_bit_count = clr_bit_count / 8;
      BMPReader::HelperRefactorXD helper_refactor_xd_object ( (image_width * image_height) * copy_clr_bit_count );



      reader_object.input_bmp_file.read( reinterpret_cast<char*>( helper_refactor_xd_object.bytes_buffer.data() ), (image_width * image_height) * copy_clr_bit_count );

      BMPReader::H_BRAA::Bytes temp_set_of_bytes {};
      for (size_t i = 0; i < helper_refactor_xd_object.bytes_buffer.size(); ++i)
      {
         temp_set_of_bytes.byte_vector.push_back( helper_refactor_xd_object.bytes_buffer[i] );
         if ( (i + 1) % copy_clr_bit_count == 0 )
         {
            helper_refactor_xd_object.set_of_bytes.push_back(temp_set_of_bytes);
            temp_set_of_bytes.byte_vector.clear();
         }
      }

      return helper_refactor_xd_object.set_of_bytes;
   }

   return {{}};
}
//--------------------------------------------------------------------------------------------------------------



/* class BMPReader::HelperRedactorXD: */
//--------------------------------------------------------------------------------------------------------------
BMPReader::HelperRefactorXD::HelperRefactorXD(const uint64_t &_sizeof_CURRENT_header)
{
   this->sizeof_CURRENT_header = _sizeof_CURRENT_header;
   bytes_buffer.resize( _sizeof_CURRENT_header );

   switch (_sizeof_CURRENT_header)
   {
   case H_BITMAP_FILE_HEADER:
      this->sizeof_types_CURRENT_header = BMPFormat::Bitmap_FILE_Header::getSizeof_Types_FILE_Header();
      break;

   case H_BITMAP_DIB_CORE_HEADER:
      this->sizeof_types_CURRENT_header = BMPFormat::Bitmap_DIB_CORE_Header::getSizeof_Types_DIB_CORE_Header();
      break;

   case H_BITMAP_DIB_INFO_HEADER:
      this->sizeof_types_CURRENT_header = BMPFormat::Bitmap_DIB_INFO_Header::getSizeof_Types_DIB_INFO_Header();
      break;

   case H_BITMAP_DIB_V4_HEADER:
      this->sizeof_types_CURRENT_header = BMPFormat::Bitmap_DIB_V4_Header::getSizeof_Types_DIB_V4_Header();
      break;

   case H_BITMAP_DIB_V5_HEADER:
      this->sizeof_types_CURRENT_header = BMPFormat::Bitmap_DIB_V5_Header::getSizeof_Types_DIB_V5_Header();
      break;

   default:
      break;
   }
}
//--------------------------------------------------------------------------------------------------------------



/* class BMPReader::BitmapReaderAlgorithmsAPI (H_BRAA): */
//--------------------------------------------------------------------------------------------------------------
inline void BMPReader::H_BRAA::ConvertReadedData(BMPReader::HelperRefactorXD &helper_refactor_xd_object)
{
   std::vector <uint8_t> sizeof_types_header = helper_refactor_xd_object.sizeof_types_CURRENT_header;

   /* scth = sizeof types current header: */
   size_t stch_size = sizeof_types_header.size();
   for (size_t readed_fields = 0, readed_bytes = 0;     readed_fields < stch_size;     ++readed_fields)
   {
      for (size_t unreaded_bytes = sizeof_types_header[readed_fields];     unreaded_bytes > 0;     --unreaded_bytes, ++readed_bytes)
         helper_refactor_xd_object.bytes.byte_vector.push_back( helper_refactor_xd_object.bytes_buffer[readed_bytes] );

      helper_refactor_xd_object.set_of_bytes.push_back( helper_refactor_xd_object.bytes );
      helper_refactor_xd_object.bytes.byte_vector.clear();
   }
}
//--------------------------------------------------------------------------------------------------------------
inline const uint64_t BMPReader::H_BRAA::convertToOneByteFromFile(const std::vector <uint8_t> &set_of_bytes)
{
   uint64_t result_value = 0;
   size_t bytes_size = set_of_bytes.size();
   for (size_t i = 0; i < bytes_size; ++i)
      result_value |= static_cast<uint64_t>( set_of_bytes[i] ) << (i * 8);

   return result_value;
}
//--------------------------------------------------------------------------------------------------------------



/* class BMPReader::Filler: */
//--------------------------------------------------------------------------------------------------------------
inline void BMPReader::Filler::fillBMP_FILE_Header(Reader &reader_object, BMPFormat::Bitmap &bitmap_struct_object)
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
   if (BMPReader::H_BRAA::convertToOneByteFromFile(reader_object.readed_FILE_DIB_headers[0].byte_vector) != H_IS_BMP_FORMAT)
      return;

   //struct_object.bmp_file_header.bf_type          = static_cast <uint16_t>( BMPReader::BRAA::getResultValueFromFile(data_from_bmp_file[0]) ); CONST INTO STRUCT:
   bitmap_struct_object.bmp_file_header.bf_size     = static_cast <uint32_t>( BMPReader::H_BRAA::convertToOneByteFromFile(reader_object.readed_FILE_DIB_headers[1].byte_vector) );
   /* data_from_bmp_file[2]: Reserved and must contain zero: */
   /* data_from_bmp_file[3]: Reserved and must contain zero: */
   bitmap_struct_object.bmp_file_header.bf_off_bits = static_cast <uint32_t>( BMPReader::H_BRAA::convertToOneByteFromFile(reader_object.readed_FILE_DIB_headers[4].byte_vector) );

   return;
}
//--------------------------------------------------------------------------------------------------------------
inline void BMPReader::Filler::fillBMP_DIB_CURRENT_Header(Reader &reader_object, BMPFormat::Bitmap &bitmap_struct_object)
{

   switch( BMPReader::H_BRAA::convertToOneByteFromFile(reader_object.readed_FILE_DIB_headers[5].byte_vector) )
   {
      break;
   case H_BITMAP_DIB_CORE_HEADER:
      BMPReader::Filler::fillBMP_DIB_CORE_Header(reader_object, bitmap_struct_object);
      break;

   case H_BITMAP_DIB_INFO_HEADER:
      BMPReader::Filler::fillBMP_DIB_INFO_Header(reader_object, bitmap_struct_object);
      break;

   case H_BITMAP_DIB_V4_HEADER:
      BMPReader::Filler::fillBMP_DIB_V4_Header(reader_object, bitmap_struct_object);
      break;

   case H_BITMAP_DIB_V5_HEADER:
      BMPReader::Filler::fillBMP_DIB_V5_Header(reader_object, bitmap_struct_object);
      break;

   default:
      return;
      break;
   }

   return;
}
//--------------------------------------------------------------------------------------------------------------
inline void BMPReader::Filler::fillBMP_DIB_CORE_Header(Reader &reader_object, BMPFormat::Bitmap &bitmap_struct_object)
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

   bitmap_struct_object.setVersion(H_VERSION_DIB_CORE_HEADER);

   bitmap_struct_object.bmp_dib_header.bmp_core_header.bc_size      =  static_cast <uint32_t> ( BMPReader::H_BRAA::convertToOneByteFromFile( reader_object.readed_FILE_DIB_headers[5].byte_vector) );
   bitmap_struct_object.bmp_dib_header.bmp_core_header.bc_width     =  static_cast <uint16_t> ( BMPReader::H_BRAA::convertToOneByteFromFile( reader_object.readed_FILE_DIB_headers[6].byte_vector) );
   bitmap_struct_object.bmp_dib_header.bmp_core_header.bc_height    =  static_cast <uint16_t> ( BMPReader::H_BRAA::convertToOneByteFromFile( reader_object.readed_FILE_DIB_headers[7].byte_vector) );
   //bitmap_struct_object.bmp_dib_header.bmp_core_header.bc_planes    =  static_cast <uint16_t> ( BMPReader::H_BRAA::convertToOneByteFromFile( reader_object.readed_FILE_DIB_headers[8].byte_vector) );
   bitmap_struct_object.bmp_dib_header.bmp_core_header.bc_bit_count =  static_cast <uint16_t> ( BMPReader::H_BRAA::convertToOneByteFromFile( reader_object.readed_FILE_DIB_headers[9].byte_vector) );

   return;
}
//--------------------------------------------------------------------------------------------------------------
inline void BMPReader::Filler::fillBMP_DIB_INFO_Header(Reader &reader_object, BMPFormat::Bitmap &bitmap_struct_object)
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

   bitmap_struct_object.setVersion(H_VERSION_DIB_INFO_HEADER);

   bitmap_struct_object.bmp_dib_header.bmp_dib_info_header.bi_size			    =  static_cast <uint32_t> ( BMPReader::H_BRAA::convertToOneByteFromFile( reader_object.readed_FILE_DIB_headers[5].byte_vector)  ); 
   bitmap_struct_object.bmp_dib_header.bmp_dib_info_header.bi_width			    =  static_cast <int32_t>	( BMPReader::H_BRAA::convertToOneByteFromFile( reader_object.readed_FILE_DIB_headers[6].byte_vector)  );   
   bitmap_struct_object.bmp_dib_header.bmp_dib_info_header.bi_height			    =  static_cast <int32_t>  ( BMPReader::H_BRAA::convertToOneByteFromFile( reader_object.readed_FILE_DIB_headers[7].byte_vector)  ); 
   //bitmap_struct_object.bmp_dib_header.bmp_dib_info_header.bi_planes			    =  static_cast <uint16_t>	( BMPReader::H_BRAA::getResultValueFromFile(data_from_bmp_file[8])  );   
   bitmap_struct_object.bmp_dib_header.bmp_dib_info_header.bi_bit_count		    =  static_cast <uint16_t> ( BMPReader::H_BRAA::convertToOneByteFromFile( reader_object.readed_FILE_DIB_headers[9].byte_vector)  ); 
   bitmap_struct_object.bmp_dib_header.bmp_dib_info_header.bi_compression	    =  static_cast <uint32_t>	( BMPReader::H_BRAA::convertToOneByteFromFile( reader_object.readed_FILE_DIB_headers[10].byte_vector) );   
   bitmap_struct_object.bmp_dib_header.bmp_dib_info_header.bi_size_image		 =  static_cast <uint32_t>	( BMPReader::H_BRAA::convertToOneByteFromFile( reader_object.readed_FILE_DIB_headers[11].byte_vector) );   
   bitmap_struct_object.bmp_dib_header.bmp_dib_info_header.bi_x_pels_per_meter =  static_cast <int32_t>  ( BMPReader::H_BRAA::convertToOneByteFromFile( reader_object.readed_FILE_DIB_headers[12].byte_vector) ); 
   bitmap_struct_object.bmp_dib_header.bmp_dib_info_header.bi_y_pels_per_meter =  static_cast <int32_t>  ( BMPReader::H_BRAA::convertToOneByteFromFile( reader_object.readed_FILE_DIB_headers[13].byte_vector) ); 
   bitmap_struct_object.bmp_dib_header.bmp_dib_info_header.bi_Òlr_used		    =  static_cast <uint32_t>	( BMPReader::H_BRAA::convertToOneByteFromFile( reader_object.readed_FILE_DIB_headers[14].byte_vector) );   
   bitmap_struct_object.bmp_dib_header.bmp_dib_info_header.bi_Òlr_important	 =  static_cast <uint32_t>	( BMPReader::H_BRAA::convertToOneByteFromFile( reader_object.readed_FILE_DIB_headers[15].byte_vector) );   

   return;
}
//--------------------------------------------------------------------------------------------------------------
inline void BMPReader::Filler::fillBMP_DIB_V4_Header(Reader &reader_object, BMPFormat::Bitmap &bitmap_struct_object)
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

   bitmap_struct_object.setVersion(H_VERSION_DIB_V4_HEADER);

   bitmap_struct_object.bmp_dib_header.bmp_dib_v4_header.bv4_size				   =  static_cast <uint32_t>     ( BMPReader::H_BRAA::convertToOneByteFromFile( reader_object.readed_FILE_DIB_headers[5].byte_vector)  ); 
   bitmap_struct_object.bmp_dib_header.bmp_dib_v4_header.bv4_width			   =  static_cast <int32_t>	   ( BMPReader::H_BRAA::convertToOneByteFromFile( reader_object.readed_FILE_DIB_headers[6].byte_vector)  );	
   bitmap_struct_object.bmp_dib_header.bmp_dib_v4_header.bv4_height			   =  static_cast <int32_t>      ( BMPReader::H_BRAA::convertToOneByteFromFile( reader_object.readed_FILE_DIB_headers[7].byte_vector)  ); 
   //bitmap_struct_object.bmp_dib_header.bmp_dib_v4_header.bv4_planes			=  static_cast <uint16_t>	   ( BMPReader::H_BRAA::convertToOneByteFromFile( reader_object.readed_FILE_DIB_headers[8].byte_vector)  ); 
   bitmap_struct_object.bmp_dib_header.bmp_dib_v4_header.bv4_bit_count		   =  static_cast <uint16_t>     ( BMPReader::H_BRAA::convertToOneByteFromFile( reader_object.readed_FILE_DIB_headers[9].byte_vector)  );	
   bitmap_struct_object.bmp_dib_header.bmp_dib_v4_header.bv4_v4_compression   =	static_cast <uint32_t>	   ( BMPReader::H_BRAA::convertToOneByteFromFile( reader_object.readed_FILE_DIB_headers[10].byte_vector) );   
   bitmap_struct_object.bmp_dib_header.bmp_dib_v4_header.bv4_size_image       =	static_cast <uint32_t>	   ( BMPReader::H_BRAA::convertToOneByteFromFile( reader_object.readed_FILE_DIB_headers[11].byte_vector) ); 	  
   bitmap_struct_object.bmp_dib_header.bmp_dib_v4_header.bv4_x_pels_per_meter	=  static_cast <int32_t>      ( BMPReader::H_BRAA::convertToOneByteFromFile( reader_object.readed_FILE_DIB_headers[12].byte_vector) );
   bitmap_struct_object.bmp_dib_header.bmp_dib_v4_header.bv4_y_pels_per_meter =	static_cast <int32_t>      ( BMPReader::H_BRAA::convertToOneByteFromFile( reader_object.readed_FILE_DIB_headers[13].byte_vector) );   
   bitmap_struct_object.bmp_dib_header.bmp_dib_v4_header.bv4_clr_used			=  static_cast <uint32_t>	   ( BMPReader::H_BRAA::convertToOneByteFromFile( reader_object.readed_FILE_DIB_headers[14].byte_vector) );    
   bitmap_struct_object.bmp_dib_header.bmp_dib_v4_header.bv4_clr_important	   =  static_cast <uint32_t>	   ( BMPReader::H_BRAA::convertToOneByteFromFile( reader_object.readed_FILE_DIB_headers[15].byte_vector) ); 
   bitmap_struct_object.bmp_dib_header.bmp_dib_v4_header.bv4_red_mask			=  static_cast <uint32_t>	   ( BMPReader::H_BRAA::convertToOneByteFromFile( reader_object.readed_FILE_DIB_headers[16].byte_vector) ); 
   bitmap_struct_object.bmp_dib_header.bmp_dib_v4_header.bv4_green_mask		   =  static_cast <uint32_t>	   ( BMPReader::H_BRAA::convertToOneByteFromFile( reader_object.readed_FILE_DIB_headers[17].byte_vector) ); 
   bitmap_struct_object.bmp_dib_header.bmp_dib_v4_header.bv4_blue_mask		   =  static_cast <uint32_t>	   ( BMPReader::H_BRAA::convertToOneByteFromFile( reader_object.readed_FILE_DIB_headers[18].byte_vector) );	
   bitmap_struct_object.bmp_dib_header.bmp_dib_v4_header.bv4_alpha_mask		   =  static_cast <uint32_t>	   ( BMPReader::H_BRAA::convertToOneByteFromFile( reader_object.readed_FILE_DIB_headers[19].byte_vector) ); 
   bitmap_struct_object.bmp_dib_header.bmp_dib_v4_header.bv4_cs_type			   =  static_cast <uint32_t>	   ( BMPReader::H_BRAA::convertToOneByteFromFile( reader_object.readed_FILE_DIB_headers[20].byte_vector) ); 
   //struct_object.bmp_dib_header.bmp_v4_header.bV4Endpoints	               =  static_cast <CIEXYZTRIPLE>	( BMPReader::H_BRAA::convertToOneByteFromFile( reader_object.readed_FILE_DIB_headers[21].byte_vector) ); 
   bitmap_struct_object.bmp_dib_header.bmp_dib_v4_header.bv4_gamma_red		   =  static_cast <uint32_t>	   ( BMPReader::H_BRAA::convertToOneByteFromFile( reader_object.readed_FILE_DIB_headers[22].byte_vector) );	
   bitmap_struct_object.bmp_dib_header.bmp_dib_v4_header.bv4_gamma_green		   =  static_cast <uint32_t>	( BMPReader::H_BRAA::convertToOneByteFromFile( reader_object.readed_FILE_DIB_headers[23].byte_vector) ); 
   bitmap_struct_object.bmp_dib_header.bmp_dib_v4_header.bv4_gamma_blue		   =  static_cast <uint32_t>	   ( BMPReader::H_BRAA::convertToOneByteFromFile( reader_object.readed_FILE_DIB_headers[24].byte_vector) ); 

   return;
}
//--------------------------------------------------------------------------------------------------------------
inline void BMPReader::Filler::fillBMP_DIB_V5_Header(Reader &reader_object, BMPFormat::Bitmap &bitmap_struct_object)
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

   bitmap_struct_object.setVersion(H_VERSION_DIB_V5_HEADER);

   bitmap_struct_object.bmp_dib_header.bmp_dib_v5_header.bv5_size				   =  static_cast <uint16_t>     ( BMPReader::H_BRAA::convertToOneByteFromFile( reader_object.readed_FILE_DIB_headers[5].byte_vector)  );
   bitmap_struct_object.bmp_dib_header.bmp_dib_v5_header.bv5_width				=  static_cast <int32_t>      ( BMPReader::H_BRAA::convertToOneByteFromFile( reader_object.readed_FILE_DIB_headers[6].byte_vector)  );
   bitmap_struct_object.bmp_dib_header.bmp_dib_v5_header.bv5_height			   =  static_cast <int32_t>      ( BMPReader::H_BRAA::convertToOneByteFromFile( reader_object.readed_FILE_DIB_headers[7].byte_vector)  );
   //bitmap_struct_object.bmp_dib_header.bmp_dib_v5_header.bv5_planes			=  static_cast <uint32_t>     ( BMPReader::H_BRAA::convertToOneByteFromFile( reader_object.readed_FILE_DIB_headers[8].byte_vector)  );
   bitmap_struct_object.bmp_dib_header.bmp_dib_v5_header.bv5_bit_count			=  static_cast <uint32_t>     ( BMPReader::H_BRAA::convertToOneByteFromFile( reader_object.readed_FILE_DIB_headers[9].byte_vector)  );
   bitmap_struct_object.bmp_dib_header.bmp_dib_v5_header.bv5_compression		=  static_cast <uint32_t>     ( BMPReader::H_BRAA::convertToOneByteFromFile( reader_object.readed_FILE_DIB_headers[10].byte_vector) );
   bitmap_struct_object.bmp_dib_header.bmp_dib_v5_header.bv5_size_image		   =  static_cast <uint32_t>     ( BMPReader::H_BRAA::convertToOneByteFromFile( reader_object.readed_FILE_DIB_headers[11].byte_vector) );
   bitmap_struct_object.bmp_dib_header.bmp_dib_v5_header.bv5_x_pels_per_meter = 	static_cast <int32_t>      ( BMPReader::H_BRAA::convertToOneByteFromFile( reader_object.readed_FILE_DIB_headers[12].byte_vector) );
   bitmap_struct_object.bmp_dib_header.bmp_dib_v5_header.bv5_y_pels_per_meter =  static_cast <int32_t>      ( BMPReader::H_BRAA::convertToOneByteFromFile( reader_object.readed_FILE_DIB_headers[13].byte_vector) );
   bitmap_struct_object.bmp_dib_header.bmp_dib_v5_header.bv5_clr_used			=  static_cast <uint32_t>     ( BMPReader::H_BRAA::convertToOneByteFromFile( reader_object.readed_FILE_DIB_headers[14].byte_vector) );
   bitmap_struct_object.bmp_dib_header.bmp_dib_v5_header.bv5_clr_important	   =  static_cast <uint32_t>     ( BMPReader::H_BRAA::convertToOneByteFromFile( reader_object.readed_FILE_DIB_headers[15].byte_vector) );
   bitmap_struct_object.bmp_dib_header.bmp_dib_v5_header.bv5_red_mask			=  static_cast <uint32_t>     ( BMPReader::H_BRAA::convertToOneByteFromFile( reader_object.readed_FILE_DIB_headers[16].byte_vector) );
   bitmap_struct_object.bmp_dib_header.bmp_dib_v5_header.bv5_green_mask		   =  static_cast <uint32_t>     ( BMPReader::H_BRAA::convertToOneByteFromFile( reader_object.readed_FILE_DIB_headers[17].byte_vector) );
   bitmap_struct_object.bmp_dib_header.bmp_dib_v5_header.bv5_blue_mask			=  static_cast <uint32_t>     ( BMPReader::H_BRAA::convertToOneByteFromFile( reader_object.readed_FILE_DIB_headers[18].byte_vector) );
   bitmap_struct_object.bmp_dib_header.bmp_dib_v5_header.bv5_alpha_mask		   =  static_cast <uint32_t>     ( BMPReader::H_BRAA::convertToOneByteFromFile( reader_object.readed_FILE_DIB_headers[19].byte_vector) );
   bitmap_struct_object.bmp_dib_header.bmp_dib_v5_header.bv5_cs_type			   =  static_cast <uint32_t>     ( BMPReader::H_BRAA::convertToOneByteFromFile( reader_object.readed_FILE_DIB_headers[20].byte_vector) );
   //struct_object.bmp_dib_header.bmp_dib_v5_header.bV5Endpoints		         =  static_cast <CIEXYZTRIPLE> ( BMPReader::H_BRAA::convertToOneByteFromFile( reader_object.readed_FILE_DIB_headers[21].byte_vector) );
   bitmap_struct_object.bmp_dib_header.bmp_dib_v5_header.bv5_gamma_red			=  static_cast <uint32_t>     ( BMPReader::H_BRAA::convertToOneByteFromFile( reader_object.readed_FILE_DIB_headers[22].byte_vector) );
   bitmap_struct_object.bmp_dib_header.bmp_dib_v5_header.bv5_gamma_green		=  static_cast <uint32_t>     ( BMPReader::H_BRAA::convertToOneByteFromFile( reader_object.readed_FILE_DIB_headers[23].byte_vector) );
   bitmap_struct_object.bmp_dib_header.bmp_dib_v5_header.bv5_gamma_blue		   =  static_cast <uint32_t>     ( BMPReader::H_BRAA::convertToOneByteFromFile( reader_object.readed_FILE_DIB_headers[24].byte_vector) );
   bitmap_struct_object.bmp_dib_header.bmp_dib_v5_header.bv5_intent			   =  static_cast <uint32_t>     ( BMPReader::H_BRAA::convertToOneByteFromFile( reader_object.readed_FILE_DIB_headers[25].byte_vector) );
   bitmap_struct_object.bmp_dib_header.bmp_dib_v5_header.bv5_profile_data		=  static_cast <uint32_t>     ( BMPReader::H_BRAA::convertToOneByteFromFile( reader_object.readed_FILE_DIB_headers[26].byte_vector) );
   bitmap_struct_object.bmp_dib_header.bmp_dib_v5_header.bv5_profile_size		=  static_cast <uint32_t>     ( BMPReader::H_BRAA::convertToOneByteFromFile( reader_object.readed_FILE_DIB_headers[27].byte_vector) );
   bitmap_struct_object.bmp_dib_header.bmp_dib_v5_header.bv5_reserved			=  static_cast <uint32_t>     ( BMPReader::H_BRAA::convertToOneByteFromFile( reader_object.readed_FILE_DIB_headers[28].byte_vector) );

   return;
}
//--------------------------------------------------------------------------------------------------------------
inline void BMPReader::Filler::fillBMP_PIXEL_DATA(Reader &reader_object, BMPFormat::Bitmap &bitmap_struct_object)
{
   bitmap_struct_object.bmp_data_pixel.clear();

   for (size_t i = 0; i < reader_object.readed_PIXEL_DATA.size(); ++i)
      bitmap_struct_object.bmp_data_pixel.push_back(reader_object.readed_PIXEL_DATA[i].byte_vector);

   return;
}
//--------------------------------------------------------------------------------------------------------------
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
std::vector <BMPReader::BRAA::Bytes> BMPReader::BitmapReader::readBMPHeaderFile()
{
   BRAA::Bytes set_of_bytes_object {};
   std::vector <BRAA::Bytes> set_of_bytes_file_header {};

   if ( !bmp_file )  /* The File .bmp must be opened: */
      return set_of_bytes_file_header;

   /* BITMAPFILEHEADER is a 14—byte structure that is located at the very beginning of the file: */
   const unsigned int sizeof_file_header = BMPFormat::BitmapStruct::getSizeofFileHeader();

   /* A temporary buffer for each byte read: */
   std::vector <unsigned char> bytes_buffer ( BMPFormat::BitmapStruct::getSizeofFileHeader() );

   /* Reading 14 bytes from .bmp file and put into bytes_buffer: */
   bmp_file.read( reinterpret_cast<char*>( bytes_buffer.data() ), sizeof_file_header );

   /* We convert the read data into a convenient format for further work: */
   BMPReader::BRAA::ConvertToBMPData(set_of_bytes_object, set_of_bytes_file_header, bytes_buffer);

   return set_of_bytes_file_header;
}
//--------------------------------------------------------------------------------------------------------------
std::vector <BMPReader::BRAA::Bytes>  BMPReader::BitmapReader::readBMPDIBHeader()
{
   BRAA::Bytes set_of_bytes_object {};
   std::vector <BRAA::Bytes> set_of_bytes_file_header {};

   if ( !bmp_file )  /* The File .bmp must be opened: */
      return set_of_bytes_file_header;

   /* BITMAPFILEHEADER is a 14—byte structure that is located at the very beginning of the file: */
   const unsigned int sizeof_file_header = BMPFormat::BitmapStruct::getSizeofFileHeader();

   /* Move the cursor to the number of bytes read (14): */
   bmp_file.seekg(sizeof_file_header, std::ios::beg);

   /* get bcSize or biSize: The size of this structure in bytes, indicating the version of the structure: */
   std::vector <unsigned char> bitmapinfo_version_flag (4);
   /* 12 bytes = CORE; 40 bytes = V3; 108 bytes = V4; 124 bytes = V5: */
   bmp_file.read( reinterpret_cast<char*>( bitmapinfo_version_flag.data() ), 4 );

   /* We convert the read data into a convenient format for further work: */
   BMPReader::BRAA::ConvertToBMPData(set_of_bytes_object, set_of_bytes_file_header, bitmapinfo_version_flag);

   switch( BMPReader::BRAA::getResultValueFromFile(set_of_bytes_file_header[0]) )
   {
   case BITMAPCOREHEADER:
      break;

   case BITMAPINFOHEADER:
      break;

   case BITMAPV4HEADER:
      break;

   case BITMAPV5HEADER:
      break;

   default:
      break;
   }

   return set_of_bytes_file_header;
}
//--------------------------------------------------------------------------------------------------------------
bool BMPReader::BitmapReader::fillBMPStruct(BMPFormat::BitmapStruct &struct_object)
{
   data_from_bmp_file = readBMPDIBHeader();//readBMPHeaderFile();

   if ( data_from_bmp_file.empty() )
      return false;

   /*
   data_from_bmp_file:
   { 0x00, 0x00 },               = A mark to distinguish the format from others. Must contain 4D42[16]/424D[16]:
   { 0x00, 0x00, 0x00, 0x00 },   = The file size in bytes:
   { 0x00, 0x00 },               = Reserved and must contain zero:
   { 0x00, 0x00 },               = Reserved and must contain zero:
   { 0x00, 0x00, 0x00, 0x00 }    = The position of the pixel data relative to the beginning of this structure:
   */

   if ( static_cast <uint16_t>( BMPReader::BRAA::getResultValueFromFile(data_from_bmp_file[0]) ) != 0x4d42 )    /* A mark to distinguish the format from others (format signature): */
      return false;

   /**/
   struct_object.bmp_file_header.bf_type    =  static_cast <uint16_t>( BMPReader::BRAA::getResultValueFromFile(data_from_bmp_file[0]) );
   struct_object.bmp_file_header.bf_size    =  static_cast <uint32_t>( BMPReader::BRAA::getResultValueFromFile(data_from_bmp_file[1]) );
   /* data_from_bmp_file[2]: Reserved and must contain zero: */
   /* data_from_bmp_file[3]: Reserved and must contain zero: */
   struct_object.bmp_file_header.bf_off_bits = static_cast <uint32_t>( BMPReader::BRAA::getResultValueFromFile(data_from_bmp_file[4]) );

   return true;
}
//--------------------------------------------------------------------------------------------------------------
const unsigned long long BMPReader::BRAA::getResultValueFromFile(BMPReader::BRAA::Bytes &set_of_bytes_object)
{
   unsigned long result_value {};
   size_t bytes_size = set_of_bytes_object.set_of_bytes.size();
   for (size_t i = 0; i < bytes_size; ++i)
      result_value |= set_of_bytes_object.set_of_bytes[i] << (i * 8);

   return result_value;
}
//--------------------------------------------------------------------------------------------------------------
inline void BMPReader::BRAA::ConvertToBMPData  (   BMPReader::BRAA::Bytes                &set_of_bytes_object,
                                                   std::vector <BMPReader::BRAA::Bytes>  &set_of_bytes_file_header,
                                                   const std::vector <unsigned char>     &bytes_buffer  )
{
   /* To further combine bytes into groups of a specific structure: */
   const std::vector <unsigned int> sizeof_types_file_header = BMPFormat::BitmapStruct::getSizeofTypesFileHeader();

   for (size_t readed_fields = 0, readed_bytes = 0; readed_fields < sizeof_types_file_header.size(); ++readed_fields)
   {
      for (size_t unreaded_bytes = sizeof_types_file_header[readed_fields]; unreaded_bytes > 0; --unreaded_bytes, ++readed_bytes)
         set_of_bytes_object.set_of_bytes.push_back( bytes_buffer[readed_bytes] );

      set_of_bytes_file_header.push_back( set_of_bytes_object );
      set_of_bytes_object.set_of_bytes.clear();
   }
}
//--------------------------------------------------------------------------------------------------------------
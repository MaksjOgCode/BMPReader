#ifndef BMPREADER_HPP
#define BMPREADER_HPP



#include "BMPFormat.hpp"



#include <vector>
#include <string>
#include <fstream>



namespace BMPReader
{
//--------------------------------------------------------------------------------------------------------------
   class BitmapReader;

   class H_BRAA
   {
   private:
      friend class BitmapReader;

      struct Bytes
      {
         std::vector <unsigned long> set_of_bytes;
      };

      struct HelperRefactor_xd
      {
      public:
         HelperRefactor_xd (const unsigned int &_sizeof_CURRENT_header)
            : sizeof_CURRENT_header(_sizeof_CURRENT_header)
         {
            bytes_buffer.resize( sizeof_CURRENT_header );

            switch (_sizeof_CURRENT_header)
            {
            case H_BITMAPFILEHEADER:
               this->sizeof_types_CURRENT_header = BMPFormat::Bitmap_File_Header::getSizeof_Types_FILE_Header();
               break;

            case H_BITMAPCOREHEADER:
               this->sizeof_types_CURRENT_header = BMPFormat::Bitmap_CORE_Header::getSizeof_Types_DIB_CORE_Header();
               break;

            case H_BITMAPINFOHEADER:
               this->sizeof_types_CURRENT_header = BMPFormat::Bitmap_INFO_Header::getSizeof_Types_DIB_INFO_Header();
               break;

            case H_BITMAPV4HEADER:
               this->sizeof_types_CURRENT_header = BMPFormat::Bitmap_V4_Header::getSizeof_Types_DIB_V4_Header();
               break;

            case H_BITMAPV5HEADER:
               this->sizeof_types_CURRENT_header = BMPFormat::Bitmap_V5_Header::getSizeof_Types_DIB_V5_Header();
               break;

            default:
               break;
            }
         }
      public:
         H_BRAA::Bytes set_of_bytes_object                          {};
         std::vector <H_BRAA::Bytes> set_of_bytes_header            {};
      public:
         const unsigned int sizeof_CURRENT_header                 {};
         std::vector <unsigned int> sizeof_types_CURRENT_header   {};
      public:
         std::vector <unsigned char> bytes_buffer                 {};
      };

   private:
      /* Full Static class: */
      H_BRAA () = delete;
      H_BRAA (const BitmapReaderAlgorithmsAPI&) = delete;
      H_BRAA &operator=(const BitmapReaderAlgorithmsAPI&) = delete;

   private:
      static const unsigned long long getResultValueFromFile (const Bytes &set_of_bytes);
      static const unsigned long long getResultValueFromFile(const std::vector <unsigned char>& bitmapinfo_version_flag);

      static inline void ConvertToBitmapData ( BMPReader::H_BRAA::HelperRefactor_xd& _helper_refactor_xd );
   };
//--------------------------------------------------------------------------------------------------------------
   class BitmapReader
   {
   private:
      friend class BMPReader::H_BRAA;

      /* Only with parametre = const std::string &input_file_name: */
      BitmapReader() = delete;
      BitmapReader (const BitmapReaderAlgorithmsAPI&) = delete;
      BitmapReader &operator=(const BitmapReaderAlgorithmsAPI&) = delete;

   private:
      std::string    path_to_file {};  /* The field that contains the path to the file */
      std::ifstream  bmp_file;         /* Input Image .bmp:                            */

      std::vector <BMPReader::H_BRAA::Bytes>  data_from_bmp_file  {};    /* Vector that contains the read data from the file:  */
   private:
      std::vector <BMPReader::H_BRAA::Bytes>  pixel_data  {};    /* Vector that contains the read data from the file:  */
   private:
      std::vector <BMPReader::H_BRAA::Bytes>  readBMP_FILE_Header();
      std::vector <BMPReader::H_BRAA::Bytes>  readBMP_DIB_Header();

      std::vector <BMPReader::H_BRAA::Bytes>  readBMPFile();
   private:
      std::vector <BMPReader::H_BRAA::Bytes> getPixelDataImage(BMPFormat::Bitmap &struct_object);

        std::vector <BMPReader::H_BRAA::Bytes> getPixelDataImage_8bf (BMPFormat::Bitmap &struct_object);
        std::vector <BMPReader::H_BRAA::Bytes> getPixelDataImage_16bf(BMPFormat::Bitmap &struct_object);
        std::vector <BMPReader::H_BRAA::Bytes> getPixelDataImage_24bf(BMPFormat::Bitmap &struct_object);
        std::vector <BMPReader::H_BRAA::Bytes> getPixelDataImage_32bf(BMPFormat::Bitmap &struct_object);
   private:
      BMPFormat::Bitmap& fill_FILE_header (BMPFormat::Bitmap &struct_object, const std::vector <BMPReader::H_BRAA::Bytes> &data_from_bmp_file);

      BMPFormat::Bitmap& fill_DIB_header        (BMPFormat::Bitmap &struct_object, const std::vector <BMPReader::H_BRAA::Bytes> &data_from_bmp_file);
      BMPFormat::Bitmap& fill_DIB_CORE_header   (BMPFormat::Bitmap &struct_object, const std::vector <BMPReader::H_BRAA::Bytes> &data_from_bmp_file);
      BMPFormat::Bitmap& fill_DIB_INFO_header   (BMPFormat::Bitmap &struct_object, const std::vector <BMPReader::H_BRAA::Bytes> &data_from_bmp_file);
      BMPFormat::Bitmap& fill_DIB_V4_header     (BMPFormat::Bitmap &struct_object, const std::vector <BMPReader::H_BRAA::Bytes> &data_from_bmp_file);
      BMPFormat::Bitmap& fill_DIB_V5_header     (BMPFormat::Bitmap &struct_object, const std::vector <BMPReader::H_BRAA::Bytes> &data_from_bmp_file);

      BMPFormat::Bitmap& fillBitmapStruct (BMPFormat::Bitmap &struct_object, const std::vector <BMPReader::H_BRAA::Bytes> &data_from_bmp_file);


   public:  
      BitmapReader(const std::string &input_file_name) : path_to_file(input_file_name) { }  /* Only with parametre = const std::string &input_file_name: */
      ~BitmapReader() { closeBMP(); }

   public:
      const bool  openBMP();     /* Open  .bmp File:                                   */
      const bool  closeBMP();    /* Close .bmp File !DESTRUCTOR CALL THIS METHOD!:     */

      std::vector < std::vector <unsigned long> > GETTER_PIXEL_DATA1 (std::vector <BMPReader::H_BRAA::Bytes> pixel_data);

      std::vector < std::vector <unsigned long> > GETTER_PIXEL_DATA2(std::vector < std::vector <unsigned long> > temp_vector);

   public:
      BMPFormat::Bitmap& readAndFillBitmapStruct(BMPFormat::Bitmap &struct_object);
   };
//--------------------------------------------------------------------------------------------------------------
};



#endif /* BMPREADER_HPP! */
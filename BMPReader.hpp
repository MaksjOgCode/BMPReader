#ifndef BMPREADER_HPP
#define BMPREADER_HPP



#include <vector>
#include <string>
#include <fstream>


#include "Hints.hpp"
#include "BMPFormat.hpp"



namespace BMPReader
{
//--------------------------------------------------------------------------------------------------------------
   class BitmapReader;

   class BRAA
   {
   private:
      friend class BitmapReader;

      struct Bytes
      {
         std::vector <unsigned long> set_of_bytes;
      };

   private:
      /* Full Static class: */
      BRAA () = delete;
      BRAA (const BitmapReaderAlgorithmsAPI&) = delete;
      BRAA &operator=(const BitmapReaderAlgorithmsAPI&) = delete;

   private:
      static const unsigned long long getResultValueFromFile (Bytes &set_of_bytes);

      static inline void ConvertToBMPData (  Bytes                              &set_of_bytes_object,
                                             std::vector <BRAA::Bytes>          &set_of_bytes_file_header,
                                             const std::vector <unsigned char>  &bytes_buffer  );
   };
//--------------------------------------------------------------------------------------------------------------
   class BitmapReader
   {
   private:
      friend class BMPReader::BRAA;

      /* Only with parametre = const std::string &input_file_name: */
      BitmapReader() = delete;
      BitmapReader (const BitmapReaderAlgorithmsAPI&) = delete;
      BitmapReader &operator=(const BitmapReaderAlgorithmsAPI&) = delete;

   private:
      std::string    path_to_file {};  /* The field that contains the path to the file */
      std::ifstream  bmp_file;         /* Input Image .bmp:                            */

      std::vector <BMPReader::BRAA::Bytes>  data_from_bmp_file  {};    /* Vector that contains the read data from the file:  */

   public:  
      BitmapReader(const std::string &input_file_name) : path_to_file(input_file_name) { }  /* Only with parametre = const std::string &input_file_name: */
      ~BitmapReader() { closeBMP(); }

   public:
      const bool  openBMP();     /* Open  .bmp File:                                   */
      const bool  closeBMP();    /* Close .bmp File !DESTRUCTOR CALL THIS METHOD!:    */

      bool  fillBMPStruct(BMPFormat::BitmapStruct &struct_object);

      std::vector <BMPReader::BRAA::Bytes>  readBMPHeaderFile();  /* Read Only BMP Header File: */
      std::vector <BMPReader::BRAA::Bytes>  readBMPDIBHeader();   /* Read Bitmap DIB Header:     */
   };
//--------------------------------------------------------------------------------------------------------------
};



#endif /* BMPREADER_HPP! */
#ifndef BMPREADER_HPP
#define BMPREADER_HPP



#include "BMPFormat.hpp"



#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <stdexcept>



//------------------------------------------------------------------------------------------------------------------------------------------------------
namespace BMPReader
{
	class HelperRefactorXD;
	class Reader;

	/* ************************************************************ */
	class H_BRAA
	{
	private:
		friend class HelperRefactorXD;
	private:
		friend class Reader;
		friend class Reader_FILE_Header;
		friend class Reader_DIB_Header;
		friend class Reader_Data_Pixel;

		friend class Filler;
	private:
		struct Bytes { std::vector <uint8_t> byte_vector {}; };
	private:
		static inline void ConvertReadedData (HelperRefactorXD &_helper_refactor_xd );
		static inline const uint64_t convertToOneByteFromFile(const std::vector <uint8_t> &set_of_bytes);
	};
	/* ************************************************************ */
	class HelperRefactorXD
	{
	private:
		friend class H_BRAA;
		friend class Reader_FILE_Header;
		friend class Reader_DIB_Header;
		friend class Reader_Data_Pixel;

	private:
		/* Only with input param _sizeof_CURRENT_header: */
		HelperRefactorXD() = delete;
		HelperRefactorXD(const HelperRefactorXD&) = delete;
		HelperRefactorXD& operator=(const HelperRefactorXD&) = delete;
	private:
		HelperRefactorXD(const uint64_t& _sizeof_CURRENT_header);
	private:
		H_BRAA::Bytes bytes                      {};
		std::vector <H_BRAA::Bytes> set_of_bytes {};
	private:
		uint64_t sizeof_CURRENT_header                     {};
		std::vector <uint8_t> sizeof_types_CURRENT_header  {};
	private:
		std::vector <uint8_t> bytes_buffer                 {};
	};
	/* ************************************************************ */
	class Filler
	{
	private:
		friend class Reader;
	private:
		Filler() = delete;
		Filler(const Filler&) = delete;
		Filler& operator=(const Filler&) = delete;
	private:
		static inline void fillBMP_FILE_Header(Reader &reader_object, BMPFormat::Bitmap &bitmap_struct_object);
		static inline void fillBMP_DIB_CURRENT_Header(Reader &reader_object, BMPFormat::Bitmap &bitmap_struct_object);
	private:
		static inline void fillBMP_DIB_CORE_Header(Reader &reader_object, BMPFormat::Bitmap &bitmap_struct_object);
		static inline void fillBMP_DIB_INFO_Header(Reader &reader_object, BMPFormat::Bitmap &bitmap_struct_object);
		static inline void fillBMP_DIB_V4_Header(Reader &reader_object, BMPFormat::Bitmap &bitmap_struct_object);
		static inline void fillBMP_DIB_V5_Header(Reader &reader_object, BMPFormat::Bitmap &bitmap_struct_object);
	private:
		static inline void fillBMP_PIXEL_DATA(Reader &reader_object, BMPFormat::Bitmap &bitmap_struct_object);
	};
	/* ************************************************************ */
	class Reader
	{
	private:
		friend class Reader_FILE_Header;
		friend class Reader_DIB_Header;
		friend class Reader_Data_Pixel;
		friend class Filler;
	public:
		Reader  (const std::string &_input_path_to_file);
		~Reader () { closeBMP(); }
	public:
      const bool openBMP();
		const bool closeBMP();
	private:
		/* Only with parametre = const std::string &input_path_to_file: */
		Reader () = delete;
		Reader (const Reader&) = delete;
		Reader &operator=(const Reader&) = delete;
	private:
		std::string    input_path_to_file {};
		std::ifstream  input_bmp_file;
	private:
		std::vector <H_BRAA::Bytes> readed_FILE_DIB_headers {};
		std::vector <H_BRAA::Bytes> readed_PIXEL_DATA       {};
	public:
		void readAndFillBMPStruct(BMPFormat::Bitmap &bitmap_struct_object);
	};
	/* ************************************************************ */
	/* A fully static class: */
	class Reader_FILE_Header
	{
	private:
		friend class Reader;
	private:
		Reader_FILE_Header() = delete;
		Reader_FILE_Header(const Reader_FILE_Header&) = delete;
		Reader_FILE_Header& operator=(const Reader_FILE_Header&) = delete;
	private:
		static inline std::vector <H_BRAA::Bytes> readBMP_FILE_Header(Reader &reader_object);
	};
	/* ************************************************************ */
	/* A fully static class: */
	class Reader_DIB_Header
	{
	private:
		friend class Reader;
	private:
		Reader_DIB_Header() = delete;
		Reader_DIB_Header(const Reader_DIB_Header&) = delete;
		Reader_DIB_Header& operator=(const Reader_DIB_Header&) = delete;
	private:
		static inline std::vector <H_BRAA::Bytes> readBMP_DIB_Header(Reader &reader_object);
	private:
		static inline std::vector <H_BRAA::Bytes> readBMP_DIB_CURRENT_Header (Reader &reader_object, const uint8_t  &flag_version_dib);
	};
	/* ************************************************************ */
	class Reader_Data_Pixel
	{
	private:
		friend class Reader;
	private:
		Reader_Data_Pixel() = delete;
		Reader_Data_Pixel(const Reader_Data_Pixel&) = delete;
		Reader_Data_Pixel& operator=(const Reader_Data_Pixel&) = delete;
	private:
		static inline std::vector <BMPReader::H_BRAA::Bytes> readBMP_PIXEL_DATA (Reader &reader_object, std::vector <BMPReader::H_BRAA::Bytes> &readed_DIB_Header);
	};
};
//------------------------------------------------------------------------------------------------------------------------------------------------------



#endif /* BMPREADER_HPP! */
#pragma once
#include <fstream>
#include <string.h>
#include <memory.h>
#include "MLDFile.h"

#include "error_statuses.h"
using namespace std;

//enum FILE_STATUS {
//	FILE_SUCCESSFUL = 0,
//	FILE_NOT_FOUND = 1,
//	FILE_MAGIC_ERROR = 2,
//	FILE_VERSION_ERROR = 3,
//	FILE_NUM_LAYERS_ERROR = 4,
//	FILE_ERROR = 5
//};
//
//enum LAYER_STATUS {
//	LAYER_SUCCESSFUL = 0,
//	INVALID_LAYER_INDEX = 1,
//	INVALID_OBJECT_INDEX = 2,
//	LAYER_NAME_ERROR = 3,
//	LAYER_IN_IMG_COORDINATES_ERROR = 4,
//	LAYER_OBJECT_READING_ERROR = 5,
//	LAYER_ERROR = 6
//};
//
//enum OBJECT_STATUS {
//	OBJECT_SUCCESSFUL = 0,
//	OBJECT_SHAPE_READING_ERROR = 1,
//	OBJECT_ERROR = 2
//};
//
//enum SHAPE_STATUS {
//	SHAPE_SUCCESSFUL = 0,
//	INVALID_SHAPE_ID = 1,
//	SHAPE_POLYGON_ERROR = 2,
//	SHAPE_ELLIPSE_ERROR = 3,
//	SHAPE_CIRCLE_ERROR = 4,
//	SHAPE_POLYLINE_ERROR = 5,
//	SHAPE_LINE_ERROR = 6,
//	SHAPE_RECTANGLE_ERROR = 7,
//	SHAPE_SQUARE_ERROR = 8,
//	SHAPE_TEXT_ERROR = 9,
//	SHAPE_ERROR = 10
//};
//
//enum READING_STATUS {
//	SUCCESSFUL_READING = 0,
//	ERROR_READING = 1
//};

namespace mld {
	class MLDReader
	{

	private:
		string m_path;
		unique_ptr<MLDFile> m_mldfile;
		FILE* m_f;

		unsigned char* m_mld_file_buffer;
		bool m_read_from_file;
		unsigned int m_buffer_count;

	public:
		///
		/// ... Default constructor for given class ...
		///
		MLDReader();
		///
		/// ... Constructor that creates class object according to the given parameters ...
		///
		//MLDReader(string path);

		///
		/// ... Function that reads mld file that is stored in the given mld_filepath ...
		/// ... Returns READING_STATUS::READING_SUCCESSFUL if file is successfully read 
		///     and if not returns READING_STATUS with appropriate error ...
		///
		STATUS ReadMld(const char* mld_filepath, bool read_from_file);
		///
		/// ... Function that read layer with given index ...
		/// ... Returns LAYER_STATUS::LAYER_SUCCESSFUL if layer is successfully read 
		///     and if not returns LAYER_STATUS with appropriate error ...
		///
		STATUS ReadLayer(unsigned int layer_index);
		///
		/// ... Function that reads the object eith given index in given layer ...
		/// ... Returns OBJECT_STATUS::OBJECT_SUCCESSFUL if object is successfully read 
		///     and if not returns OBJECT_STATUS with appropriate error ...
		///
		STATUS ReadObject(unsigned int layer_index, unsigned int obj_index, unsigned int& buff_size);
		///
		/// ... Function that reads shape of the object with given index in given layer ...
		/// ... Returns SHAPE_STATUS::SHAPE_SUCCESSFUL if shape is successfully read 
		///     and if not returns SHAPE_STATUS with appropriate error ...
		///
		STATUS ReadShape(unsigned int layer_index, unsigned int obj_index, unsigned int& buff_size);
		///
		/// ... Function that reads concrete shape of the object with given index in given layer and its shape_id and type ...
		/// ... Returns SHAPE_STATUS::SHAPE_SUCCESSFUL if concrete shape is successfully read 
		///     and if not returns SHAPE_STATUS with appropriate error ...
		///
		STATUS ReadConcreteShape(unsigned int layer_index, unsigned int obj_index, int8_t shape_id, int8_t type, unsigned& buff_size);
		///
		/// ... Function that reads string and additional info from file .....
		///	
		void ReadTextAndAdditional(string& text, string& additional, unsigned int& buff_size);
		///
		/// ... Function that reads Points from file based on passed number of points and stores them in given points vector ...
		///
		void ReadPointsVector(unsigned int num_of_points, vector<Point>& points_vector, unsigned int& buff_size);

		///
		/// ... Function that reads polygon of the object with given index in given layer and its shape_id and type ...
		/// ... Returns SHAPE_STATUS::SHAPE_SUCCESSFUL if polygon is successfully read 
		///     and if not returns SHAPE_STATUS with appropriate error ...
		///
		STATUS ReadPolygon(unsigned int layer_index, unsigned int obj_index, int8_t shape_id, int8_t type, unsigned int& buff_size);
		///
		/// ... Function that reads ellipse of the object with given index in given layer and its shape_id and type ...
		/// ... Returns SHAPE_STATUS::SHAPE_SUCCESSFUL if ellipse is successfully read 
		///     and if not returns SHAPE_STATUS with appropriate error ...
		///
		STATUS ReadEllipse(unsigned int layer_index, unsigned int obj_index, int8_t shape_id, int8_t type, unsigned int& buff_size);
		///
		/// ... Function that reads circle of the object with given index in given layer and its shape_id and type ...
		/// ... Returns SHAPE_STATUS::SHAPE_SUCCESSFUL if circle is successfully read 
		///     and if not returns SHAPE_STATUS with appropriate error ...
		///
		STATUS ReadCircle(unsigned int layer_index, unsigned int obj_index, int8_t shape_id, int8_t type, unsigned int& buff_size);
		///
		/// ... Function that reads polyline of the object with given index in given layer and its shape_id and type ...
		/// ... Returns SHAPE_STATUS::SHAPE_SUCCESSFUL if polyline is successfully read 
		///     and if not returns SHAPE_STATUS with appropriate error ...
		///
		STATUS ReadPolyLine(unsigned int layer_index, unsigned int obj_index, int8_t shape_id, int8_t type, unsigned int& buff_size);
		///
		/// ... Function that reads line of the object with given index in given layer and its shape_id and type ...
		/// ... Returns SHAPE_STATUS::SHAPE_SUCCESSFUL if line is successfully read 
		///     and if not returns SHAPE_STATUS with appropriate error ...
		///	
		STATUS ReadLine(unsigned int layer_index, unsigned int obj_index, int8_t shape_id, int8_t type, unsigned int& buff_size);
		///
		/// ... Function that reads rectangle of the object with given index in given layer and its shape_id and type ...
		/// ... Returns SHAPE_STATUS::SHAPE_SUCCESSFUL if rectangle is successfully read 
		///     and if not returns SHAPE_STATUS with appropriate error ...
		///	
		STATUS ReadRectangle(unsigned int layer_index, unsigned int obj_index, int8_t shape_id, int8_t type, unsigned int& buff_size);
		///
		/// ... Function that reads square of the object with given index in given layer and its shape_id and type ...
		/// ... Returns SHAPE_STATUS::SHAPE_SUCCESSFUL if square is successfully read 
		///     and if not returns SHAPE_STATUS with appropriate error ...
		///
		STATUS ReadSquare(unsigned int layer_index, unsigned int obj_index, int8_t shape_id, int8_t type, unsigned int& buff_size);
		///
		/// ... Function that reads text of the object with given index in given layer and its shape_id and type ...
		/// ... Returns SHAPE_STATUS::SHAPE_SUCCESSFUL if text is successfully read 
		///     and if not returns SHAPE_STATUS with appropriate error ...
		///	
		STATUS ReadText(unsigned int layer_index, unsigned int obj_index, int8_t shape_id, int8_t type, unsigned int& buff_size);

		///
		/// ... Function that starts reading mld file and its basic info that is stored in the given mld_filepath ...
		/// ... Returns FILE_STATUS::FILE_SUCCESSFUL if reading is successfully started 
		///     and if not returns FILE_STATUS with appropriate error ...
		///
		STATUS OpenMLDFile(const char* mld_filepath);
		///
		/// ... Function that return number of layer for mld file reader is created for ...
		///	
		int GetNumOfLayers();
		///
		/// ... Function that return number of objects in a layer with given index for mld file reader is created for ...
		///		
		int GetNumOfObjectsInLayer(unsigned int layer_index);
		///
		/// ... Function that stores information about object with given object index in given layer to the passed object reference ...
		/// ... Returns LAYER_STATUS::LAYER_SUCCESSFUL if informations are successfully copie 
		///     and if not returns LAYER_STATUS with appropriate error ...
		///
		STATUS GetObjectFromLayer(Object& obj, unsigned int layer_index, unsigned int object_index);

		void CopyCharacters(unsigned char* dest_buffer, unsigned char* source_buffer, unsigned int byte_count);

		STATUS LoadToBuffer(FILE* f);

		STATUS ReadMldFileAttributes();


		MLDFile& GetMld();
	};
}

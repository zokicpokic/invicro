#pragma once
#include "Layer.h"
#include <vector>

#include <fstream>

#define MAGIC 4
#define L 3
using namespace std;

namespace mld {
	class MLDFile
	{
	private:
		char m_magic[MAGIC];
		unsigned int m_version;
		unsigned int m_layers_number;
		unsigned int m_current_number;
		vector<unique_ptr<Layer>> m_layer_array;
		MLDFile();
		FILE* f;

	public:

		///
		/// ... Constructor that creates class object according to the given parameters ...
		///
		MLDFile(char magic[], unsigned int version, unsigned int layers_number);

		///
		/// ... Copy constructor for the given class ...
		///
		MLDFile(const MLDFile& mldf);

		///
		/// ... Setter for the magic attribute ...
		///
		void SetMagic(char magic[]);
		///
		/// ... Getter for the magic attribute ...
		///
		char* GetMagic();

		///
		/// ... Setter for the version attribute ...
		///
		void SetVersion(unsigned int version);
		///
		/// ... Getter for the version attribute ...
		///
		unsigned int GetVersion();

		///
		/// ... Setter for the layers number attribute ...
		///
		void SetLayersNumber(unsigned int layers_number);
		///
		/// ... Getter for the layers number attribute ...
		///
		unsigned int GetLayersNumber();

		void SetReadFromFile(bool read_from_file);
		bool GetReadFromFile();

		///
		/// ... TODO: Switch to logging into file ...
		///
		void Print();

		///
		/// ... Function that returns object stored in the given layer with given object index ...
		///
		Object& GetObject(unsigned int layer_index, unsigned int object_index);
		///
		/// ... Function that returns number of objects in given layer ...
		///
		unsigned int GetNumOfObjectsInLayer(unsigned int layer_index);

		///
		/// ... Function that stores an object in the layer with given index ...
		///
		void PushLayerObject(unsigned int layer_index);
		///
		/// ... Function that stores layer with given parameters in layer vector ...
		///
		void PushLayer(char name[], bool in_img_coordinates, unsigned int objects_number, vector<unsigned int>& buffer_size_vector);



		///
		/// ... Returns a reference to layer object for a given layer index...
		///	
		Layer& GetLayer(unsigned int layer_index);

		STATUS ConvertMMToPixels(unsigned int layer_index, unsigned int lower_index, unsigned int upper_index);
		STATUS ConvertPixelsToMM(unsigned int layer_index, unsigned int lower_index, unsigned int upper_index);

		///
		/// ... Writes mld_file attributes and calls the next writing function ...
		///	
		STATUS WriteMld();
		//CHECK what are possible writing errors


		///
		/// ... opens mld file for writing(if file does not exist on given path, it creates one) and calls function to initialize writing ...
		///	
		void Write(char* file_path);

		///
		/// ... Destructor for the given class ...
		///	
		virtual ~MLDFile();

	};
}

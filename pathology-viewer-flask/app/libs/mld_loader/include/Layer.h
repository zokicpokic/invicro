#pragma once
#include "Object.h"
#include <vector>
#define N 64

namespace mld {
	class Layer
	{
	private:
		char m_name[N];
		bool m_in_img_coordinates;
		unsigned int m_objects_number;
		unsigned int m_objects_counter;
		vector<unsigned int> m_buffer_size_vector;

		bool m_changed_in_img_coords;
		Layer();
		vector<unique_ptr<Object>> m_objects_vector;

	public:
		///
		/// ... Constructor that creates class object according to the given parameters ...
		///
		Layer(char name[], bool in_img_coordinates, unsigned int object_number, vector<unsigned int>& buffer_size_vector);

		///
		/// ... Copy constructor for the given class ...
		///
		Layer(const Layer& layer);
		///
		/// ... Overloaded assignment operator for the given class ...
		///
		void operator=(const Layer& layer);

		///
		/// ... Setter for the name attribute ...
		///
		void SetName(char name[]);
		///
		/// ... Getter for the name attribute ...
		///
		char* GetName();

		///
		/// ... Setter for the boolean attribute in image coordinates ...
		///
		void SetInImgCoordinates(bool in_img_coordinates);
		///
		/// ... Getter for the boolean attribute in image coordinates ...
		///
		bool GetInImgCoordinates();

		///
		/// ... Setter for the number of objects attribute ...
		///
		void SetObjectNumber(unsigned int objects_umber);
		///
		/// ... Getter for the number of objects attribute ...
		///
		unsigned int GetObjectNumber();


		unsigned int& GetBufferSize();

		vector<unsigned int>& GetBuffers();

		///
		/// ... Function that stores given object into the vector of objects ...
		///
		void AddObject(Object& o);

		///
		/// ... TODO: switch to logging into file ...
		///
		void Print();

		///
		/// ... Function that returns object stored on the given index in vector of objects  ...
		///
		Object& GetObject(unsigned int object_index);

		///
		/// ... Function that stores object created with deafult constructor into the vector of objects ...
		///
		void PushObject();

		///
		/// ... Writes layer attributes in a given file ...
		///	
		STATUS WriteLayer(FILE* f);


		vector<Object>& GetObjects();
		STATUS ConvertMMToPixels(unsigned int lower_index, unsigned int upper_index);
		STATUS ConvertPixelsToMM(unsigned int lower_index, unsigned int upper_index);

		void AddBufferSize(unsigned int buffer_size);


		///
		/// ... Destructor for the given class ...
		///
		virtual ~Layer();
	};
}

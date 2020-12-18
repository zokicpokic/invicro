#include "../include/Layer.h"

using namespace std;

namespace mld
{
	Layer::Layer(char name[], bool in_img_coordinates, unsigned int objects_number, vector<unsigned int> &buffer_size_vector)
	{
		strcpy(m_name, name);
		m_in_img_coordinates = in_img_coordinates;
		m_objects_number = objects_number;
		m_objects_counter = 0;
		m_buffer_size_vector.reserve(1);
		m_buffer_size_vector = buffer_size_vector;
		m_objects_vector.reserve(m_objects_number);
		m_changed_in_img_coords = false;
	}

	Layer::Layer(const Layer &layer)
	{
		strcpy(m_name, layer.m_name);
		m_in_img_coordinates = layer.m_in_img_coordinates;
		m_objects_number = layer.m_objects_number;

		m_changed_in_img_coords = layer.m_changed_in_img_coords;
		for (unsigned int i = 0; i < layer.m_buffer_size_vector.size(); i++)
		{
			m_buffer_size_vector.push_back(layer.m_buffer_size_vector[i]);
		}

		for (unsigned int i = 0; i < layer.m_objects_vector.size(); i++)
		{
			Object &o = *layer.m_objects_vector[i];
			m_objects_vector.push_back(make_unique<Object>(o));
		}
	}

	void Layer::operator=(const Layer &layer)
	{
		strcpy(m_name, layer.m_name);
		m_in_img_coordinates = layer.m_in_img_coordinates;
		m_objects_number = layer.m_objects_number;

		m_changed_in_img_coords = layer.m_changed_in_img_coords;
		for (unsigned int i = 0; i < layer.m_buffer_size_vector.size(); i++)
		{
			m_buffer_size_vector[i] = layer.m_buffer_size_vector[i];
		}

		for (unsigned int i = 0; i < layer.m_objects_vector.size(); i++)
		{
			Object &o = *layer.m_objects_vector[i];
			m_objects_vector.push_back(make_unique<Object>(o));
		}
	}

	void Layer::SetName(char name[])
	{
		strcpy(m_name, name);
	}
	char *Layer::GetName()
	{
		return m_name;
	}

	void Layer::SetInImgCoordinates(bool in_img_coordinates)
	{
		m_in_img_coordinates = in_img_coordinates;
	}
	bool Layer::GetInImgCoordinates()
	{
		return m_in_img_coordinates;
	}

	void Layer::SetObjectNumber(unsigned int objects_number)
	{
		m_objects_number = objects_number;
	}
	unsigned int Layer::GetObjectNumber()
	{
		return m_objects_number;
	}

	unsigned int &Layer::GetBufferSize()
	{
		return *m_buffer_size_vector.end();
	}

	vector<unsigned int> &Layer::GetBuffers()
	{
		return m_buffer_size_vector;
	}

	void Layer::AddObject(Object &o)
	{
		m_objects_vector.push_back(make_unique<Object>(o));
		m_objects_number++;
	}

	void Layer::Print()
	{
		cout << "Name: " << m_name << endl;
		cout << "InImgCoordinates: " << m_in_img_coordinates << endl;
		cout << "Object number: " << m_objects_number << endl;

		for (unsigned int i = 0; i < m_objects_vector.size(); i++)
			m_objects_vector[i]->Print();
	}

	Object &Layer::GetObject(unsigned int object_index)
	{
		return *m_objects_vector[object_index];
	}

	void Layer::PushObject()
	{
		m_objects_vector.push_back(make_unique<Object>(Object()));
	}

	STATUS Layer::WriteLayer(FILE *f)
	{
		//check layer name
		fwrite(m_name, sizeof(m_name), 1, f);

		fwrite(&m_in_img_coordinates, sizeof(bool), 1, f);
		fwrite(&m_objects_number, sizeof(unsigned int), 1, f);

		//TODO!!!!

		//fwrite(&m_buffer_size, sizeof(int), 1, f);

		for (unsigned int i = 0; i < m_objects_vector.size(); i++)
		{
			STATUS status = m_objects_vector[i]->WriteObject(f);
			if (status != SUCCESSFULL)
				return status;
		}

		return SUCCESSFULL;
	}

	STATUS Layer::ConvertMMToPixels(unsigned int lower_index, unsigned int upper_index)
	{
		if (lower_index >= 0 && upper_index < m_objects_vector.size())
		{
			for (unsigned int i = lower_index; i < upper_index; i++)
			{
				m_objects_vector[i]->ConvertMMToPixels();
			}
			m_changed_in_img_coords = !m_changed_in_img_coords;
			m_in_img_coordinates = !m_in_img_coordinates;
			return SUCCESSFULL;
		}
		else
			return INVALID_BOUNDARIES_FOR_CONVERSION;
	}

	//CHECK if needed for mld file - for all layers
	STATUS Layer::ConvertPixelsToMM(unsigned int lower_index, unsigned int upper_index)
	{
		if (lower_index >= 0 && upper_index < m_objects_vector.size())
		{
			for (unsigned int i = lower_index; i < upper_index; i++)
			{
				m_objects_vector[i]->ConvertPixelsToMM();
			}
			m_changed_in_img_coords = !m_changed_in_img_coords;
			m_in_img_coordinates = !m_in_img_coordinates;
			return SUCCESSFULL;
		}
		else
			return INVALID_BOUNDARIES_FOR_CONVERSION;
	}

	void Layer::AddBufferSize(unsigned int buffer_size)
	{
		m_buffer_size_vector.push_back(buffer_size);
	}

	Layer::~Layer()
	{
	}
} // namespace mld

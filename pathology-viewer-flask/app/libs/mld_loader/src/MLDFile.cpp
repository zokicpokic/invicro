#include "../include/MLDFile.h"

namespace mld
{
	MLDFile::MLDFile()
	{
	}

	MLDFile::MLDFile(char magic[], unsigned int version, unsigned int layers_number)
	{
		strcpy(m_magic, magic);
		m_version = version;
		m_layers_number = layers_number;
		m_current_number = 0;
		//_layerArray.reserve(layers_number);
	}

	MLDFile::MLDFile(const MLDFile &mldf)
	{
		strcpy(m_magic, mldf.m_magic);
		m_version = mldf.m_version;
		m_layers_number = mldf.m_layers_number;
		m_current_number = mldf.m_current_number;

		for (unsigned int i = 0; i < mldf.m_layer_array.size(); i++)
		{
			//Layer l(mldf.m_layer_array[i]->GetName(), mldf.m_layer_array[i]->GetInImgCoordinates(), mldf.m_layer_array[i]->GetObjectNumber(), mldf.m_layer_array[i]->GetBufferSize());

			Layer &lay = *mldf.m_layer_array[i];
			m_layer_array.push_back(make_unique<Layer>(lay));
		}
	}

	void MLDFile::SetMagic(char magic[])
	{
		strcpy(m_magic, magic);
	}
	char *MLDFile::GetMagic()
	{
		return m_magic;
	}

	void MLDFile::SetVersion(unsigned int version)
	{
		m_version = version;
	}
	unsigned int MLDFile::GetVersion()
	{
		return m_version;
	}

	void MLDFile::SetLayersNumber(unsigned int layers_number)
	{
		m_layers_number = layers_number;
	}
	unsigned int MLDFile::GetLayersNumber()
	{
		return m_layers_number;
	}

	void MLDFile::Print()
	{
		cout << "Magic: ";
		for (unsigned int i = 0; i < MAGIC; i++)
			cout << m_magic[i];
		cout << endl
			 << "Version: " << m_version << endl;
		cout << "Layer number: " << m_layers_number << endl;

		for (unsigned int i = 0; i < m_layer_array.size(); i++)
			m_layer_array[i]->Print();
	}
	Object &MLDFile::GetObject(unsigned int layer_index, unsigned int object_index)
	{
		return m_layer_array[layer_index]->GetObject(object_index);
	}

	unsigned int MLDFile::GetNumOfObjectsInLayer(unsigned int layer_index)
	{
		return m_layer_array[layer_index]->GetObjectNumber();
	}

	void MLDFile::PushLayerObject(unsigned int layer_index)
	{
		m_layer_array[layer_index]->PushObject();
	}

	void MLDFile::PushLayer(char name[], bool inImgCoord, unsigned int objectsNumber, vector<unsigned int> &buffer_size_vector)
	{
		m_layer_array.push_back(make_unique<Layer>(Layer(name, inImgCoord, objectsNumber, buffer_size_vector)));
		m_current_number++;
	}

	MLDFile::~MLDFile()
	{
	}

	Layer &MLDFile::GetLayer(unsigned int layer_index)
	{
		return *m_layer_array[layer_index];
	}

	STATUS MLDFile::ConvertMMToPixels(unsigned int layer_index, unsigned int lower_index, unsigned int upper_index)
	{
		return m_layer_array[layer_index]->ConvertMMToPixels(lower_index, upper_index);
	}

	STATUS MLDFile::ConvertPixelsToMM(unsigned int layer_index, unsigned int lower_index, unsigned int upper_index)
	{
		return m_layer_array[layer_index]->ConvertPixelsToMM(lower_index, upper_index);
	}

	STATUS MLDFile::WriteMld()
	{
		if (m_magic[0] != 'L' || m_magic[1] != 'D' || m_magic[2] != 'F' || m_magic[3] != 'F')
			return INVALID_MLD_MAGIC;
		fwrite(m_magic, sizeof(m_magic), 1, f);

		if (m_version != 3)
			return INVALID_MLD_VERSION;
		fwrite(&m_version, sizeof(int), 1, f);

		if (m_layers_number != 3)
			return INVALID_MLD_NUMBER_OF_LAYERS;
		fwrite(&m_layers_number, sizeof(int), 1, f);

		for (unsigned int i = 0; i < m_layer_array.size(); i++)
		{
			STATUS status = m_layer_array[i]->WriteLayer(f);
			if (status != SUCCESSFULL)
				return status;
		}
		return SUCCESSFULL;
	}

	void MLDFile::Write(char *file_path)
	{
		f = fopen(file_path, "wb");

		WriteMld();

		fclose(f);
	}

} // namespace mld

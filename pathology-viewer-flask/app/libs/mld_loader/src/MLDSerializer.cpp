#include "../include/MLDSerializer.h"

namespace mld
{

	MLDSerializer::MLDSerializer()
	{
		m_json = "";
	}

	string MLDSerializer::SerializeMLD(MLDFile &mldfile, vector<int> layer_index, vector<int> object_index, double translation_x, double translation_y, double scale)
	{
		m_json = "{\"type\":\"FeatureCollection\",";
		m_json += "\"features\":[";

		for (int i = 0; i < layer_index.size(); i++)
		{
			m_json += mldfile.GetObject(layer_index[i], object_index[i]).ToJSON(translation_x, translation_y, scale);
			if (i < layer_index.size() - 1)
				m_json += ",";
		}

		m_json += "]}";
		return m_json;
	}

	MLDSerializer::~MLDSerializer()
	{
	}
} // namespace mld
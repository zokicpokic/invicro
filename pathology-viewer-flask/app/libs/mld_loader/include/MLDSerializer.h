#pragma once
#include "MLDFile.h"
#include <string>
using namespace std;

namespace mld {
	class MLDSerializer
	{
	private:
		string m_json;

	public:
		MLDSerializer();
		string SerializeMLD(MLDFile& mldfile, vector<int> layer_index, vector<int> object_index, double translation_x, double translation_y, double scale);

		virtual ~MLDSerializer();
	};
}

#include "../include/mld_loader.h"

using namespace std;
using namespace mld;

int load_whole_mld(string& data, string& info, string& filename_img, string& filename_mld)
{
	struct stat buff;
	if (stat(filename_img.c_str(), &buff)) {
		info = "File does not exist: " + filename_img;
		return -1;
	}

	if (!openslide_detect_vendor(filename_img.c_str())) {
		info = "File '" + filename_img + "' is corrupt.";
		return -1;
	}

	openslide_t* big_image = openslide_open(filename_img.c_str());
	if (!big_image) {
		info = openslide_get_error(big_image);
		openslide_close(big_image);
		return -1;
	}
	
	double translation_x, translation_y, scale;
	translation_x = atof(openslide_get_property_value(big_image, "aperio.LineAreaXOffset"));
	translation_y = atof(openslide_get_property_value(big_image, "aperio.LineAreaYOffset"));
	scale = atof(openslide_get_property_value(big_image, "aperio.MPP"));	
	
	MLDReader reader;
	STATUS reading_status = reader.ReadMld(filename_mld.c_str(), false);
	if (reading_status != STATUS::SUCCESSFULL) {
		info = "STATUS:" + to_string(reading_status);
		return -1;
	}

	vector<int> layer_index;
	vector<int> object_index;
	MLDFile mld = reader.GetMld();
	for (int l = 0; l < mld.GetLayersNumber(); l++) {
		for (int o = 0; o < mld.GetLayer(l).GetObjectNumber(); o++) {
			layer_index.push_back(l);
			object_index.push_back(o);
		}
	}

	MLDSerializer serializer;
	data = serializer.SerializeMLD(mld, layer_index, object_index, -translation_x, -translation_y, 1/scale);
	return 0;
}

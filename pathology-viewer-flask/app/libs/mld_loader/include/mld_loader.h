#pragma once

#include <stdio.h>
#include <fstream>
#include <string>
#include <sys/stat.h>	
#include <math.h>

#include "openslide/openslide.h"

#include "MLDSerializer.h"
#include "MLDReader.h"

int load_whole_mld(string& data, string& info, string& filename_img, string& filename_mld);

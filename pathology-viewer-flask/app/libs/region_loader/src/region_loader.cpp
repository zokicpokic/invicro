#include "../include/region_loader.h"
#include <fstream>

#include <openslide/openslide.h>
#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>

int load_thumbnail(unsigned int* data, string& info, string& filename) {
    int64_t* width = new int64_t;
    int64_t* height = new int64_t;

    struct stat buff;
    if (stat(filename.c_str(), &buff)) {
        info = "File does not exist: " + filename;
        delete width, height;
        return -1;
    }
    
    if (!openslide_detect_vendor(filename.c_str())) {
        info = "File is corrupt.";
        delete width, height;
        return -1;
    }

    openslide_t* big_image = openslide_open(filename.c_str());
    if (!big_image) {
        info = openslide_get_error(big_image);
        openslide_close(big_image);
        delete width, height;
        return -1;
    }
    
    const char* const* names = openslide_get_associated_image_names(big_image);
    if (names[2] == NULL) // this associated image should be a thumbnail (check if this is always the case)
    {
        info = "Missing thumbnail.";
        openslide_close(big_image);
        delete width, height;
        return -1;
    }
    
    openslide_get_associated_image_dimensions(big_image, names[2], width, height);
    if (*width == -1 || *height == -1) {
        info = openslide_get_error(big_image);
        openslide_close(big_image);
        delete width, height;
        return -1;
    }
    
    //uint32_t* thumb_data = new uint32_t[(*width) * (*height)];   
    char* thumb_data = new char[4 * (*width) * (*height)];
    openslide_read_associated_image(big_image, names[2], (uint32_t*)thumb_data);
    memcpy(data, thumb_data, 4*(*width) * (*height));
    
    delete[] thumb_data;
    delete width, height;
    openslide_close(big_image); 
    return 0;  
}

int get_thumbnail_dimensions(int64_t* width, int64_t* height, string& info, string& filename) {
    struct stat buff;
    if (stat(filename.c_str(), &buff)) {
        info = "File does not exist: " + filename;
        return -1;
    }
    
    if (!openslide_detect_vendor(filename.c_str())) {
        info = "File is corrupt.";
        return -1;
    }

    openslide_t* big_image = openslide_open(filename.c_str());
    if (!big_image) {
        info = openslide_get_error(big_image);
        openslide_close(big_image);
        return -1;
    }
    
    const char* const* names = openslide_get_associated_image_names(big_image);
    if (names[2] == NULL)
    {
        info = "Missing thumbnail.";
        openslide_close(big_image);
        delete width, height;
        return -1;
    } 
    
    openslide_get_associated_image_dimensions(big_image, names[2], width, height);
    if (*width == -1 || *height == -1) {
        info = openslide_get_error(big_image);
        openslide_close(big_image);
        return -1;
    }
    
    openslide_close(big_image);
    return 0;
}

int load_region(unsigned int* data, string& info, string& filename, int tile, int level, int x, int y) {
    int32_t critical_level = 0, max_level = 0, prev_level, ratio = 1, real_downsample = 1;
    int64_t w = tile, h = tile, max_w, max_h;
    int64_t* width = new int64_t;
    int64_t* height = new int64_t;
    double downsample;
    string name = "l" + to_string(level) + "_x" + to_string(x) + "_y" + to_string(y) + ".png";

    struct stat buff;
    if (stat(filename.c_str(), &buff)) {
        info = "File does not exist: " + filename;
        delete width, height;
        return -1;
    }

    if (!openslide_detect_vendor(filename.c_str())) {
        info = "File is corrupt.";
        delete width, height;
        return -1;
    }

    openslide_t* big_image = openslide_open(filename.c_str());
    if (!big_image) {
        info = openslide_get_error(big_image);
        openslide_close(big_image);
        delete width, height;
        return -1;
    }

    openslide_get_level_dimensions(big_image, 0, width, height);
    if (*width == -1 || *height == -1) {
        info = openslide_get_error(big_image);
        openslide_close(big_image);
        delete width, height;
        return -1;
    }

    int dim_max = *width > * height ? *width : *height;
    int dim_min = *width < *height ? *width : *height;
    while (dim_max > tile) {
        dim_min /= 2;
        dim_max /= 2;
        if (dim_min > tile)
            critical_level++;
        max_level++;
    }

    max_w = *width;
    max_h = *height;

    if (level > max_level || level < 0) {
        info = "Zoom out of range.";
        delete width, height;
        return -1;
    }

    // if level 0 is smallest
    level = max_level - level;

    for (int i = 0; i < level; i++)
        real_downsample *= 2;

    int x_num = ceil((double)(*width) / (tile * real_downsample)) + floor((max_level - level) / critical_level);
    int y_num = ceil((double)(*height) / (tile * real_downsample)) + floor((max_level - level) / critical_level);

    if (x >= x_num || x < 0 || y >= y_num || y < 0) {
        info = "Tile out of range. Max values: " + to_string(x_num - 1) + ", " + to_string(y_num - 1);
        delete width, height;
        return -1;
    }

    // TODO: comment this
    // data = new char[4 * tile * tile];

    // if (sizeof(data) / sizeof(int) != 4*tile*tile) {
    //     info = "Memory allocated only for " + to_string(sizeof(data) / sizeof(int)) + " elements.";
    //     delete width, height;
    //     return -1;
    // }

    prev_level = openslide_get_best_level_for_downsample(big_image, real_downsample + EPSI);
    if (prev_level == -1) {
        info = openslide_get_error(big_image);
        openslide_close(big_image);
        delete width, height;
        return -1;
    }

    openslide_get_level_dimensions(big_image, prev_level, width, height);
    if (*width == -1 || *height == -1) {
        info = openslide_get_error(big_image);
        openslide_close(big_image);
        delete width, height;
        return -1;
    }

    downsample = openslide_get_level_downsample(big_image, prev_level);
    if (downsample == -1) {
        info = openslide_get_error(big_image);
        openslide_close(big_image);
        delete width, height;
        return -1;
    }

    // char* data = new char[4 * tile * tile];

    ratio = ceil(real_downsample / downsample);
    w *= ratio;
    h *= ratio;

    if (x == x_num - 1)
        w = *width - x * tile * ratio;

    if (y == y_num - 1)
        h = *height - y * tile * ratio;

    x *= tile * real_downsample;
    y *= tile * real_downsample;

    info = "x=" + to_string(x) + ", y=" + to_string(y) + ", max_width=" + to_string(max_w) + ", max_height=" + to_string(max_h);
    info += ", big_width=" + to_string(*width) + ", big_height=" + to_string(*height) + ", w=" + to_string(w) + ", h=" + to_string(h);
    info += ", max_level=" + to_string(max_level) + ", critical_level=" + to_string(critical_level) + ", prev_level=" + to_string(prev_level);
    info += ", ratio=" + to_string(ratio) + ", downsample=" + to_string(downsample) + ", real_downsample=" + to_string(real_downsample);
    info += ", x_num=" + to_string(x_num) + ", y_num=" + to_string(y_num);

    if (ratio > 1) {
        char* big_data = new char[4 * w * h];
        openslide_read_region(big_image, (uint32_t*)big_data, x, y, prev_level, w, h);

        cv::Mat image(cv::Size(w, h), CV_8UC4, big_data);
        cv::Mat d_image;
        if (w != tile * ratio || h != tile * ratio) {
            try {
                cv::resize(image, d_image, cv::Size(ceil(w / ratio), ceil(h / ratio)), 0, 0, cv::INTER_AREA);
                cv::Mat tmp(cv::Size(tile, tile), CV_8UC4, cv::Scalar(0));
                d_image.copyTo(tmp(cv::Rect(0, 0, d_image.cols, d_image.rows)));
                memcpy(data, tmp.data, 4 * tile * tile);

                if (SAVE)
                    cv::imwrite(name, tmp);
            }
            catch (cv::Exception e) {
                info = "OPENCV: " + e.msg;
                delete[] big_data;
                delete width, height;
                openslide_close(big_image);
                return -1;
            }
        }
        else {
            try {
                cv::resize(image, d_image, cv::Size(ceil(w / ratio), ceil(h / ratio)), 0, 0, cv::INTER_AREA);
                memcpy(data, d_image.data, 4 * ceil(w / ratio) * ceil(h / ratio));

                if (SAVE)
                    cv::imwrite(name, d_image);
            }
            catch (cv::Exception e) {
                info = "OPENCV: " + e.msg;
                delete[] big_data;
                delete width, height;
                openslide_close(big_image);
                return -1;
            }
        }
        delete[] big_data;
    }
    else {
        if (w != tile || h != tile) {
            char* big_data = new char[4 * w * h];
            try {
                openslide_read_region(big_image, (uint32_t*)big_data, x, y, prev_level, w, h);
                cv::Mat image(cv::Size(w, h), CV_8UC4, big_data);

                cv::Mat tmp(cv::Size(tile, tile), CV_8UC4, cv::Scalar(0));
                image.copyTo(tmp(cv::Rect(0, 0, image.cols, image.rows)));
                memcpy(data, tmp.data, 4 * tile * tile);
                delete[] big_data;

                info += "TSET";

                if (SAVE)
                    cv::imwrite(name, tmp);
            }
            catch (cv::Exception e) {
                info = "OPENCV: " + e.msg;
                delete[] big_data;
                delete width, height;
                openslide_close(big_image);
                return -1;
            }
        }
        else {
            openslide_read_region(big_image, (uint32_t*)data, x, y, prev_level, w, h);
            // if (SAVE) {
            // cv::Mat image(cv::Size(w, h), CV_8UC4, data);
            // cv::imwrite(name, image);
            // cv::imwrite("cpp.png", image);
            info = "---cpp---";
            // }
            std::fstream myfile;
            myfile = std::fstream("file.binary", std::ios::out | std::ios::binary);
            //Here would be some error handling
                //Some calculations to fill a[]
            myfile.write((char*)data,w*h*sizeof(uint32_t));
            myfile.close();
        }
    }

    delete width, height;
    openslide_close(big_image);
    return 0;
}

int get_dimensions(string& data, string& info, string& filename) {
    struct stat buff;
    if (stat(filename.c_str(), &buff)) {
        info = "File does not exist: " + filename;
        return -1;
    }

    if (!openslide_detect_vendor(filename.c_str())) {
        info = "File is corrupt.";
        return -1;
    }

    openslide_t* big_image = openslide_open(filename.c_str());
    if (!big_image) {
        info = openslide_get_error(big_image);
        openslide_close(big_image);
        return -1;
    }

    int64_t w, h;
    openslide_get_level_dimensions(big_image, 0, &w, &h);
    if (w == -1 || h == -1) {
        info = openslide_get_error(big_image);
        openslide_close(big_image);
        return -1;
    }

    data = "{\"width\":" + to_string(w) + ",\"height\":" + to_string(h) + "}";
    return 0;
}

int get_value(int64_t* arg) {
    return *arg;
}

unsigned int* alloc_memory(string& info, int width, int height) {
    unsigned int* data = new unsigned int[width*height];
    info = to_string(sizeof(data));
    return data;
}

int64_t* alloc_memory_dim(string& info) {
    int64_t* data = new int64_t;
    info = to_string(sizeof(data));
    return data;
}

unsigned int* alloc_memory_v2(string& info, int64_t* width, int64_t* height) {
    unsigned int* data = new unsigned int[(*width)*(*height)];
    info = to_string(sizeof(data));
    return data;
}

int free_memory(unsigned int* data) {
    try {
        delete[] data;
        return 0;
    } catch(exception e) {
        return -1;
    }
}

int free_memory_dim(int64_t* data) {
    try {
        delete data;
        return 0;
    } catch(exception e) {
        return -1;
    }
}

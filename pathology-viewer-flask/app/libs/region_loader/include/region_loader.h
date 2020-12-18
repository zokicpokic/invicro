#pragma once

#define EPSI 0.01
#define SAVE 0

#include <iostream>
#include <stdint.h>
#include <stdio.h>
#include <string>
#include <sys/stat.h>
#include <vector>
#include <exception>
#include <math.h>

using namespace std;

int load_thumbnail(unsigned int* data, string& info, string& filename);
int load_region(unsigned int* data, string& info, string& filename, int tile, int level, int x, int y);
int get_thumbnail_dimensions(int64_t* width, int64_t* height, string& info, string& filename);
int get_dimensions(string& data, string& info, string& filename);
int get_value(int64_t* arg);
unsigned int* alloc_memory(string& info, int width, int height);
unsigned int* alloc_memory_v2(string& info, int64_t* width, int64_t* height);
int64_t* alloc_memory_dim(string& info);
int free_memory(unsigned int* data);
int free_memory_dim(int64_t* data);

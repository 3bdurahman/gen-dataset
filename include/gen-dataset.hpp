#ifndef GEN
#define GEN
#include <iostream>
#include <cstring>
#include <cstdint>
#include <fstream>
#include <fmt/ostream.h>
#include <thread>
#include <chrono>
#include <cctype>
#include <stdfloat>
#include <math.h>
#include <fmt/ostream.h>
#include <fmt/core.h>
#include <fmt/format.h>
#include <filesystem>
#include <vector>
#include <cmath>
#include <list>

using namespace std;

 
const char* count_check(const char* str, bool floating = false);
const char* size_check(const char* str_size);
size_t get_size(const double& size, const char* c_size);
void generate(const char* branch, const char* count, const char* depth, const char* file_size, const char* buffer, const char* threads, const char* prompt, const char* sub);
void progress_bar(const size_t& count);
void name_check(const char* name);
void create_file_directories(size_t count_dirs, size_t count_files, vector<char>* size, vector<char>* buffer, vector<string>* directories, const char* subdir);
void create_tree(vector<string>* directories, const size_t& depth, const size_t& branch, const char* path);
#endif
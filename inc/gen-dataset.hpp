#include <thread>
#include <iostream>
#include <string>
#include <vector>
#include <fmt/core.h>
#include <cmath>
#include <list>
#include <fstream>
#include <filesystem>
#include <fstream>
#include <mutex>
using namespace std;

void create_depth(const unsigned& branch, const unsigned& depth, const char* path, vector<string>& folders_id);
void is_digit(const char* argv, const unsigned& length, bool point = false);
size_t get_size(const char* argv);
void generate(const char* branch, const char* files_count, const char* depth, const char* c_size, const char* c_buffer, const char* count_threads, const char* prompt, const char* subdir);
void create_files(const unsigned& start, const unsigned& end, const char* path, vector<char>& size, vector<char>& buffer);
void progress_bar(const unsigned& count);
void name_check(const char *name);
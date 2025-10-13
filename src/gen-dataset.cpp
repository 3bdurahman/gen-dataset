#include "gen-dataset.hpp"

void create_depth(const unsigned& branch, const unsigned& depth, const char* path, vector<string>& folders_id) {
  if (!depth) return;
  for (unsigned i = 1; i <= branch; ++i) {
    string temp_path = fmt::format("{}/Sub{}", path, i);
    filesystem::create_directory(temp_path.c_str());
    if (depth == 1) folders_id.push_back(temp_path);
    create_depth(branch, depth - 1, temp_path.c_str(), folders_id);
  }
  
}

void is_digit(const char* argv, const unsigned& length, bool point) {
  if (!argv) 
    throw runtime_error("Argument is null");
  for (unsigned i = 0; i < length; ++i) {
    if (*(argv + i) < '0' || *(argv + i) > '9') {
      if (point && *(argv + i) == '.') {
        point = false;
        continue;
      }
      throw runtime_error("Should the argument value is digit.");
    }
  }
}

size_t get_size(const char* argv) {
  if (!argv) return 0;
  unsigned length = strlen(argv);
  is_digit(argv, length - 3, true);
  unsigned len = 4;
  char start[len] = {*(argv + length - 3), *(argv + length - 2), *(argv + length - 1), '\0'};
  for (unsigned i = 0; i < len; ++i)
    *(start + i) = static_cast<char>(tolower(*(start + i)));
  const char* types[len] = {"kib", "mib", "gib", "tib"};
  for (unsigned i = 0; i < len; ++i) {
    if (!strcmp(start, *(types + i))) break;
    if (i == len - 1 && strcmp(start, *(types + i))) throw runtime_error("This size type not support.");
  }
  char c_size[length - 2];
  c_size[length - 3] = '\0';
  for (unsigned i = 0; i < length - 3; ++i) *(c_size + i) = *(argv + i);
  double d_size = stod(c_size);
  size_t size = static_cast<size_t>(d_size);
  d_size -= size;
  if (!strcmp(start, "kib")) {
    const size_t kib = 1024;
    size *= kib;
    d_size *= kib;
  }
  else if (!strcmp(start, "mib")) {
    const size_t mib = 1024ULL * 1024;
    size *= mib;
    d_size *= mib;
  }
  else if (!strcmp(start, "gib")) {
    const size_t gib = 1024Ull * 1024 * 1024;
    size *= gib;
    d_size *= gib;
  }
  else {
    const size_t tib = 1024ULL * 1024 * 1024 * 1024;
    size *= tib;
    d_size *= tib;
  }
  size += d_size;
  return size;
}

void generate(const char *c_branch, const char *c_files, const char *c_depth, const char *c_size, const char *c_buffer, const char *c_threads, const char *prompt, const char *subdir) {
  unsigned branch = 0, files_count = 0, depth = 0, threads_count = 0;
  if (c_branch) {
    is_digit(c_branch, strlen(c_branch));
    branch = stoul(c_branch);
  }
  if (c_files) {
    is_digit(c_files, strlen(c_files));
    files_count = stoul(c_files);
  }
  if (c_depth) {
    is_digit(c_depth, strlen(c_depth));
    depth = stoul(c_depth);
  }
  if (c_threads) {
    is_digit(c_threads, strlen(c_threads));
    threads_count = stoul(c_threads);
  }
  unsigned total_dirs = (!depth ? 0 : static_cast<unsigned>(pow(branch, depth)));
  if (subdir) name_check(subdir);
  vector<char> size(get_size(c_size), 'A');
  vector<char> buffer(get_size(c_buffer), 'A');
  if (!prompt || strcmp(prompt, "-y")) {
    size_t Total_Dirs = 0;
    for (unsigned i = 1; i <= depth; ++i)
      Total_Dirs += static_cast<size_t>(pow(branch, i));
    printf("Create %u directories and %u files in %S? [y/n] ", ((Total_Dirs && !subdir) ? Total_Dirs : (subdir && !Total_Dirs) ? 1 : 0), files_count, filesystem::current_path().c_str());
    char c;
    scanf("%c", &c);
    if (c != 'y') return;
  }
  vector<string> folders_id;
  progress_bar(files_count);
  if (threads_count) {
    list<thread> threads;
    unsigned base_files = 0;
    unsigned remain_files = 0;
    if (branch && depth) {
      total_dirs = (total_dirs > files_count ? files_count : total_dirs);
      base_files = files_count / total_dirs;
      remain_files = files_count % total_dirs;
      unsigned base_dirs = total_dirs / threads_count;
      unsigned remain_dirs = total_dirs % threads_count;
      create_depth(branch, depth, ".", folders_id);
      unsigned index = 0;
      for (unsigned i = 0; i < threads_count; ++i) {
        threads.push_back(thread([i, &index, &remain_dirs, &base_dirs, &threads_count, &branch, &depth, &folders_id, &remain_files, &base_files, &size, &buffer](){
          for (unsigned j = 0; j < base_dirs + (i < remain_dirs ? 1 : 0); ++j)
            create_files(0, base_files + (index <= remain_files ? 1 : 0), folders_id.at(index++).c_str(), size, buffer);
        }));
      }
    }
    else if (subdir && !branch && !depth) {
      base_files = files_count / threads_count;
      remain_files = files_count % threads_count;
      unsigned start = 0;
      filesystem::create_directory(subdir);
      for (unsigned i = 0; i < threads_count; ++i) {
        unsigned end = start + base_files + (i < remain_files ? 1 : 0);
        threads.push_back(thread([start, end, i, &subdir, &threads_count, &branch, &depth, &folders_id, &remain_files, &base_files, &size, &buffer](){
            create_files(start, end , subdir, size, buffer);
        }));
        start = end;
      }
    }
    else {
      base_files = files_count / threads_count;
      remain_files = files_count % threads_count;
      unsigned start = 0;
      for (unsigned i = 0; i < threads_count; ++i) {
        unsigned end = start + base_files + (i < remain_files ? 1 : 0);
        threads.push_back(thread([start, end, i, &subdir, &threads_count, &branch, &depth, &folders_id, &remain_files, &base_files, &size, &buffer](){
            create_files(start, end , ".", size, buffer);
        }));
        start = end;
      }
    }
    for (thread& t : threads) t.join();
  }
  else {
    if (branch && depth || subdir) {
      if (branch && depth) {
        unsigned base_files = files_count / total_dirs;
        unsigned remain_files = files_count % total_dirs;
        create_depth(branch, depth, ".", folders_id);
        for (unsigned i = 0; i < total_dirs; ++i)
          create_files(0, base_files + (i < remain_files ? 1 : 0), folders_id.at(i % folders_id.size()).c_str(), size, buffer);
      }
      else if (subdir && !branch && !depth) {
        filesystem::create_directory(subdir);
        create_files(0, files_count , subdir, size, buffer);
      }
    }
    else
      create_files(0, files_count , ".", size, buffer);
  }
}

void create_files(const unsigned& start, const unsigned& end, const char* path, vector<char>& size, vector<char>& buffer) {
  if (!path) throw runtime_error("The folder path is nullptr.");
  ofstream file;
  for (unsigned i = start; i < end; ++i) {
    file.open(fmt::format("{}/dataset{}.img", path, i + 1), ios::binary);
    if (file.is_open()) {
      if (size.size() && buffer.size())
        for (size_t j = 0; j < size.size(); j += buffer.size()) {
          file.write(buffer.data(), buffer.size());
          file.flush();
        }
    }
    else throw runtime_error("Cant write in file because cant open file.");
    file.flush();
    file.close();
  }
}

void name_check(const char *name) {
  if (name)
    for (unsigned i = 0; i < strlen(name); ++i)
      if (ispunct(*(name + i))) throw logic_error(fmt::format("Folder name correct value.").c_str());
}

void progress_bar(const unsigned& total, const unsigned& width) {
  for (unsigned current = 1; current <= total; ++current) {
    if (current > total) current = total;
    double ratio = static_cast<double>(current) / total;
    unsigned filled = static_cast<unsigned>(ratio * width);

    printf("\r");  
    printf("Files created %u/%u\n", current, total);
    printf("[");
    if (filled > 0) {
      if (filled < width) {
        for (unsigned i = 0; i < filled - 1; ++i) putchar('=');
        putchar('>');
      }
      else for (unsigned i = 0; i < filled; ++i) putchar('=');
    }
    for (unsigned i = filled; i < width; ++i) putchar(' ');
    printf("]");
    fflush(stdout);
    printf("\033[F");
  }
  printf("\n\n");
}
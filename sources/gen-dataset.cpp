#include "gen-dataset.hpp"

const char* count_check(const char* str, bool floating) {
  if (!str) return "0";
  size_t length = strlen(str);
  for (size_t i = 0; i < length; ++i) {
    if (floating && *(str + i) == '.') {
      floating = false;
      continue;
    }
    else if (!isdigit(*(str + i))) throw logic_error("Not digit.");
  }
  return str;
}

const char* size_check(const char* str) {
  if (str) {
    const unsigned len = 5;
    const unsigned len_2 = strlen(str);
    const char s[len] = "tgmk";
    for (size_t i = 0; i < len_2; ++i) {
      if (i != len_2 - 1 && !isdigit(*(str + i))) throw logic_error("Size number.");
      else {
        for (unsigned j = 0; j < len - 1; ++j)
          if (*(s + j) == tolower(*(str + i))) return str;
      }
    }
    throw logic_error("Size type.");
  }
  else return "0";
}

size_t get_size(const double& size, const char* c_size) {
  size_t res = size;
  if (size && c_size) {
    float remain = size - res;
    const size_t len = strlen(c_size);
    size_t types[4] = {1024, 1024 * 1024 , 1024UL * 1024 * 1024, 1024ULL * 1024 * 1024 * 1024};
    const char c = tolower(*(c_size + len - 1));
    for (int i = 0; i < 2; ++i) {
      if (c == 'k') (i ? remain *= *types : res *= *types);
      else if (c == 'm') (i ? remain *= *(types + 1) : res *= *(types + 1));
      else if (c == 'g') (i ? remain *= *(types + 2) : res *= *(types + 2));
      else (i ? remain *= *(types + 3) : res *= *(types + 3));
    }
    res += remain;
  }
  return res;
}

void generate(const char* branch, const char* count, const char* depth, const char* file_size, const char* buffer_size , const char* threads, const char* prompt, const char* sub) {
  if (!count) throw runtime_error("Count empty!.\nuse --help");
  //argv
  size_t Count = stoull(count_check(count));
  size_t Threads = stoull(count_check(threads));
  size_t Depth = stoull(count_check(depth));
  size_t Branch = stoull(count_check(branch));
  size_t Total_Dirs = static_cast<size_t>(pow(Branch, depth ? Depth : 1));
  name_check(sub);
  if (sub && Total_Dirs) throw logic_error("use --help");
  
  //size & buffer
  const size_t Size = get_size(stod(size_check(file_size)), file_size);
  const size_t Buffer = get_size(stod(size_check(buffer_size)), (buffer_size));
  vector<char> *size = (Size ? new vector<char>(Size, 'A') : nullptr);
  vector<char> *buffer = (Buffer ? new vector<char>(Buffer, 'A') : nullptr);

  if (!prompt) {
    bool skip = true;
    string p = filesystem::current_path().string();
    for (size_t i = 0; i < p.size(); ++i) {
      if (skip && p.at(i) == '\\') skip = false;
      else if (!skip && p.at(i) == '\\') {
        p.resize(i);
        break;
      }
    }
    printf(fmt::format("Create {} directories and {} files in {}? [y/n] ", ((Total_Dirs && !sub) ? Total_Dirs : (sub && !Total_Dirs) ? 1 : 0), count, p).c_str());
    char c;
    scanf("%c", &c);
    if (c != 'y') return;
  }
  //progress_bar(Count);
  
  vector<string> *directories = (Branch ? new vector<string> : nullptr);
  
  create_tree(directories, Depth, Branch, "..");
  
  if (sub) filesystem::create_directory(sub);
  list<thread> *workers = (threads ? new list<thread> : nullptr);
   
  if (Threads && Total_Dirs) {
    const size_t base_dir = Total_Dirs / Threads;
    const size_t base_file = Count / Total_Dirs;
    const size_t remain_dirs = Total_Dirs % Threads;
    size_t remain_files = Count % Total_Dirs;
    bool check = false;
    if (remain_files > Threads) {
      remain_files -= Threads;
      check = true;
    }
    for (size_t i = 0; i < Threads; ++i) {
      workers->push_back(thread(create_file_directories, 
        base_dir + (i < remain_dirs ? 1 : 0), 
        base_file + ((check ? i > remain_files : i < remain_files) ? 1 : 0), 
        ref(size), ref(buffer), ref(directories), ref(sub)));
    }
  }
  else if (Threads && !Total_Dirs && !sub) {
    const size_t base_file = Count / Threads;
    const size_t remain_files = Count % Threads;
    size_t start = 0;
    for (size_t i = 0; i < Threads; ++i) {
      size_t end = start + base_file + (i < remain_files ? 1 : 0);
      workers->push_back(thread(create_file_directories,
      start, end, ref(size), ref(buffer), ref(directories), ref(sub)));
      start = end;
    }
  }
  else if (Threads && sub && !Total_Dirs) {
    size_t base_file = Count / Threads;
    size_t remain_file = Count % Threads;
    size_t start = 0;
    for (size_t i = 0; i < Threads; ++i) {
      size_t end = start + base_file + (i < remain_file ? 1 : 0);
      workers->push_back(thread(create_file_directories, 
        start, end, ref(size), ref(buffer), ref(directories), ref(sub)));
      start = end;
    }
  }
  else if (Total_Dirs && !Threads) {
    const size_t base_file = Count / Total_Dirs;
    const size_t remain_file = Count % Total_Dirs;
    for (size_t i = 0; i < Total_Dirs; ++i)
      create_file_directories(1, base_file + (i < remain_file ? 1 : 0), size, buffer, directories, sub);
  }
  else if (!Threads && sub && !Total_Dirs) {
    size_t start = 0;
    for (size_t i = 0; i < Count; ++i) {
      size_t end = start + 1;
      create_file_directories(start, end, size, buffer, directories, sub);
      start = end;
    }
  }
  if (Threads && workers) for (thread& t : *workers) t.join();
}

void progress_bar(const size_t& count) {
  printf("\n");
  for (size_t i = 0; i <= count; ++i) {
    float progress = float(i) / count;
    int barWidth = 50;
    int pos = int(barWidth * progress);

    printf("\033[F");
    printf("\rFiles created: %i/%i          \n", i, count);

    printf("[");
    for (int j = 0; j < barWidth; ++j) {
        if (j < pos) printf("=");
        else if (j == pos) printf(">");
        else printf(" ");
    }
    printf("]");
    fflush(stdout);
  }
}

void name_check(const char *name) {
  if (name) {
    for (unsigned i = 0; i < strlen(name); ++i)
      if (ispunct(*(name + i))) throw logic_error(fmt::format("\"{}\" Folder name correct format.", name).c_str());
  }
}

void create_tree(vector<string>* directories, const size_t& depth, const size_t& branch, const char* path) {
  if (!directories || !depth) return;
  for (size_t i = 0; i < branch; ++i) {
    string Path = fmt::format("{}/Sub{}", path, i + 1);
    if (depth == 1) directories->push_back(Path);
    filesystem::create_directory(Path);
    create_tree(directories, depth - 1, branch, Path.c_str());
  }
}

void create_file_directories(size_t count_dirs, size_t count_files, vector<char>* size, vector<char>* buffer, vector<string>* directories, const char* subdir) {
  if (directories && !subdir) {
    static size_t x = 0;
    static bool start = false;
    for (size_t i = 0; i < count_dirs; ++i) {
      ofstream file;
      if (start) ++x;
      for (size_t j = 0; j < count_files; ++j) {
        file.open((fmt::format("{}/dataset{}.bin", directories->at(x), j + 1)), ios::binary);
        if (size && buffer && file.is_open()) {
          for (size_t z = buffer->size(); z <= size->size(); z += buffer->size())
            file.write(buffer->data(), buffer->size());
        }
        file.close();
      }
      start = true;
    }
  }
  else if (!subdir && !directories){
    ofstream file;
    for (size_t i = count_dirs; i < count_files; ++i) {
      file.open(fmt::format("./dataset{}.bin", i + 1), ios::binary);
      if (size && buffer && file.is_open()) {
        for (size_t z = buffer->size(); z <= size->size(); z += buffer->size()){
          file.write(buffer->data(), buffer->size());
          file.flush();
        }
      }
      file.close();
    }
  }
  else {
    for (size_t i = count_dirs; i < count_files; ++i) {
      ofstream file;
      file.open(fmt::format("{}/dataset{}.bin", subdir, i + 1), ios::binary);
      if (size && buffer && file.is_open()) {
        for (size_t z = buffer->size(); z <= size->size(); z += buffer->size())
          file.write(buffer->data(), buffer->size());
      }
      file.close();
    }
  }
}

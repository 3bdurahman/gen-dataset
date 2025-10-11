#include "gen-dataset.hpp"

//gen -b 5 -c 100 -d 5 -s 1m -S 5m -t 5 -y -> 14
int main(int argc, char* argv[]) {
  try {
    const char* branch = nullptr;
    const char* count = nullptr;
    const char* depth = nullptr;
    const char* size = nullptr;
    const char* buffer = nullptr;
    const char* threads = nullptr;
    const char* prompt = nullptr;
    const char* subdir = nullptr;
    if (argc == 2 && !strcmp(*(argv + 1), "--help")) {
      printf("usage:\n"
  "gen-dataset  -c [-b -d -s -S -t -w -y] [path]\n"

"flags:\n"
  "-b, --branches <int>              - number of subdirectories per directory\n"
  "-c, --count <int>                 - total number of files to create\n"
  "-d, --depth <int>                 - number of directory levels\n"
  "-s, --size <float[K..T][i]B>      - file size\n"
  "-S, --buff-size <float[K..T][i]B> - write buffer size (default=1M)\n"
  "-t, --threads <int>               - number of parallel file creation threads\n"
  "-y, --yes                         - don't prompt before creating files\n");
  fflush(stdout);
    }
    else if (argc > 2) {
      for (unsigned i = 1; i < argc; i += 2) {
        if (!strcmp(*(argv + i), "-b") && argc > i + 1) {
          branch = *(argv + i + 1);
          if (argc > i + 2 && *(*(argv + i + 2)) != '-'){
            if (*(*(argv + i + 3)) == '-') throw logic_error("Usage.");
            subdir = *(argv + i + 2);
          }
        }
        else if (!strcmp(*(argv + i), "-c") && argc > i + 1) {
          count = *(argv + i + 1);
          if (argc > i + 2 && *(*(argv + i + 2)) != '-'){
            if (*(*(argv + i + 3)) == '-') throw logic_error("Usage.");
            subdir = *(argv + i + 2);
          }
        }
        else if (!strcmp(*(argv + i), "-d") && argc > i + 1) {
          depth = *(argv + i + 1);
          if (argc > i + 2 && *(*(argv + i + 2)) != '-'){
            if (*(*(argv + i + 3)) == '-') throw logic_error("Usage.");
            subdir = *(argv + i + 2);
          }
        }
        else if (!strcmp(*(argv + i), "-s") && argc > i + 1) {
          size = *(argv + i + 1);
          if (argc > i + 2 && *(*(argv + i + 2)) != '-'){
            if (*(*(argv + i + 3)) == '-') throw logic_error("Usage.");
            subdir = *(argv + i + 2);
          }
        }
        else if (!strcmp(*(argv + i), "-S") && argc > i + 1) {
          buffer = *(argv + i + 1);
          if (argc > i + 2 && *(*(argv + i + 2)) != '-'){
            if (*(*(argv + i + 3)) == '-') throw logic_error("Usage.");
            subdir = *(argv + i + 2);
          }
        }
        else if (!strcmp(*(argv + i), "-t") && argc > i + 1) {
          threads = *(argv + i + 1);
          if (argc > i + 2 && *(*(argv + i + 2)) != '-'){
            if (*(*(argv + i + 3)) == '-') throw logic_error("Usage.");
            subdir = *(argv + i + 2);
          }
        }
        else if (!strcmp(*(argv + i), "-y")) {
          prompt = *(argv + i);
          if (argc > i + 2 && *(*(argv + i + 2)) != '-'){
            if (*(*(argv + i + 3)) == '-') throw logic_error("Usage.");
            subdir = *(argv + i + 2);
          }
        }
      }
      buffer = ((size && !buffer) ? "1M" : nullptr);
      depth = (branch && !depth ? "1" : depth);
      if (branch && subdir) {
        printf("use --help");
        return 1;
      }
      generate(branch, count, depth, size, buffer, threads, prompt, subdir);
    }
    else printf("use --help");
  } catch (const exception& err) {
    printf("Error!, %s", err.what());
  }
  return 0;
}
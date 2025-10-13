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
      for (int i = 1; i < argc; ++i) {
        const char* a = argv[i];
        if (!strcmp(a, "-b") || !strcmp(a, "--branches")) {
          if (i + 1 < argc && *(*(argv + i + 1)) != '-') branch = argv[++i];
          else throw logic_error("Missing value for -b");
        }
        else if (!strcmp(a, "-c") || !strcmp(a, "--count")) {
          if (i + 1 < argc && *(*(argv + i + 1)) != '-') count = argv[++i];
          else throw logic_error("Missing value for -c");
        }
        else if (!strcmp(a, "-d") || !strcmp(a, "--depth")) {
          if (i + 1 < argc && *(*(argv + i + 1)) != '-') depth = argv[++i];
          else throw logic_error("Missing value for -d");
        }
        else if (!strcmp(a, "-s") || !strcmp(a, "--size")) {
          if (i + 1 < argc && *(*(argv + i + 1)) != '-') size = argv[++i];
          else throw logic_error("Missing value for -s");
        }
        else if (!strcmp(a, "-S") || !strcmp(a, "--buff-size")) {
          if (i + 1 < argc && *(*(argv + i + 1)) != '-') buffer = argv[++i];
          else throw logic_error("Missing value for -S");
        }
        else if (!strcmp(a, "-t") || !strcmp(a, "--threads")) {
          if (i + 1 < argc && *(*(argv + i + 1)) != '-') threads = argv[++i];
          else throw logic_error("Missing value for -t");
        }
        else if (!strcmp(a, "-y") || !strcmp(a, "-Y")) prompt = "y";
        else if (*a == '-') throw logic_error(string("Unknown flag: ") + a);
        else subdir = a;
      }
    }
    else {
      printf("use --help");
      return 1;
    }
    buffer = ((size && !buffer) ? "1MiB" : buffer);
    depth = ((!depth && branch) ? "1" : depth);
    generate(branch, count, depth, size, buffer, threads, prompt, subdir);
  } catch (const exception& err) {
    printf("Error!, %s", err.what());
  }
  return 0;
}
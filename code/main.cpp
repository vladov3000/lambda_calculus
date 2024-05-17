#include <errno.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>
#include "basic.hpp"
#include "print.hpp"
#include "arena.hpp"
#include "syntax.hpp"
#include "lexer.hpp"
#include "parse.hpp"
#include "evaluate.hpp"

static String read_file(char* path) {
  int fd = open(path, O_RDONLY);
  if (fd == -1) {
    print(ERROR, "Failed to open \"", path, "\": ", strerror(errno), ".\n");
    exit(EXIT_FAILURE);
  }

  struct stat info;
  if (fstat(fd, &info) == -1) {
    print(ERROR, "Failed to stat \"", path, "\": ", strerror(errno), ".\n");
    exit(EXIT_FAILURE);
  }

  String file;
  file.size = info.st_size;
  file.data = static_cast<U8*>(mmap(NULL, file.size, PROT_READ, MAP_PRIVATE, fd, 0));
  if (file.data == MAP_FAILED) {
    print(ERROR, "Failed to mmap \"", path, "\": ", strerror(errno), ".\n");
    exit(EXIT_FAILURE);
  }
  return file;
}

I32 main(I32 argc, char** argv) {
  atexit(flush);

  if (argc != 2) {
    print(ERROR, "Expected exactly 1 argument.\n");
    print("Usage: lambdac PROGRAM\n");
    exit(EXIT_FAILURE);
  }

  String input = read_file(argv[1]);

  Arena arena = make_arena(1ull << 32);

  Lexer lexer;
  lexer.input = input;

  Environment* environment = parse_environment(&arena, &lexer);
  Term*        main        = lookup(environment, "main");
  if (main == NULL) {
    print(ERROR, "main is not defined.\n");
    exit(EXIT_FAILURE);
  }

  print("Evaluating: ", main, '\n');
  Term* result = evaluate(&arena, main, environment);
  print("Result:     ", result, '\n');
}

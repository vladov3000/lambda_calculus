#define ERROR "\x1b[31;1mError\x1b[0m "

static U8  print_buffer[4096];
static I64 print_buffered;

static void flush() {
  if (print_buffered > 0) {
    write(STDOUT_FILENO, print_buffer, print_buffered);
    print_buffered = 0;
  }
}

static void print(String message) {
  if (message.size > sizeof print_buffer) {
    flush();
    write(STDOUT_FILENO, message.data, message.size);
  } else {
    if (message.size > sizeof print_buffer - print_buffered) {
      flush();
    }
    memcpy(&print_buffer[print_buffered], message.data, message.size);
    print_buffered += message.size;
  }
}

static void print(char c) {
  if (print_buffered == sizeof print_buffer) {
    flush();
  }
  print_buffer[print_buffered++] = c;
}

static void print(auto&& first, auto&& second, auto&&... rest) {
  print(first);
  print(second);
  (print(rest), ...);
}

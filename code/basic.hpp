#define length(array) (sizeof(array) / sizeof((array)[0]))

typedef unsigned char U8;
typedef unsigned long U64;
typedef int           I32;
typedef long          I64;

struct String {
  U8* data = nullptr;
  I64 size = 0;

  String() {}

  String(const char* string) {
    data = reinterpret_cast<U8*>(const_cast<char*>(string));
    size = strlen(string);
  }

  String(U8* data, I64 size) {
    this->data = data;
    this->size = size;
  }

  U8& operator[](I64 i) {
    return data[i];
  }

  String slice(I64 start, I64 size) {
    return String(&data[start], size);
  }

  U8* begin() {
    return data;
  }

  U8* end() {
    return &data[size];
  }

  operator bool() {
    return size > 0;
  }

  bool operator==(String other) {
    return size == other.size && memcmp(data, other.data, size) == 0;
  }
};

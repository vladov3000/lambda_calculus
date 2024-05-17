struct Arena {
  U8* memory    = nullptr;
  U64 used      = 0;
  U64 committed = 0;
  U64 size      = 0;
};

static Arena make_arena(U64 size) {
  void* memory = mmap(NULL, size, PROT_NONE, MAP_ANONYMOUS | MAP_PRIVATE, -1, 0);
  if (memory == MAP_FAILED) {
    print(ERROR, "Failed to reserve memory: ", strerror(errno), ".\n");
    exit(EXIT_FAILURE);
  }

  Arena arena;
  arena.memory = static_cast<U8*>(memory);
  arena.size   = size;
  return arena;
}

static U64 align(U64 offset, U64 alignment) {
  return (offset + alignment - 1) & ~(alignment - 1);
}

static U8* allocate_bytes(Arena* arena, U64 size, U64 alignment) {
  U8* memory    = arena->memory;
  U64 committed = arena->committed;
  
  U64 aligned_used = align(arena->used, alignment);
  U8* bytes        = &memory[aligned_used];
  U64 new_used     = aligned_used + size;
  arena->used      = new_used;

  if (new_used > committed) {
    U64 aligned_difference = align(new_used - committed, 4096);
    mprotect(&memory[committed], aligned_difference, PROT_READ | PROT_WRITE);
    arena->committed += aligned_difference;
  }

  return bytes;
}

static String allocate_buffer(Arena* arena, U64 size) {
  String buffer;
  buffer.data = allocate_bytes(arena, size, 1);
  buffer.size = size;
  return buffer;
}

template <typename A>
static A* allocate(Arena* arena) {
  return reinterpret_cast<A*>(allocate_bytes(arena, sizeof(A), alignof(A)));
}

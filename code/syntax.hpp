struct Environment;

enum TermKind {
  TERM_VARIABLE,
  TERM_ABSTRACTION,
  TERM_APPLICATION,
};

struct Term {
  TermKind kind;
  union {
    String variable;
    struct {
      Environment* environment;
      String       parameter;
      Term*        body;
    };
    struct {
      Term* function;
      Term* argument;
    };
  };
};

static void print(Term* term) {
  switch (term->kind) {
    
  case TERM_VARIABLE:
    print(term->variable);
    break;
    
  case TERM_ABSTRACTION:
    print('\\', term->parameter, '.', term->body);
    break;
    
  case TERM_APPLICATION:
    print('(', term->function, ' ', term->argument, ')');
    break;
  }
}

struct Environment {
  String       name;
  Term*        value;
  Environment* previous;
};

static Term* lookup(Environment* environment, String target) {
  for (Environment* i = environment; i; i = i->previous) {
    if (i->name == target) {
      return i->value;
    }
  }
  return NULL;
}

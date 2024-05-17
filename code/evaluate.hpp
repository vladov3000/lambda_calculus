static Term* evaluate(Arena* arena, Term* term, Environment* environment) {
  switch (term->kind) {
    
  case TERM_VARIABLE: {
    Term* value = lookup(environment, term->variable);
    if (value) {
      return value;
    } else {
      print(ERROR, '"', term->variable, "\" is not defined.\n");
      exit(EXIT_FAILURE);
    }
  }

  case TERM_ABSTRACTION: {
    Term* copy        = allocate<Term>(arena);
    *copy             = *term;
    copy->environment = environment;
    return copy;
  }

  case TERM_APPLICATION: {
    Term* function = evaluate(arena, term->function, environment);
    if (function->kind == TERM_ABSTRACTION) {
      Environment* new_environment = allocate<Environment>(arena);
      new_environment->name        = function->parameter;
      new_environment->value       = evaluate(arena, term->argument, environment);
      new_environment->previous    =
	function->environment ? function->environment : environment;
      return evaluate(arena, function->body, new_environment);
    } else {
      print(ERROR, function, "is not a function.\n");
      exit(EXIT_FAILURE);
    }
  }
  };
}

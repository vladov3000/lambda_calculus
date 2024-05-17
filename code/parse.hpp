static Term* parse_term(Arena* arena, Lexer* lexer) {
  Token token = lexer->lex();
  if (token.kind == TOKEN_IDENTIFIER) {
    Term* result     = allocate<Term>(arena);
    result->kind     = TERM_VARIABLE;
    result->variable = token.text;
    return result;
  } else if (token.text == "\\") {
    token = lexer->lex();
    if (token.kind == TOKEN_IDENTIFIER) {
      String parameter = token.text;
      token            = lexer->lex();
      if (token.text == ".") {
	Term* result        = allocate<Term>(arena);
	result->kind        = TERM_ABSTRACTION;
	result->environment = NULL;
	result->parameter   = parameter;
	result->body        = parse_term(arena, lexer);
	return result;
      } else {
	print(ERROR, "Expected '.', got \"", token.text, "\".\n");
	exit(EXIT_FAILURE);
      }
    } else {
      print(ERROR, "Expected parameter, got \"", token.text, "\".\n");
      exit(EXIT_FAILURE);
    }
  } else if (token.text == "(") {
    Term* result     = allocate<Term>(arena);
    result->kind     = TERM_APPLICATION;
    result->function = parse_term(arena, lexer);
    result->argument = parse_term(arena, lexer);

    while (true) {
      token = lexer->peek();
      if (token.text == ")") {
	token = lexer->lex();
	return result;
      } else {
	Term* function   = result;
	result           = allocate<Term>(arena);
	result->kind     = TERM_APPLICATION;
	result->function = function;
	result->argument = parse_term(arena, lexer);
      }
    }
  } else {
    print(ERROR, "Expected term, got \"", token.text, "\".\n");
    exit(EXIT_FAILURE);
  }
}

static Environment* parse_environment(Arena* arena, Lexer* lexer) {
  Environment* environment = NULL;
  while (true) {
    Environment* new_environment = allocate<Environment>(arena);
    Token        token           = lexer->lex();
    if (token.kind == TOKEN_IDENTIFIER) {
      new_environment->name = token.text;
      token                 = lexer->lex();
      if (token.text == "=") {
	new_environment->value    = parse_term(arena, lexer);
	new_environment->previous = environment;
	environment               = new_environment;
      } else {
	print(ERROR, "Expected '=', got \"", token.text, "\".\n");
	exit(EXIT_FAILURE);
      }
    } else if (token.kind == TOKEN_END) {
      return environment;
    } else {
      print(ERROR, "Expected definition, got \"", token.text, "\".\n");
      exit(EXIT_FAILURE);
    }
  }
}

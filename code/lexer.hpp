enum TokenKind {
  TOKEN_NULL,
  TOKEN_END,
  TOKEN_SYMBOL,
  TOKEN_IDENTIFIER,
};

struct Token {
  TokenKind kind = TOKEN_NULL;
  String    text = "";
};

static bool is_space(U8 c) {
  return c == ' ' || c == '\t' || c == '\n';
}

static String symbols = "\\().=";

static bool is_symbol(U8 c) {
  for (U8 symbol : symbols) {
    if (c == symbol) {
      return true;
    }
  }
  return false;
}

struct Lexer {
  String input  = "";
  I64    offset = 0;
  Token  peeked;

  Token lex() {
    if (peeked.kind != TOKEN_NULL) {
      Token token = peeked;
      peeked.kind = TOKEN_NULL;
      return token;
    }
    
    while (offset < input.size && is_space(input[offset])) {
      offset++;
    }

    if (offset == input.size) {
      Token token;
      token.kind = TOKEN_END;
      return token;
    }

    U8 c = input[offset];
    if (c == '#') {
      while (offset < input.size && input[offset] != '\n') {
	offset++;
      }
      return lex();
    }
    
    if (is_symbol(c)) {
      Token token;
      token.kind = TOKEN_SYMBOL;
      token.text = input.slice(offset++, 1);
      return token;
    }

    I64 start = offset;
    while (offset < input.size && !is_space(input[offset]) && !is_symbol(input[offset])) {
      offset++;
    }

    Token token;
    token.kind = TOKEN_IDENTIFIER;
    token.text = input.slice(start, offset - start);
    return token;
  }

  Token peek() {
    Token token = lex();
    peeked      = token;
    return token;
  }
};

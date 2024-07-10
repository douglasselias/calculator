#include "string.h"

typedef enum {
  NUMBER  = 1,
  FLOAT   = 2,
  OP_SUM  = 3,
  OP_SUB  = 4,
  OP_MULT = 5,
  OP_DIV  = 6,
} TokenTypes;

typedef struct {
  TokenTypes type;
  char* text;
} Token;

static char input[20] = {0};
static int input_size = 0;

static Token* current_token = NULL;
static int token_index = 0;

static Token** tokens;
static int tokens_count = 0;

void next_token() {
  token_index++;
  current_token = tokens[token_index];
  // printf("Token: %d, %s\n", current_token->type, current_token->text);
}

int parse_number() {
  int value = TextToInteger(current_token->text);
  next_token();
  return value;
}

int precedence(char token) {
  switch (token) {
    case '+': return 1;
    case '-': return 1;
    case '*': return 2;
    case '/': return 2;
    default:  return 0;
  }
}

int expression(int binding_power);

int parse_expression(int left) {
  switch (current_token->text[0]) {
    case '+':
      next_token();
      // printf("Op: %d, %s\n", left,current_token->text);
      return left + expression(1);
    case '-':
      next_token();
      return left - expression(1);
    case '*':
      next_token();
      return left * expression(2);
    case '/':
      next_token();
      return left / expression(2);
    default:
      return 0;
  }
}

int expression(int min_precedence) {
  int left = 0;

  if(current_token == NULL) return left;

  if(current_token->type == NUMBER) {
    left = parse_number();
  }

  if(current_token == NULL) return left;

  while (precedence(current_token->text[0]) > min_precedence) {
    left = parse_expression(left);
    if(current_token == NULL) return left;
  }

  return left;
}

void clear_input() {
  for(int i = 0; i < 20; i++) input[i] = '\0';
  input_size = 0;
}

int MAX_TOKENS = 100;
void reset_tokens() {
  for(int i = 0; i < MAX_TOKENS; i++) {
    tokens[i] = NULL;
  }
  tokens_count = 0;
}

void init_tokens() {
  tokens = malloc(MAX_TOKENS * sizeof(Token*));
  reset_tokens();
}

Token** tokenizer() {
  int i = 0;

  char current_char = input[i];
  bool is_tokenizing_number = false;
  int start_index = 0;
  int end_index = 0;
  while(current_char != '\0') {
    switch(current_char) {
      case '+': {
        tokens[tokens_count] = malloc(sizeof(Token));
        // tokens[tokens_count] = &(Token){OP_SUM, "+"};
        /// @todo: can I delete this?
        tokens[tokens_count]->type = OP_SUM;
        // tokens[tokens_count]->text = strdup("+");
        tokens[tokens_count]->text = "+";

        tokens_count++;
        break;
      }
      case '-': {
        tokens[tokens_count] = malloc(sizeof(Token));
        tokens[tokens_count]->type = OP_SUB;
        tokens[tokens_count]->text = "-";
        tokens_count++;
        break;
      }
      case '*': {
        tokens[tokens_count] = malloc(sizeof(Token));
        tokens[tokens_count]->type = OP_MULT;
        tokens[tokens_count]->text = "*";
        tokens_count++;
        break;
      }
      case '/': {
        tokens[tokens_count] = malloc(sizeof(Token));
        tokens[tokens_count]->type = OP_DIV;
        tokens[tokens_count]->text = "/";
        tokens_count++;
        break;
      }
      default: {
        if(isdigit(current_char)) {
          start_index = i;
          while(isdigit(current_char)) {
            i++;
            current_char = input[i];
          }
          end_index = i;
          int number_length = end_index - start_index;

          tokens[tokens_count] = malloc(sizeof(Token));
          tokens[tokens_count]->type = NUMBER;
          tokens[tokens_count]->text = malloc((number_length) * sizeof(char));
          strncpy(tokens[tokens_count]->text, input + start_index, number_length);
          tokens[tokens_count]->text[number_length] = '\0';
          tokens_count++;

          start_index = 0;
          end_index = 0;
          continue;
        }
      }
    }
    i++;
    current_char = input[i];
  }

  // for(int index = 0; index < tokens_count; index++) {
  //   TraceLog(LOG_WARNING, TextFormat("(%s)", tokens[index]->text));
  // }

  return tokens;
}

void evaluate() {
  reset_tokens();
  tokenizer();
  for(int index = 0; index < tokens_count; index++) {
    TraceLog(LOG_WARNING, TextFormat("(%s)", tokens[index]->text));
  }
  current_token = tokens[0];
  int result = expression(0);
  clear_input();
  sprintf(input, "%d", result);
  
  while(input[input_size] != '\0') {
    input_size++;
  }
  token_index = 0;
}

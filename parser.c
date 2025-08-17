#include <stdio.h>
#include <string.h>
#include <stdlib.h>

char input[20];
int input_size = 0;

void clear_input()
{
  memset(input, '\0', 20);
  input_size = 0;
}

typedef enum
{
  OPERATOR,
  NUMBER,
  LEFT_PAREN,
  RIGHT_PAREN,
} TokenType;

typedef struct
{
  TokenType type;
  union
  {
    char operator;
    int  number;
  };
} Token;

typedef struct
{
  Token *data;
  int   size;
  int   i;
} Tokens;

int precedence_table(char operator)
{
  switch(operator)
  {
    case '+': return 1;
    case '-': return 1;
    case '*': return 2;
    case '/': return 2;
    default:  return 0;
  }
}

Tokens tokenize()
{
  Tokens tokens = {};
  tokens.data = calloc(1, sizeof(Token) * 100);

  size_t len = strlen(input);

  for(int i = 0; i < len; i++)
  {
    char c = input[i];

    if(c == ' ') continue;

    if(c == '(')
    {
      tokens.data[tokens.size++] = (Token){.type = LEFT_PAREN};
      continue;
    }

    if(c == ')')
    {
      tokens.data[tokens.size++] = (Token){.type = RIGHT_PAREN};
      continue;
    }

    if(c == '+' || c == '-' || c == '*' || c == '/')
    {
      tokens.data[tokens.size++] = (Token){.type = OPERATOR, .operator = c};
      continue;
    }

    if('0' <= c && c <= '9')
    {
      int number = 0;

      while('0' <= input[i] && input[i] <= '9')
      {
        int digit = input[i] - '0';
        number *= 10;
        number += digit;
        i++;
      }
      i--; // Undoing last increment to correct the index for the next token.

      tokens.data[tokens.size++] = (Token){.type = NUMBER, .number = number};
      continue;
    }
  }

  return tokens;
}

int pratt_parser(Tokens *tokens, int min_precedence)
{
  int left = 0;

  Token token = tokens->data[tokens->i];

  switch(token.type)
  {
    case NUMBER:
    {
      left = token.number;
      tokens->i++;
      if(tokens->i == tokens->size) return left;
      break;
    }
    case LEFT_PAREN:
    {
      tokens->i++; // Skip left paren
      left = pratt_parser(tokens, 0);
      tokens->i++; // Skip right paren
    }
    case OPERATOR:
    {
      if(token.operator == '-')
      {
        tokens->i++; // Skip the '-'
        int operand = pratt_parser(tokens, 3); // Precedence 3 (higher than * and /)
        left = -operand;
      }
    }
  }

  token = tokens->data[tokens->i];

  while(precedence_table(token.operator) > min_precedence)
  {
    tokens->i++;

    char operator = token.operator;
    int right = pratt_parser(tokens, precedence_table(operator));

    switch(operator)
    {
      case '+': left += right; break;
      case '-': left -= right; break;
      case '*': left *= right; break;
      case '/': left /= right; break;
    }

    token = tokens->data[tokens->i];
  }

  return left;
}

void evaluate()
{
  Tokens tokens = tokenize();
  int result = pratt_parser(&tokens, 0);
  free(tokens.data);

  clear_input();
  sprintf(input, "%d", result);
  input_size = (int)strlen(input);
}
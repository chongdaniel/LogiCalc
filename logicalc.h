#ifndef LOGICALC_H
#define LOGICALC_H

#include <stdbool.h>

/* Global variables */

extern int g_variable_count;
extern char *g_variable_names[20];
extern bool *g_variable_address[20];

/* Constant definitions */
#define MAX_STRING_LEN (50)
#define NUM_OPS        (7)

/* Error codes */
#define OK             (-1)
#define BAD_INPUT      (-2)
#define OUTPUT_ERROR   (-3)

typedef enum {
  END = -1,
  VAR = 0,
  NOT = 1,
  AND = 2,
  OR = 3,
  XOR = 4,
  IMPL = 5,
  BICOND = 6
} operation;


/* Structure declaration */
typedef struct proposition {
  operation o;
  bool value;
  struct proposition *a;
  struct proposition *b;
  char name[100];
} proposition;


/* prototype */

bool solve(proposition *);



#endif //LOGICALC_H

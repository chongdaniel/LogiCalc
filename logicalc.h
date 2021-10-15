#ifndef LOGICALC_H
#define LOGICALC_H

#include <stdbool.h>

/* Constant definitions */
#define MAX_STRING_LEN (50)
#define NUM_OPS        (7)

/* Error codes */
#define OK             (-1)
#define BAD_INPUT      (-2)
#define OUTPUT_ERROR   (-3)

typedef enum {
  END = 0,
  AND = 1,
  OR = 2,
  XOR = 3,
  IMPL = 4,
  BICOND = 5,
  NOT = 6
} operation;


/* Structure declaration */
typedef struct {
  operation o;
  bool value;
  struct proposition *a;
  struct proposition *b;
} proposition;


/* prototype */

bool solve(proposition *);



#endif //LOGICALC_H

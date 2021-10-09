#ifndef LOGICALC_H
#define LOGICALC_H

#include <stdbool.h>

/* Constant definitions */
  
/* Error codes */
#define OK             (-1)
#define BAD_INPUT      (-2)
#define OUTPUT_ERROR   (-3)

typedef enum {
  END = 0,
  AND = 1,
  OR = 2,
  XOR = 3,
  NOR = 4,
  IMPL = 5,
  BICOND = 6,
  NOT = 7
} operation;


/* Structure declaration */
typedef struct {
  operation o;
  bool value;
  struct proposition *a;
  struct proposition *b;
} proposition;


/* prototype */

bool solve(proposition *, proposition *, operation);



#endif //LOGICALC_H

/* Logic Calculator
 * Daniel Chong and Benson Tsai
 * A proof-of-concept propositional logic calculator.
 */

#include "logicalc.h"

#include <stdio.h>
#include <stdbool.h>

int g_solve_count = 0;

proposition generate_prop(char *input, int input_length) {
  int level = 0;
  int str_lvl[input_length] = {}
  for (int i = 0; i < input_length; i++) {
    if (input[i] == '(') {
      level++;
    }
    else if (input[i] == ')') {
      level--;
    }
    str_lvl[i] = level;
  }

  return prop;

}

bool solve(proposition *input) {
  proposition *left = input->a;
  proposition *right = input->b;
  operation function = input->o;
//  printf("in function NOTC: %d\n", right->o);
  if (left->o != END) {
//    printf("left->a: %p\n", left->a);
//    printf("Pre-Left value: %d\n", left->value);
//    printf("Pre-Left operator: %d\n", left->o);

    left->value = solve(left);
//    printf("Left value: %d\n", left->value);
  }
  if (right->o != END) {
//    printf("right->a: %p\n", right->a);
//    printf("Pre-Right value: %d\n", right->value);
//    printf("Pre-Right operator: %d\n", right->o);
    right->value = solve(right);
//    printf("Post-Right operator: %d\n", right->o);
//    printf("Right value: %d\n", right->value);
  }
  switch (function) {
    case AND:
      return left->value && right->value;
    case OR:
      return left->value || right->value;
    case NOT: {
      //bool temp = left->value;
      //return !temp;
      return !(left->value);
    }
    case XOR:
      if (!(left->value) == right->value) {
        return true;
      }
      else {
        return false;
      }
    case IMPL: {
      proposition temp_first = {END, !left->value, NULL, NULL};
      proposition temp_second = {END, right->value, NULL, NULL};
      proposition temp = {OR, NULL, &temp_first, &temp_second};
      return solve(&temp);
    }
    case BICOND:
      if (left->value == right->value) {
        return true;
      }
      else {
        return false;
      }
//    case NULL:
//     printf("fuck fuck fuck\n");
  }

  return true;
}


int main() {
  proposition capital_a = {END, true, NULL, NULL};
  proposition capital_b = {END, false, NULL, NULL};

  //testing for nested struct to see if recursion works
  printf("testing recursion\n");
  bool con_A = true;
  bool con_B = true;
  bool con_C = true;
  bool con_D = true;
  proposition rec_testA = {END, con_A, NULL, NULL};
  proposition rec_testB = {END, con_B, NULL, NULL};
  proposition rec_testC = {END, con_C, NULL, NULL};
  proposition rec_testD = {END, con_D, NULL, NULL};

  //statement: (A && B) -> !C
  proposition A_and_B = {AND, NULL, &rec_testA, &rec_testB};
  proposition A_and_C = {AND, NULL, &rec_testA, &rec_testC};
  proposition A_or_B = {OR, NULL, &rec_testA, &rec_testB};
  proposition R1 = {AND, NULL, &A_and_B, &A_and_C};
  proposition R2 = {AND, NULL, &A_and_C, &A_and_B};
  proposition C_and_D = {OR, NULL, &rec_testC, &rec_testD};
  proposition not_C_and_D = {NOT, NULL, &C_and_D, &C_and_D};
  proposition not_C = {NOT, NULL, &rec_testC, &rec_testC};

  proposition final = {IMPL, false, &A_and_B, &not_C};
/*
  for (int i = 0; i < 16; i++) {
    printf("A: %d, B: %d, C: %d, D: %d\ ",
      (&rec_testA)->value, (&rec_testB)->value, (&rec_testC)->value, (&rec_testD)->value);
    printf("R: %d\n", solve(&A_and_B, &not_C_and_D, IMPL));
    (&rec_testD)->value = !(&rec_testD)->value;
    if ((i + 1) % 2 == 0) {
      (&rec_testC)->value = !(&rec_testC)->value;
    }
    if ((i + 1) % 4 == 0) {
      (&rec_testB)->value = !(&rec_testB)->value;
    }
    if ((i + 1) % 8 == 0) {
      (&rec_testA)->value = !(&rec_testA)->value;
    }
  }
*/
  //XOR test
  for (int i = 0; i < 4; i++) {
    printf("A: %d, B: %d ", (&rec_testA)->value, (&rec_testB)->value);
    printf("R: %d\n", solve(&final));

    (&rec_testB)->value = !(&rec_testB)->value;
    if ((i + 1) % 2 == 0) {
      (&rec_testA)->value = !((&rec_testA)->value);
    }
  }

  return OK;
}

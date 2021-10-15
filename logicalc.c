/* Logic Calculator
 * Daniel Chong and Benson Tsai
 * A proof-of-concept propositional logic calculator.
 */

#include "logicalc.h"

#include <stdio.h>
#include <stdbool.h>
#include <string.h>

int g_solve_count = 0;

char *fix_parenthesis(char *input) {
  //fix back parenthesis
  if (input[strlen(input) - 1] != ')') {
    input[strlen(input)] = ')';
  }
  //fix front parenthesis
  if (input[0] != '(') {
    strcpy(&input[1], input);
    input[0] = '(';
  }
  return input;
}

char *replace_o(char *input) {
  int i = 1;
  while (i < NUM_OPS) {
    bool condition = true;
    while (condition) {
      char *index = NULL;
      switch (i) {
        case 1: {
          index = strstr(input, "AND");
          if (index != NULL) {
            *index = '1';
            *(index + 1) = ' ';
            *(index + 2) = ' ';
          }
          else {
            condition = false;
            break;
          }
        }
        case 2: {
          index = strstr(input, "OR");
          if (index != NULL) {
            *index = '2';
            *(index + 1) = ' ';
          }
          else {
            condition = false;
            break;
          }
        }
        case 3: {
          index = strstr(input, "XOR");
          if (index != NULL) {
            *index = '3';
            *(index + 1) = ' ';
            *(index + 2) = ' ';
          }
          else {
            condition = false;
            break;
          }
        }
        case 4: {
          index = strstr(input, "IMPL");
          if (index != NULL) {
            *index = '4';
            *(index + 1) = ' ';
            *(index + 2) = ' ';
            *(index + 3) = ' ';
          }
          else {
            condition = false;
            break;
          }
        }
        case 5: {
          index = strstr(input, "BICOND");
          if (index != NULL) {
            *index = '5';
            *(index + 1) = ' ';
            *(index + 2) = ' ';
            *(index + 3) = ' ';
            *(index + 4) = ' ';
            *(index + 5) = ' ';
          }
          else {
            condition = false;
            break;
          }
        }
        case 6: {
          index = strstr(input, "NOT");
          if (index != NULL) {
            *index = '6';
            *(index + 1) = ' ';
            *(index + 2) = ' ';
          }
          else {
            condition = false;
            break;
          }
        }
      }
    }
    i++;
  }
  return input;
}


proposition *generate_tree(char *input) {
  proposition prop = {END, false, NULL, NULL};
  //find levels
  int str_lvl[102] = {};
  int level = 0;
  for (int i = 0; i < strlen(input); i++) {
    if (input[i] == '(') {
      level++;
    }
    str_lvl[i] = level;
    if (input[i] == ')') {
      level--;
    }
  }

  // do some string splitting magic
  int lpo = -1; // index of least precedent operator of this level
  //finds least precedent operator
  for (int i = 0; i < strlen(input); i++) {
    if (((input[i] == '1')
      || (input[i] == '2') || (input[i] == '3') || (input[i] == 4)
      || (input[i] == '5') || (input[i] == '6'))) {
      if (lpo == -1) {
        lpo = i;
        level = str_lvl[i];
      }
      else {
        if ((str_lvl[i] < level) || 
          (((int) input[i]) >= ((int) input[lpo]) && (str_lvl[i] == level))) {
          lpo = i;
          level = str_lvl;
        }
      }
    }
  }
  // no proposition found
  if (lpo == -1) {
    return &prop;
  }
  else if (input[lpo] == '6') {//NOT operation
    prop.o = NOT;
    int end_string = -1;
    for (int i = (lpo + 4); i < strlen(input); i++) {
      if (str_lvl[i] < level) {
        end_string = i;
        break;
      }
    }
    char new_string[102] = {};
    strncpy(new_string, &input[lpo + 4], end_string - (lpo + 4));
    prop.a = generate_tree(fix_parenthesis(new_string));
    prop.b = prop.a;
    return &prop;
  }
  else {
    //find start of left string
    int start_left = -1;
    for (int i = 0; i < strlen(input); i++) {
      if (str_lvl[i] == (level + 1)) {
        start_left = i;
        break;
      }
    }
    //find end of left string (index of operator - 1)
    int end_left = lpo - 1;
    //make left string
    char left_string[102] = {};
    strncpy(left_string, &input[start_left], end_left - start_left);
    //find start of right string
    int start_right = -1;
    for (int i = lpo + 1; i < strlen(input); i++) {
      if (input[i] != ' ') {
        start_right = i;
        break;
      }
    }
    //find end of right string
    int end_right = -1;
    for (int i = start_right; i < strlen(input); i++) {
      if (str_lvl[i] < level) {
        end_right = i;
      }
    }
    if (end_right = -1) {
      end_right = strlen(input);
    }
    //make right string
    char right_string[102] = {};
    strncpy(right_string, &input[start_right], end_right - start_right);
    //determine prop
    switch (input[lpo]) {
      case '1':
        prop.o = AND;
      case '2':
        prop.o = OR;
      case '3':
        prop.o = XOR;
      case '4':
        prop.o = IMPL;
      case '5':
        prop.o = BICOND;
    }
    prop.a = generate_tree(left_string);
    prop.b = generate_tree(right_string);
    return &prop;
  }
}

proposition generate_prop(char *input) {
  proposition prop = {END, NULL, NULL, NULL};
  int level = 0;
  int str_lvl[102] = {};
  for (int i = 0; i < strlen(input); i++) {
    if (input[i] == '(') {
      level++;
    }
    str_lvl[i] = level;
    if (input[i] == ')') {
      level--;
    }
  }
  for (int i = 0; i < strlen(input); i++) {
    printf("%d", str_lvl[i]);
  }
  printf("\n");
  input = replace_o(input);
//  generate_tree(input, str_lvl, 1);
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

  char test_string[100] = {"(((A OR X OR B) IMPL (NOT C)) AND D) BICOND Z"};
  printf("%s\n", test_string);
  char test_string2[102] = {};
  test_string2[0] = '(';
  strcpy(&test_string2[1], test_string);
  test_string2[strlen(test_string2)] = ')';
//  proposition boo = generate_prop(test_string, strlen(test_string));
  printf("%s\n", test_string2);
  printf("replace test\n");
  char *test_string3 = replace_o(&test_string2);
  printf("%s\n", test_string3);
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
/*
  for (int i = 0; i < 4; i++) {
    printf("A: %d, B: %d ", (&rec_testA)->value, (&rec_testB)->value);
    printf("R: %d\n", solve(&final));

    (&rec_testB)->value = !(&rec_testB)->value;
    if ((i + 1) % 2 == 0) {
      (&rec_testA)->value = !((&rec_testA)->value);
    }
  }
*/
  return OK;
}

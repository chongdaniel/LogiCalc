/* Logic Calculator
 * Daniel Chong and Benson Tsai
 * A proof-of-concept propositional logic calculator.
 */

#include "logicalc.h"

#include <stdio.h>
#include <stdbool.h>
#include <string.h>

int g_variable_count = 0;
char *g_variable_name[20] = {};
bool *g_variable_address[20] = {};

char *fix_parenthesis(char *input) {
  int count = 0;
  for (int i = 0; i < strlen(input); i++) {
    if (input[i] == '(') {
      count++;
    }
    else if (input[i] == ')') {
      count--;
    }
  }
  if (count != 0) {
    if (count < 0) {
      for (int i = count; i != 0; i++) {
        strncpy(&input[1], input, strlen(input));
        input[0] = '(';
      }
    }
    else if (count > 0) {
      for (int i = count; i != 0; i--) {
        input[strlen(input)] = ')';
      }
    }
  }
  //remove excess parenthesis
  while ((input[0] == '(') && (input[1] == '(')
    && (input[strlen(input) - 1] == ')') && (input[strlen(input) - 2]) == ')') { 
    strncpy(input, &input[1], strlen(input) - 1);
    input[strlen(input) - 2] = '\0';
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
          index = strstr(input, "NOT");
          if (index != NULL) {
            *index = '1';
            *(index + 1) = ' ';
            *(index + 2) = ' ';
            break;
          }
          else {
            condition = false;
            break;
          }
        }
        case 2: {
          index = strstr(input, "AND");
          if (index != NULL) {
            *index = '2';
            *(index + 1) = ' ';
            *(index + 2) = ' ';
            break;
          }
          else {
            condition = false;
            break;
          }
        }
        case 3: {
          index = strstr(input, "OR");
          if (index != NULL) {
            *index = '3';
            *(index + 1) = ' ';
            break;
          }
          else {
            condition = false;
            break;
          }
        }
        case 4: {
          index = strstr(input, "XOR");
          if (index != NULL) {
            *index = '4';
            *(index + 1) = ' ';
            *(index + 2) = ' ';
            break;
          }
          else {
            condition = false;
            break;
          }
        }
        case 5: {
          index = strstr(input, "IMPL");
          if (index != NULL) {
            *index = '5';
            *(index + 1) = ' ';
            *(index + 2) = ' ';
            *(index + 3) = ' ';
            break;
          }
          else {
            condition = false;
            break;
          }
        }
        case 6: {
          index = strstr(input, "BICOND");
          if (index != NULL) {
            *index = '6';
            *(index + 1) = ' ';
            *(index + 2) = ' ';
            *(index + 3) = ' ';
            *(index + 4) = ' ';
            *(index + 5) = ' ';
            break;
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


proposition generate_tree(char *input) {
  printf("variables found: ");
  for (int i = 0; i < g_variable_count; i++) {
    printf("%s ", g_variable_name[i]);
  }
  printf("\n");
  proposition prop = {VAR, false, NULL, NULL, ""};
  strcpy(prop.name, input);
  printf("input: %s, in prop: %s\n", input, prop.name);
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
      || (input[i] == '2') || (input[i] == '3') || (input[i] == '4')
      || (input[i] == '5') || (input[i] == '6'))) {
      if (lpo == -1) {
        lpo = i;
        level = str_lvl[i];
      }
      else {
        if ((str_lvl[i] < level) || 
          (((int) input[i]) >= ((int) input[lpo]) && (str_lvl[i] == level))) {
          lpo = i;
          level = str_lvl[i];
        }
      }
    }
  }
  // no proposition found
  if (lpo == -1) {
    printf("no prop, variable name: %s\n", input);
    g_variable_name[g_variable_count] = prop.name;
    printf("name in g_array %s, address %p\n",
      g_variable_name[g_variable_count], &g_variable_name[g_variable_count]);
    g_variable_address[g_variable_count] = &prop.value;
    printf("g_variable_count: %d\n", g_variable_count);
    g_variable_count++;
    return prop;
  }
  else if (input[lpo] == '1') {//NOT operation
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
    proposition temp_prop_a = generate_tree(fix_parenthesis(new_string));
    prop.a = &temp_prop_a;
    proposition end = {END, false, NULL, NULL, "end"};
    prop.b = &end;
    return prop;
  }
  else {
    //find start of left string
    int start_left = 0;
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
    if (end_right == -1) {
      end_right = strlen(input);
    }
    //make right string
    char right_string[102] = {};
    strncpy(right_string, &input[start_right], end_right - start_right);
    //determine prop
    printf("start_left = %d, end_left = %d\n", start_left, end_left);
    printf("input[lpo] = %c, lpo = %d\n", input[lpo], lpo);
    switch (input[lpo]) {
      case '2':
        prop.o = AND;
        break;
      case '3':
        prop.o = OR;
        break;
      case '4':
        prop.o = XOR;
        break;
      case '5':
        prop.o = IMPL;
        break;
      case '6':
        prop.o = BICOND;
        break;
      default:
        printf("something went terribly wrong\n");
    }
    printf("prop.o: %d\n", prop.o);
    printf("left_string before fix: %s\n", left_string);
    printf("left_string after fix: %s\n", fix_parenthesis(left_string));
    printf("right_string before fix: %s\n", right_string);
    printf("right_string after fix: %s\n", fix_parenthesis(right_string));
    proposition temp_prop_a = generate_tree(fix_parenthesis(left_string));
    prop.a = &temp_prop_a;
    proposition temp_prop_b = generate_tree(fix_parenthesis(right_string));
    prop.b = &temp_prop_b;
    return prop;
  }
}

bool solve(proposition *input) {
  proposition *left = input->a;
  proposition *right = input->b;
  operation function = input->o;
//  printf("in function NOTC: %d\n", right->o);
  if ((left->o != VAR) && (left->o != END)) {
//    printf("left->a: %p\n", left->a);
//    printf("Pre-Left value: %d\n", left->value);
//    printf("Pre-Left operator: %d\n", left->o);

    left->value = solve(left);
//    printf("Left value: %d\n", left->value);
  }
  if ((right->o != VAR) && (right->o != END)) {
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
      proposition temp_first = {END, !left->value, NULL, NULL, "1"};
      proposition temp_second = {END, right->value, NULL, NULL, "2"};
      proposition temp = {OR, NULL, &temp_first, &temp_second, "temp"};
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
  //testing for nested struct to see if recursion works
  printf("testing recursion\n");
  bool con_A = true;
  bool con_B = true;
  bool con_C = true;
  bool con_D = true;
  proposition rec_testA = {VAR, con_A, NULL, NULL, "rec_testA"};
  proposition rec_testB = {VAR, con_B, NULL, NULL, "rec_testB"};
  proposition rec_testC = {VAR, con_C, NULL, NULL, "rec_testC"};
  proposition rec_testD = {VAR, con_D, NULL, NULL, "rec_testD"};

  proposition end = {END, false, NULL, NULL, "end"};

  //statement: (A && B) -> !C
//  proposition A_and_B = {AND, NULL, &rec_testA, &rec_testB};
//  proposition A_and_C = {AND, NULL, &rec_testA, &rec_testC};
//  proposition A_or_B = {OR, NULL, &rec_testA, &rec_testB};
//  proposition R1 = {AND, NULL, &A_and_B, &A_and_C};
//  proposition R2 = {AND, NULL, &A_and_C, &A_and_B};
//  proposition C_and_D = {OR, NULL, &rec_testC, &rec_testD};
//  proposition not_C_and_D = {NOT, NULL, &C_and_D, &end};
  proposition not_C = {NOT, NULL, &rec_testC, &end, "not_C"};

//  printf("not_test: %d\n", solve(&not_C));

  char test_string[100] = {"(((A OR Y OR G OR H OR F OR J AND K OR K OR 0 IMPL C BICOND B) IMPL (CCCC)) AND D) BICOND Z"};
//  printf("%s\n", test_string);
  char test_string2[102] = {};
  test_string2[0] = '(';
  strcpy(&test_string2[1], test_string);
  test_string2[strlen(test_string2)] = ')';

//  printf("%s\n", test_string2);
  printf("replace test\n");

  char *test_string3 = replace_o(test_string2);
//  printf("%s\n", test_string3);
  //testing generate tree
  printf("tree test\n");
  proposition tree = generate_tree(test_string3);
/*
  while (tree.o != VAR) {
    tree = *tree.a;
    printf("tree.o: %d\n", tree.o);
    printf("%s, length: %ld\n", tree.name, strlen(tree.name));
  }
*/
  //variables found
  printf("testing variables\n");
  printf("name 0: %s\n", g_variable_name[0]);
  for (int i = 0; i < g_variable_count; i++) {
    printf("variable %d, name %s, value %d, address %p\n",
      i, g_variable_name[i], *g_variable_address[i], &g_variable_name[i]);
  }
/*
  for (int i = 0; i < 16; i++) {
    printf("A: %d, B: %d, C: %d, D: %d\ ",
      (&rec_testA)->value, (&rec_testB)->value, (&rec_testC)->value, (&rec_testD)->value);
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
*/
  return OK;
}

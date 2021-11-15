/* Logic Calculator
 * Daniel Chong and Benson Tsai
 * A proof-of-concept propositional logic calculator.
 */

#include "logicalc.h"

#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <malloc.h>
#include <math.h>

int g_variable_count = 0;
char *g_variable_name[20] = {};
bool *g_variable_address[20] = {};
proposition *tree = NULL;

char *fix_parenthesis(char *input) {
  int count = 0;
  char *temp = input;
  input = malloc(strlen(input) + 3);
  strncpy(&input[1], temp, strlen(temp));
  input[0] = '(';
  input[strlen(temp) + 1] = ')';
//  free(temp);
  temp = NULL;
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
        temp = input;
        input = malloc(strlen(input) + 2);
        strncpy(&input[1], temp, strlen(temp));
        input[0] = '(';
//        free(temp);
        temp = NULL;
      }
    }
    else if (count > 0) {
      for (int i = count; i != 0; i--) {
        temp = input;
        input = malloc(strlen(input) + 2);
        strncpy(input, temp, strlen(temp));
        input[strlen(temp)] = ')';
//        free(temp);
        temp = NULL;
      }
    }
  }
  //remove excess parenthesis
//  printf("start removing excess parenthesis\n");
//  printf("length of input = %ld\n", strlen(input));
//  printf("input = %s\n", input);
  while ((input[0] == '(') && (input[1] == '(')
    && (input[strlen(input) - 1] == ')') && (input[strlen(input) - 2]) == ')') {
    temp = input;
    input = malloc(strlen(input) - 1);
    strncpy(input, &temp[1], strlen(temp) - 2);
//    free(temp);
    temp = NULL;
  }
//  printf("after removing excess parenthesis\ninput = %s\n", input);
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

proposition *generate_tree(char *input) {
  char *temp_input = input;
  input = malloc(strlen(input) + 1);
  strcpy(input, temp_input);
//  printf("variables found: ");
//  for (int i = 0; i < g_variable_count; i++) {
//    printf("%s ", g_variable_name[i]);
//  }
//  printf("\n");
  proposition *prop = malloc(sizeof(proposition));
//  prop = {VAR, false, NULL, NULL, ""};
  prop->name = malloc(strlen(input) + 1);
  strcpy(prop->name, input);
//  printf("input: %s, in prop: %s\n", input, prop->name);
  prop->a = malloc(sizeof(proposition *));
  prop->a = NULL;
  prop->b = malloc(sizeof(proposition *));
  prop->b = NULL;
  prop->o = VAR;
  prop->value = false;
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
//    printf("no prop, variable name: %s\n", input);
    g_variable_name[g_variable_count] = prop->name;
//    printf("name in g_array %s, address %p\n",
//      g_variable_name[g_variable_count], &g_variable_name[g_variable_count]);
    g_variable_address[g_variable_count] = &prop->value;
    g_variable_count++;
//    printf("g_variable_count: %d\n", g_variable_count);
    return prop;
  }
  else if (input[lpo] == '1') {//NOT operation
    prop->o = NOT;
    int end_string = -1;
    for (int i = (lpo + 4); i < strlen(input); i++) {
      if (str_lvl[i] < level) {
        end_string = i;
        break;
      }
    }
    char new_string[102] = {};
    strncpy(new_string, &input[lpo + 4], end_string - (lpo + 4));
    proposition *temp_prop_a = malloc(sizeof(proposition));
    temp_prop_a = generate_tree(fix_parenthesis(new_string));
    prop->a = temp_prop_a;
    proposition *end = malloc(sizeof(proposition));
//    end = {END, false, NULL, NULL, "end"};
    end->o = END;
    end->value = false;
    end->a = NULL;
    end->b = NULL;
    end->name = NULL;
    prop->b = end;
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
//    printf("start_left = %d, end_left = %d\n", start_left, end_left);
//    printf("input[lpo] = %c, lpo = %d\n", input[lpo], lpo);
    switch (input[lpo]) {
      case '2':
        prop->o = AND;
        break;
      case '3':
        prop->o = OR;
        break;
      case '4':
        prop->o = XOR;
        break;
      case '5':
        prop->o = IMPL;
        break;
      case '6':
        prop->o = BICOND;
        break;
      default:
        printf("something went terribly wrong\n");
    }
//    printf("prop.o: %d\n", prop->o);
//    printf("left_string before fix: %s\n", left_string);
//    printf("left_string after fix:  %s\n", fix_parenthesis(left_string));
//    printf("right_string before fix: %s\n", right_string);
//    printf("right_string after fix:  %s\n", fix_parenthesis(right_string));
    char *fixed_left_string = fix_parenthesis(left_string);
    char *fixed_right_string = fix_parenthesis(right_string);
//    proposition *temp_prop_a = generate_tree(fix_parenthesis(left_string));
//    prop->a = temp_prop_a;
    prop->a = generate_tree(fixed_left_string);
//    proposition *temp_prop_b = generate_tree(fix_parenthesis(right_string));
//    prop->b = temp_prop_b;
    prop->b = generate_tree(fixed_right_string);
    return prop;
  }
}

void solve(proposition *input) {
//  printf("solving %s\n", input->name);
  proposition *left = input->a;
  proposition *right = input->b;
  operation function = input->o;
  if ((left->o != VAR) && (left->o != END)) {
    solve(left);
  }
  if ((right->o != VAR) && (right->o != END)) {
    solve(right);
  }
//  printf("before solve switch\n");
  switch (function) {
    case AND:
      input->value = (left->value && right->value);
      break;
    case OR:
      input->value = (left->value || right->value);
      break;
    case NOT:
      input->value = !(left->value);
      break;
    case XOR:
      if (!(left->value) == right->value) {
        input->value = true;
      }
      else {
        input->value = false;
      }
      break;
    case IMPL: {
//      printf("left value %d, right value %d\n", left->value, right->value);
      proposition temp_first = {END, !left->value, NULL, NULL, "1"};
      proposition temp_second = {END, right->value, NULL, NULL, "2"};
      proposition temp = {OR, false, &temp_first, &temp_second, input->name};
      solve(&temp);
      input->value = temp.value;
      break;
    }
    case BICOND:
      if (left->value == right->value) {
        input->value = true;
      }
      else {
        input->value = false;
      }
      break;
//    case NULL:
//    printf("fuck fuck fuck\n");
  }
//  printf("\ninput name %s, value %d   ", input->name, input->value);
}

//reverse replace
void reverse_replace(char *input) {
  int i = 1;
  while (i < NUM_OPS) {
    bool condition = true;
    while (condition) {
      char *index = NULL;
      switch (i) {
        case 1: {
          index = strstr(input, "1");
          if (index != NULL) {
            *index = 'N';
            *(index + 1) = 'O';
            *(index + 2) = 'T';
            break;
          }
          else {
            condition = false;
            break;
          }
        }
        case 2: {
          index = strstr(input, "2");
          if (index != NULL) {
            *index = 'A';
            *(index + 1) = 'N';
            *(index + 2) = 'D';
            break;
          }
          else {
            condition = false;
            break;
          }
        }
        case 3: {
          index = strstr(input, "3");
          if (index != NULL) {
            *index = 'O';
            *(index + 1) = 'R';
            break;
          }
          else {
            condition = false;
            break;
          }
        }
        case 4: {
          index = strstr(input, "4");
          if (index != NULL) {
            *index = 'X';
            *(index + 1) = 'O';
            *(index + 2) = 'R';
            break;
          }
          else {
            condition = false;
            break;
          }
        }
        case 5: {
          index = strstr(input, "5");
          if (index != NULL) {
            *index = 'I';
            *(index + 1) = 'M';
            *(index + 2) = 'P';
            *(index + 3) = 'L';
            break;
          }
          else {
            condition = false;
            break;
          }
        }
        case 6: {
          index = strstr(input, "6");
          if (index != NULL) {
            *index = 'B';
            *(index + 1) = 'I';
            *(index + 2) = 'C';
            *(index + 3) = 'O';
            *(index + 4) = 'N';
            *(index + 5) = 'D';
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
}

//travserse tree and print
void print_tree(proposition *input, int control) {//control 0 print value, 1 print name
  proposition *left = input->a;
  proposition *right = input->b;
  operation function = input->o;
  if ((left->o != VAR) && (left->o != END)) {
    print_tree(left, control);
  }
  if ((right->o != VAR) && (right->o != END)) {
    print_tree(right, control);
  }
  if (control == 0) {
    int spaces_before = strlen(input->name) / 2;
    for (int i = 0; i < spaces_before; i++) {
      printf(" ");
    }
    printf("%d", input->value);
    for (int i = spaces_before + 1; i < strlen(input->name); i++) {
      printf(" ");
    }
  } else if (control == 1) {
    reverse_replace(input->name);
    printf("%s ", input->name);
  }
}

//display variable values
void display_values() {
  for (int j = 0; j < g_variable_count; j++) {
    printf("%d ", *g_variable_address[j]);
  }
  solve(tree);
//  printf("after solve\n");
//  printf("%d\n", tree->value);
  print_tree(tree, 0);
  printf("\n");
}

//function to alternate values of variables prototype
void alternate_var_values(int var_index) {//at first i is 0
  if (var_index == g_variable_count) {
    display_values();
    return;
  }
  for (int i = 0; i < 2; i++) {
    alternate_var_values(var_index + 1);
    *g_variable_address[var_index] = !*g_variable_address[var_index];
  }
}

int main() {
  //testing for nested struct to see if recursion works
//  printf("testing recursion\n");
  bool con_A = true;
  bool con_B = true;
  bool con_C = true;
  bool con_D = true;
  proposition rec_testA = {VAR, con_A, NULL, NULL, "rec_testA"};
  proposition rec_testB = {VAR, con_B, NULL, NULL, "rec_testB"};
  proposition rec_testC = {VAR, con_C, NULL, NULL, "rec_testC"};
  proposition rec_testD = {VAR, con_D, NULL, NULL, "rec_testD"};
  proposition end = {END, false, NULL, NULL, "end"};
  proposition not_C = {NOT, NULL, &rec_testC, &end, "not_C"};

//  printf("not_test: %d\n", solve(&not_C));

  // "(((A OR Y OR F AND G BICOND H IMPL B) IMPL (CCCC)) AND D) BICOND Z"
  // "((A OR B) AND C) IMPL D"
  char test_string[100] = {"(A AND B) OR C"};
  printf("%s\n", test_string);

  char test_string2[102] = {};
  test_string2[0] = '(';
  strcpy(&test_string2[1], test_string);
  test_string2[strlen(test_string2)] = ')';

  //replace test

  char *test_string3 = replace_o(test_string2);

  //testing generate tree
//  printf("tree test\n");
  tree = generate_tree(test_string3);

  //print out left most variables
//  printf("print out left most side of tree\n");
//  while (tree->o != VAR) {
//    tree = tree->a;
//    printf("tree->o: %d\n", tree->o);
//    printf("%s, length: %ld\n", tree->name, strlen(tree->name));
//  }

  //tree
//  printf("tree name %s value %d\n", tree->name, tree->value);
//  printf("tree left name %s value %d\n", tree->a->name, tree->a->value);
//  printf("tree right name %s value %d\n", tree->b->name, tree->b->value);

  //variables found
//  printf("testing variables\n");
  for (int i = 0; i < g_variable_count; i++) {
    printf("variable %d, name %s, value %d, address %p\n",
      i, g_variable_name[i], *g_variable_address[i], &g_variable_name[i]);
  }

  //change this shit to recursive
  //test alternate with address
//  printf("testing alternate with address with 4 variables\n");
  printf("g_variable_count = %d\n", g_variable_count);
//  printf("tree initial value %d\n", tree->value);
//  printf("A B C D E ");
  printf("A B C ");
  print_tree(tree, 1);
  printf("\n");
  /*
  for (int i = 1; i <= 16; i++) {
    display_vars();
    *g_variable_address[3] = !*g_variable_address[3];
    if (i % 2 == 0) {
      *g_variable_address[2] = !*g_variable_address[2];
    }
    if (i % 4 == 0) {
      *g_variable_address[1] = !*g_variable_address[1];
    }
    if (i % 8 == 0) {
      *g_variable_address[0] = !*g_variable_address[0];
    }
  }
  */
  alternate_var_values(0);

  return OK;
}

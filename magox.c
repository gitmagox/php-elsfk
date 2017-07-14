/*
  +----------------------------------------------------------------------+
  | PHP Version 7                                                        |
  +----------------------------------------------------------------------+
  | Copyright (c) 1997-2017 The PHP Group                                |
  +----------------------------------------------------------------------+
  | This source file is subject to version 3.01 of the PHP license,      |
  | that is bundled with this package in the file LICENSE, and is        |
  | available through the world-wide-web at the following url:           |
  | http://www.php.net/license/3_01.txt                                  |
  | If you did not receive a copy of the PHP license and are unable to   |
  | obtain it through the world-wide-web, please send a note to          |
  | license@php.net so we can mail you a copy immediately.               |
  +----------------------------------------------------------------------+
  | Author:                                                              |
  +----------------------------------------------------------------------+
*/

/* $Id$ */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif
#include "php.h"
#include "php_ini.h"
#include "ext/standard/info.h"
#include "php_magox.h"
#include <stdio.h>
#include <signal.h>
#include <sys/time.h>
#include <setjmp.h>
#include <termios.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/time.h>
#include <string.h>


/* If you declare any globals in php_magox.h uncomment this:
ZEND_DECLARE_MODULE_GLOBALS(magox)
*/

/* True global resources - no need for thread safety here */
static int le_magox;

//init for globle
void
init_for_globle(void)
{
  x = X / 2 - 2;   
  flag_erase = 1;
  srand(getpid());  
  num = rand() % 7; 
  mode = rand() % 4;  
  color = rand() % 7 + 41;  

  next_num = rand() % 7;
  next_mode = rand() % 4;
  save_color = rand() % 7 + 41;

  print_start_interface();  
  print_score();    
  print_level();    
}

//print start interface
void
print_start_interface(void)
{
  int x, y;
  printf("\33[2J");
  printf("\33[%d;%dH\33[32m分数:\33[0m", p_y + 10, p_x + 25);
  printf("\33[%d;%dH\33[32m等级:\33[0m", p_y + 14, p_x + 25);
  for (x = p_x, y = p_y; x <= 46; x++)
    printf("\33[%d;%dH\33[41m==\33[0m", y, x);
  for (x = p_x, y = p_y + 1; y <= 25; y++)
    printf("\33[%d;%dH\33[41m||\33[0m", y, x);
  for (x = p_x + 22, y = p_y + 1; y <= 25; y++)
    printf("\33[%d;%dH\33[41m||\33[0m", y, x);
  for (x = p_x + 36, y = p_y + 1; y <= 25; y++)
    printf("\33[%d;%dH\33[41m||\33[0m", y, x);
  for (x = p_x + 24, y = p_y + 8; x <= 44; x++)
    printf("\33[%d;%dH\33[41m--\33[0m", y, x);
  for (x = p_x, y = p_y + 21; x <= 46; x++)
    printf("\33[%d;%dH\33[41m==\33[0m", y, x);
  printf("\33[?25l");
  fflush(stdout);
}

//erase last diamonds
void
erase_last(void)
{
  int j, x1, y1, n;
  x1 = save_x + p_x + 2;
  for (j = 0, n = 0; j < 16; j++) {
    if (j / 4 >= shape[num][save_mode][16] && j % 4 == 0) {
      y1 = save_y + p_y + 1 + n;
      printf("\33[%d;%dH", y1, x1);
      n++;
    }
    if (j / 4 >= shape[num][save_mode][16]
        && j % 4 >= shape[num][save_mode][17]) {
      if (shape[num][save_mode][j] == 0) {
        printf("\33[2C");
      }
      if (shape[num][save_mode][j] == 1) {
        printf("  ");
      }
    }
  }
  fflush(stdout);
}

//print modes shape
void
print_mode_shape(void)
{
  int j, x1, y1, n;
  int left_flag = 0;
  if (flag_erase == 0) {
    erase_last();
  }
  x1 = x + p_x + 2;
  for (j = 0, n = 0; j < 16; j++) {
    if (j / 4 >= shape[num][mode][16] && j % 4 == 0) {
      y1 = y + p_y + 1 + n;
      printf("\33[%d;%dH", y1, x1);
      n++;
    }
    if (j / 4 >= shape[num][mode][16]
        && j % 4 >= shape[num][mode][17]) {
      if (shape[num][mode][j] == 0) {
        printf("\33[2C");
      }
      if (shape[num][mode][j] == 1) {
        printf("\33[%dm[]\33[0m", color);
      }
    }
    fflush(stdout);
  }
  printf("\33[0m");
  fflush(stdout);
  save_x = x;
  save_y = y;
  save_mode = mode;
  save_row = 4 - shape[num][mode][16];
  save_col = 4 - shape[num][mode][17];
  flag_erase = 0;

}

//store diamonds to  matrix by color to flag
void
store_flag_color(void)
{
  int i, a = 0, b = 0;
  for (i = 0; i < 16; i++) {
    if (i / 4 >= shape[num][mode][16] && i % 4 == 0) {
      a++;
      b = 0;
    }
    if (i / 4 >= shape[num][mode][16]
        && i % 4 >= shape[num][mode][17]) {
      if (shape[num][save_mode][i] == 0) {
        b = b + 2;
      }
      if (shape[num][save_mode][i] == 1) {
        matirx[save_y + a - 1][save_x + b] = color;
        b++;
        matirx[save_y + a - 1][save_x + b] = color;
        b++;
      }
    }
  }
}

//print the save matrix
void
print_save_matrix(void)
{
  int i, j, n = 0;
  for (i = 0; i < Y; i++) {
    printf("\33[%d;%dH", i + p_y + 1, p_x + 2);
    for (j = 0; j < X; j++) {
      if (matirx[i][j] != 0) {
        n = (n + 1) % 2;
        fprintf(stdout, "\33[%dm", matirx[i][j]);
        (n == 1) ? printf("[") : printf("]");
      }
      if (matirx[i][j] == 0) {
        printf("\33[0m");
        printf(" ");
      }
      fflush(stdout);
    }
  }
}

// change shape
void
change_shape(void)
{
  int i, n;
  for (i = 0; i < save_row; i++) {
    if (num == 6) {
      n = 4;
    } else {
      n = 0;
    }
    if (((x + n) >= X - save_col * 2 && save_col < save_row) ||
        judge_by_color(x, (mode + 1) % 4) == 1) {
      return;
    }
  }
  mode = (mode + 1) % 4;
  fflush(stdout);
  print_mode_shape();
  fflush(stdout);
}

//move right
void
move_right(void)
{
  int i;
  if (x >= X - save_col * 2 || judge_by_color(x + 2, mode) == 1) {
    return;
  }
  x = x + 2;
  print_mode_shape();
  fflush(stdout);
}

// move left
void
move_left(void)
{
  int i;
  if (x <= 0 || judge_by_color(x - 2, mode) == 1) {
    return;
  }
  x = x - 2;
  print_mode_shape();
  fflush(stdout);
}

// move down
void
move_down()
{
  y++;
  if (y >= Y - save_row + 1 || judge_by_color(x, mode) == 1) {
    store_flag_color();
    game_over();
    y = 0;
    save_row = 0;
    save_col = 0;
    x = X / 2 - 2;

    num = next_num;
    mode = next_mode;
    color = save_color;
    next_num = random() % 7;
    next_mode = random() % 4;
    save_color = random() % 7 + 41;
    print_next();
    flag_erase = 1;
    destroy_line();
    fflush(stdout);
    return;
  }
  print_mode_shape();
  fflush(stdout);
}

// fall down
void
fall_down()
{
  while (1) {
    y++;
    if (y >= Y - save_row + 1 || judge_by_color(x, mode) == 1) {
      store_flag_color();
      game_over();
      y = 0;
      save_row = 0;
      save_col = 0;
      x = X / 2 - 2;

      num = next_num;
      mode = next_mode;
      color = save_color;
      next_num = rand() % 7;
      next_mode = rand() % 4;
      save_color = rand() % 7 + 41;
      print_next();
      flag_erase = 1;
      destroy_line();
      fflush(stdout);
      return;
    }
    print_mode_shape();
    fflush(stdout);
  }

}

//erase next tip diamond
void
erase_next(void)
{
  int i, j, n = 0;
  for (i = 0; i < 4; i++) {
    printf("\33[%d;%dH", p_y + 3 + n, p_x + X + 7);
    n++;
    for (j = 0; j < 4; j++) {
      printf("  ");
    }
  }
  printf("\33[30;4H\33[?25l");
  fflush(stdout);
}

//print next tip diamond
void
print_next(void)
{
  int j, n = 0;
  erase_next();
  for (j = 0; j < 16; j++) {
    if (j / 4 >= shape[next_num][next_mode][16] && j % 4 == 0) {
      printf("\33[%d;%dH", p_y + 3 + n, p_x + X + 7);
      n++;
    }
    if (j / 4 >= shape[next_num][next_mode][16]
        && j % 4 >= shape[next_num][next_mode][17]) {
      if (shape[next_num][next_mode] == 0) {
        printf("\33[2C");
      }
      if (shape[next_num][next_mode][j] == 1) {
        printf("\33[%dm[]\33[0m", save_color);
      }
    }
  }
}

//print scores info
void
print_score(void)
{
  printf("\33[%d;%dH\33[31m%d\33[0m", p_y + 10, p_x + X + 10, score);
  fprintf(stdout, "\33[%d;0H", p_y + 20 + 2);
}

//print grades info
void
print_level(void)
{
  printf("\33[%d;%dH\33[31m%d\33[0m", p_y + 14, p_x + X + 10, level);
  fprintf(stdout, "\33[%d;0H", p_y + 20 + 2);
}

//destroy a line or lines    
void
destroy_line(void)
{
  int i, j, full;
  int a, b, c;
  for (i = 0; i < Y; i++) {
    full = 1;
    for (j = 0; j < X; j++) {
      if (matirx[i][j] == 0) {
        full = 0;
      }
    }
    if (full == 1) {
      for (a = 0; a < i; a++) {
        for (b = 0; b < X; b++) {
          matirx[i - a][b] = matirx[i - a - 1][b];
        }
      }
      print_save_matrix();
      score = score + 100;
      if (score % LEVEL_SCORE == 0) {
        level = level + 1;
        if (level >= 9)
          level = 9;
        change_level();
        print_level();

      }
      print_score();
    }
  }
}

//change level , change rate   
void
change_level(void)
{
  switch (level) {
  case 1:
    setitimer(ITIMER_REAL, &level_01, NULL);
    break;
  case 2:
    setitimer(ITIMER_REAL, &level_02, NULL);
    break;
  case 3:
    setitimer(ITIMER_REAL, &level_03, NULL);
    break;
  case 4:
    setitimer(ITIMER_REAL, &level_04, NULL);
    break;
  case 5:
    setitimer(ITIMER_REAL, &level_05, NULL);
    break;
  case 6:
    setitimer(ITIMER_REAL, &level_06, NULL);
    break;
  case 7:
    setitimer(ITIMER_REAL, &level_07, NULL);
    break;
  case 8:
    setitimer(ITIMER_REAL, &level_08, NULL);
    break;
  case 9:
    setitimer(ITIMER_REAL, &level_09, NULL);
    break;
  default:
    break;
  }
}

//by the color to judge whether went across or not
int
judge_by_color(int x, int mode)
{
  int i, a = 0, b = 0;
  for (i = 0; i < 16; i++) {
    if (i / 4 >= shape[num][mode][16] && i % 4 == 0) {
      a++;
      b = 0;
    }
    if (i / 4 >= shape[num][mode][16]
        && i % 4 >= shape[num][mode][17]) {
      if (shape[num][mode][i] == 0) {
        b = b + 2;
      }
      if (shape[num][mode][i] == 1) {
        if (matirx[a + y - 1][b + x] != 0) {
          return 1;
        } else
          b = b + 2;
      }
    }

  }
}

//control the diamonds shape by the key        
void
key_control(void)
{
  int ch, flag = 1;
  struct termios save, raw;

  tcgetattr(0, &save);
  cfmakeraw(&raw);
  tcsetattr(0, 0, &raw);
  if (setjmp(env) == 0) {
    while (flag) {
      ch = getchar();
      if (ch == '\r') {
        fall_down();
      }
      if (ch == '\33') {
        ch = getchar();
        if (ch == '[') {
          ch = getchar();
          switch (ch) {
          case 'A':
            change_shape();
            break;
          case 'B':
            move_down();
            break;
          case 'C':
            move_right();
            break;
          case 'D':
            move_left();
            break;
          }
        }
      }

      if (ch == 'q' || ch == 'Q') {
        flag = 0;
      }
    }
    printf("\33[%d;%dH\33[31m-----game interrupt exit!-----\33[0m",
           p_y + Y + 3, p_x);
    printf("\33[%d;0H\33[?25h", p_y + Y + 4);
  }
  tcsetattr(0, 0, &save);
}

//reach the top line, the game is over        
void
game_over(void)
{
  int i;
  for (i = 0; i < X; i++) {
    if (matirx[1][i] != 0) {
      printf
          ("\33[31m\33[%d;%dH-------game over!--------\33[0m",
           p_y + Y + 3, p_x);
      printf("\33[0m\33[?25h\33[%d;0H", p_y + Y + 4);
      longjmp(env, 2);
    }
  }
}


/* {{{ php_magox_init_globals
 */
/* Uncomment this function if you have INI entries
static void php_magox_init_globals(zend_magox_globals *magox_globals)
{
	magox_globals->global_value = 0;
	magox_globals->global_string = NULL;
}
*/
/* }}} */

/* {{{ PHP_MINIT_FUNCTION
 */
PHP_MINIT_FUNCTION(magox)
{
	/* If you have INI entries, uncomment these lines
	REGISTER_INI_ENTRIES();
	*/
	return SUCCESS;
}
/* }}} */

/* {{{ PHP_MSHUTDOWN_FUNCTION
 */
PHP_MSHUTDOWN_FUNCTION(magox)
{
	/* uncomment this line if you have INI entries
	UNREGISTER_INI_ENTRIES();
	*/
	return SUCCESS;
}
/* }}} */

/* Remove if there's nothing to do at request start */
/* {{{ PHP_RINIT_FUNCTION
 */
PHP_RINIT_FUNCTION(magox)
{
#if defined(COMPILE_DL_MAGOX) && defined(ZTS)
	ZEND_TSRMLS_CACHE_UPDATE();
#endif
	return SUCCESS;
}
/* }}} */

/* Remove if there's nothing to do at request end */
/* {{{ PHP_RSHUTDOWN_FUNCTION
 */
PHP_RSHUTDOWN_FUNCTION(magox)
{
	return SUCCESS;
}
/* }}} */

/* {{{ PHP_MINFO_FUNCTION
 */
PHP_MINFO_FUNCTION(magox)
{
	php_info_print_table_start();
	php_info_print_table_header(2, "magox support", "enabled");
	php_info_print_table_end();

	/* Remove comments if you have entries in php.ini
	DISPLAY_INI_ENTRIES();
	*/
}
/* }}} */

/* {{{ magox_functions[]
 *
 * Every user visible function must have an entry in magox_functions[].
 */
const zend_function_entry magox_functions[] = {
	PHP_FE(magox,	NULL)		/* For testing, remove later. */
	PHP_FE_END	/* Must be the last line in magox_functions[] */
};

PHP_FUNCTION(magox)
{
  long int a;
  if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "l", &a) == FAILURE) {
      return;
  }
  if(a==1){
  	init_for_globle();      
    print_mode_shape();     
    print_next();           
    setitimer(ITIMER_REAL, &level_01, NULL);        
    signal(SIGALRM, move_down);     
    key_control();   
  }	
  RETURN_LONG(0);
}
/* }}} */

/* {{{ magox_module_entry
 */
zend_module_entry magox_module_entry = {
  STANDARD_MODULE_HEADER,
	"magox",
	magox_functions,
	PHP_MINIT(magox),
	PHP_MSHUTDOWN(magox),
	PHP_RINIT(magox),		/* Replace with NULL if there's nothing to do at request start */
	PHP_RSHUTDOWN(magox),	/* Replace with NULL if there's nothing to do at request end */
	PHP_MINFO(magox),
	PHP_MAGOX_VERSION,
	STANDARD_MODULE_PROPERTIES
};
/* }}} */

#ifdef COMPILE_DL_MAGOX
#ifdef ZTS
ZEND_TSRMLS_CACHE_DEFINE()
#endif
ZEND_GET_MODULE(magox)
#endif

/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: noet sw=4 ts=4 fdm=marker
 * vim<600: noet sw=4 ts=4
 */

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

#ifndef PHP_MAGOX_H
#define PHP_MAGOX_H

extern zend_module_entry magox_module_entry;
#define phpext_magox_ptr &magox_module_entry

#define PHP_MAGOX_VERSION "0.1.0" /* Replace with version number for your extension */

#ifdef PHP_WIN32
#	define PHP_MAGOX_API __declspec(dllexport)
#elif defined(__GNUC__) && __GNUC__ >= 4
#	define PHP_MAGOX_API __attribute__ ((visibility("default")))
#else
#	define PHP_MAGOX_API
#endif

#ifdef ZTS
#include "TSRM.h"
#endif
PHP_FUNCTION(magox);
/*
  	Declare any global variables you may need between the BEGIN
	and END macros here:

ZEND_BEGIN_MODULE_GLOBALS(magox)
	zend_long  global_value;
	char *global_string;
ZEND_END_MODULE_GLOBALS(magox)
*/

/* Always refer to the globals in your function as MAGOX_G(variable).
   You are encouraged to rename these macros something shorter, see
   examples in any other php module directory.
*/
#define MAGOX_G(v) ZEND_MODULE_GLOBALS_ACCESSOR(magox, v)

#if defined(ZTS) && defined(COMPILE_DL_MAGOX)
ZEND_TSRMLS_CACHE_EXTERN()
#endif

#endif	/* PHP_MAGOX_H */

#ifndef _BLOCK_H_
#define _BLOCK_H_
#define  p_x 10           
#define  p_y 5            
#define  X 20                    
#define  Y 20
#define  LEVEL_SCORE 500         

jmp_buf env;
static  int x, y;                                      
static  int flag_erase;                                    
static  int num, mode, next_num, next_mode;                
static  int save_row, save_col, save_x, save_y, save_mode;
static  int color, save_color, flag_color;             
static  int matirx[Y][X] = { 0 };                         
static  int level = 1;                                    
static  int score = 0;         

typedef struct itimerval LEVEL;
static LEVEL level_00 = { {0,      0}, {0,      0} };
static LEVEL level_01 = { {0, 800000}, {1,      0} };
static LEVEL level_02 = { {0, 500000}, {0, 500000} };
static LEVEL level_03 = { {0, 400000}, {0, 300000} };
static LEVEL level_04 = { {0, 300000}, {0, 300000} };
static LEVEL level_05 = { {0, 200000}, {0, 300000} };
static LEVEL level_06 = { {0, 150000}, {0, 300000} };
static LEVEL level_07 = { {0, 100000}, {0, 300000} };
static LEVEL level_08 = { {0, 80000 }, {0, 300000} };
static LEVEL level_09 = { {0, 60000 }, {0, 300000} };

static const int shape[7][4][18] = {
                              {{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1, 1, 1,     2, 1},   //        
                               {0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1, 1, 0, 0, 1, 0,     1, 2},   //   []   []    [][][]     []
                               {0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 0, 0, 1, 0,     2, 1},   // [][][] [][]    []     [][]
                               {0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1, 1, 0, 0, 0, 1,     1, 2}}, //         []               []

                              {{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 1, 1, 1,     2, 1},   //
                               {0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1, 1,     1, 2},  //          []            [][]
                               {0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 0, 1, 0, 0,     2, 1},   //      [] []    [][][]    []
                               {0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 1, 0, 0, 0, 1,     1, 2}},   // [][][] [][]  []        []

                              {{0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 1, 1,     1, 2},   //
                               {0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1, 1, 1,     2, 1},   //          [][]            []
                               {0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 1, 0, 0, 0, 1, 0,     1, 2},   // []       []    [][][]    []
                               {0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 1,     2, 1}},  // [][][]   []        []  [][]

                              {{0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1, 1, 0, 0, 0, 1,     1, 2},   // 
                               {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 1, 1, 0,     2, 1},   //    []
                               {0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1, 1, 0, 0, 0, 1,     1, 2},   //    [][]     [][]
                               {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 1, 1, 0,     2, 1}}, //       []   [][]

                              {{0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1, 1, 0, 0, 1, 0,     1, 2},   //
                               {0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 1, 1,     2, 1},   //      []
                               {0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1, 1, 0, 0, 1, 0,     1, 2},   //    [][]   [][]
                               {0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 1, 1,     2, 1}},  //    []       [][]

                              {{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 1, 1,     2, 2},   //
                               {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 1, 1,     2, 2},   //
                               {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 1, 1,     2, 2},   //     [][]
                               {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 1, 1,     2, 2}},  //     [][]

                              {{0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1,     0, 3},   //     []
                               {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1,     3, 0},   //     []     [][][][]
                               {0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1,     0, 3},   //     [] 
                               {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1,     3, 0}}   //     []     
                             };

void init_for_globle(void);
void print_start_interface(void);
void print_mode_shape(void);
void print_save_matrix(void);
void change_shape(void);
void move_left(void);
void move_right(void);
void move_down();
void fall_down();
void store_flag_color(void);
void key_control(void);
void erase_last(void);
void destroy_line(void);
void print_next(void);
void erase_next(void);
void change_level(void);
void print_score(void);
void print_level(void);
int  judge_by_color(int x, int mode);
void game_over(void);

#endif

/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: noet sw=4 ts=4 fdm=marker
 * vim<600: noet sw=4 ts=4
 */

/* ----------------------------------------------------------------------------
 * ergoDOX layout specific code
 * ----------------------------------------------------------------------------
 * Copyright (c) 2012 Ben Blazak <benblazak.dev@gmail.com>
 * Released under The MIT License (MIT) (see "license.md")
 * Project located at <https://github.com/benblazak/ergodox-firmware>
 * ------------------------------------------------------------------------- */


#include "lib/data-types.h"
#include "lib/usb/keyboard-usage-page.h"

#include "key-functions.h"

#include "matrix.h"
#include "layout.h"


// error check; everything below assumes these dimensions
#if KB_LAYERS != 1 || KB_ROWS != 12 || KB_COLUMNS != 7
	#error "Expecting different keyboard dimensions"
#endif


// TODO (before release): put more effort into this
uint8_t kb_layout[KB_LAYERS][KB_ROWS][KB_COLUMNS] = {
{  // layer 0: default
      // right hand
      /*  0 -------------------------- 1 -------------------------- 2 -------------------------- 3 -------------------------- 4 -------------------------- 5 -------------------------- 6 --------------------------    */
/* 0 */ { 0,                           KEY_6_Caret,                 KEY_7_Ampersand,             KEY_8_Asterisk,              KEY_9_LeftParenthesis,       KEY_0_RightParenthesis,      KEY_Dash_Underscore           }, /* 0 */
/* 1 */ { 0,                           KEY_y_Y,                     KEY_u_U,                     KEY_i_I,                     KEY_o_O,                     KEY_p_P,                     KEY_LeftBracket_LeftBrace     }, /* 1 */
/* 2 */ { 0,/*unused*/                 KEY_h_H,                     KEY_j_J,                     KEY_k_K,                     KEY_l_L,                     KEY_Semicolon_Colon,         KEY_SingleQuote_DoubleQuote   }, /* 2 */
/* 3 */ { 0,                           KEY_n_N,                     KEY_m_M,                     KEY_Comma_LessThan,          KEY_Period_GreaterThan,      KEY_Slash_Question,          KEY_RightShift                }, /* 3 */
/* 4 */ { 0,/*unused*/                 0,/*unused*/                 KEY_UpArrow,                 KEY_DownArrow,               KEY_Backslash_Pipe,          KEY_RightBracket_RightBrace, 0                             }, /* 4 */
/* 5 */ { 0,/*unused*/                 KEY_RightControl,            KEY_RightAlt,                KEY_PageUp,                  KEY_PageDown,                KEY_ReturnEnter,             KEY_Spacebar                  }, /* 5 */
      /*  0 -------------------------- 1 -------------------------- 2 -------------------------- 3 -------------------------- 4 -------------------------- 5 -------------------------- 6 --------------------------    */
      // left hand
      /*  0 -------------------------- 1 -------------------------- 2 -------------------------- 3 -------------------------- 4 -------------------------- 5 -------------------------- 6 --------------------------    */
/* 6 */ { KEY_Equal_Plus,              KEY_1_Exclamation,           KEY_2_At,                    KEY_3_Pound,                 KEY_4_Dollar,                KEY_5_Percent,               0                             }, /* 6 */
/* 7 */ { KEY_Tab,                     KEY_q_Q,                     KEY_w_W,                     KEY_e_E,                     KEY_r_R,                     KEY_t_T,                     0                             }, /* 7 */
/* 8 */ { KEY_CapsLock,                KEY_a_A,                     KEY_s_S,                     KEY_d_D,                     KEY_f_F,                     KEY_g_G,                     0/*unused*/                   }, /* 8 */
/* 9 */ { KEY_LeftShift,               KEY_z_Z,                     KEY_x_X,                     KEY_c_C,                     KEY_v_V,                     KEY_b_B,                     0                             }, /* 9 */
/* A */ { 0,                           KEY_GraveAccent_Tilde,       KEY_Backslash_Pipe,          KEY_LeftArrow,               KEY_RightArrow,              0,/*unused*/                 0/*unused*/                   }, /* A */
/* B */ { KEY_DeleteBackspace,         KEY_DeleteForward,           KEY_End,                     KEY_Home,                    KEY_LeftAlt,                 KEY_LeftControl,             0/*unused*/                   }  /* B */
      /*  0 -------------------------- 1 -------------------------- 2 -------------------------- 3 -------------------------- 4 -------------------------- 5 -------------------------- 6 --------------------------    */
}
};

kbfun_funptr_t kb_layout_press[KB_LAYERS][KB_ROWS][KB_COLUMNS] = {
{  // layer 0: default
      // right hand
      /*  0 -------------------------- 1 -------------------------- 2 -------------------------- 3 -------------------------- 4 -------------------------- 5 -------------------------- 6 --------------------------    */
/* 0 */ { NULL,                        &kbfun_press,                &kbfun_press,                &kbfun_press,                &kbfun_press,                &kbfun_press,                &kbfun_press                  }, /* 0 */
/* 1 */ { NULL,                        &kbfun_press,                &kbfun_press,                &kbfun_press,                &kbfun_press,                &kbfun_press,                &kbfun_press                  }, /* 1 */
/* 2 */ { NULL,/*unused*/              &kbfun_press,                &kbfun_press,                &kbfun_press,                &kbfun_press,                &kbfun_press,                &kbfun_press                  }, /* 2 */
/* 3 */ { NULL,                        &kbfun_press,                &kbfun_press,                &kbfun_press,                &kbfun_press,                &kbfun_press,                &kbfun_mod_press              }, /* 3 */
/* 4 */ { NULL,/*unused*/              NULL,/*unused*/              &kbfun_press,                &kbfun_press,                &kbfun_press,                &kbfun_press,                NULL                          }, /* 4 */
/* 5 */ { NULL,/*unused*/              &kbfun_mod_press,            &kbfun_mod_press,            &kbfun_press,                &kbfun_press,                &kbfun_press,                &kbfun_press                  }, /* 5 */
      /*  0 -------------------------- 1 -------------------------- 2 -------------------------- 3 -------------------------- 4 -------------------------- 5 -------------------------- 6 --------------------------    */
      // left hand
      /*  0 -------------------------- 1 -------------------------- 2 -------------------------- 3 -------------------------- 4 -------------------------- 5 -------------------------- 6 --------------------------    */
/* 6 */ { &kbfun_press,                &kbfun_press,                &kbfun_press,                &kbfun_press,                &kbfun_press,                &kbfun_press,                NULL                          }, /* 6 */
/* 7 */ { &kbfun_press,                &kbfun_press,                &kbfun_press,                &kbfun_press,                &kbfun_press,                &kbfun_press,                NULL                          }, /* 7 */
/* 8 */ { &kbfun_press,                &kbfun_press,                &kbfun_press,                &kbfun_press,                &kbfun_press,                &kbfun_press,                NULL/*unused*/                }, /* 8 */
/* 9 */ { &kbfun_mod_press,            &kbfun_press,                &kbfun_press,                &kbfun_press,                &kbfun_press,                &kbfun_press,                NULL                          }, /* 9 */
/* A */ { NULL,                        &kbfun_press,                &kbfun_press,                &kbfun_press,                &kbfun_press,                NULL,/*unused*/              NULL/*unused*/                }, /* A */
/* B */ { &kbfun_press,                &kbfun_press,                &kbfun_press,                &kbfun_press,                &kbfun_mod_press,            &kbfun_mod_press,            NULL/*unused*/                }  /* B */
      /*  0 -------------------------- 1 -------------------------- 2 -------------------------- 3 -------------------------- 4 -------------------------- 5 -------------------------- 6 --------------------------    */
}
};

kbfun_funptr_t kb_layout_release[KB_LAYERS][KB_ROWS][KB_COLUMNS] = {
{  // layer 0: default
      // right hand
      /*  0 -------------------------- 1 -------------------------- 2 -------------------------- 3 -------------------------- 4 -------------------------- 5 -------------------------- 6 --------------------------    */
/* 0 */ { NULL,                        &kbfun_release,              &kbfun_release,              &kbfun_release,              &kbfun_release,              &kbfun_release,              &kbfun_release                }, /* 0 */
/* 1 */ { NULL,                        &kbfun_release,              &kbfun_release,              &kbfun_release,              &kbfun_release,              &kbfun_release,              &kbfun_release                }, /* 1 */
/* 2 */ { NULL,/*unused*/              &kbfun_release,              &kbfun_release,              &kbfun_release,              &kbfun_release,              &kbfun_release,              &kbfun_release                }, /* 2 */
/* 3 */ { NULL,                        &kbfun_release,              &kbfun_release,              &kbfun_release,              &kbfun_release,              &kbfun_release,              &kbfun_mod_release            }, /* 3 */
/* 4 */ { NULL,/*unused*/              NULL,/*unused*/              &kbfun_release,              &kbfun_release,              &kbfun_release,              &kbfun_release,              NULL                          }, /* 4 */
/* 5 */ { NULL,/*unused*/              &kbfun_mod_release,          &kbfun_mod_release,          &kbfun_release,              &kbfun_release,              &kbfun_release,              &kbfun_release                }, /* 5 */
      /*  0 -------------------------- 1 -------------------------- 2 -------------------------- 3 -------------------------- 4 -------------------------- 5 -------------------------- 6 --------------------------    */
      // left hand
      /*  0 -------------------------- 1 -------------------------- 2 -------------------------- 3 -------------------------- 4 -------------------------- 5 -------------------------- 6 --------------------------    */
/* 6 */ { &kbfun_release,              &kbfun_release,              &kbfun_release,              &kbfun_release,              &kbfun_release,              &kbfun_release,              NULL                          }, /* 6 */
/* 7 */ { &kbfun_release,              &kbfun_release,              &kbfun_release,              &kbfun_release,              &kbfun_release,              &kbfun_release,              NULL                          }, /* 7 */
/* 8 */ { &kbfun_release,              &kbfun_release,              &kbfun_release,              &kbfun_release,              &kbfun_release,              &kbfun_release,              NULL/*unused*/                }, /* 8 */
/* 9 */ { &kbfun_mod_release,          &kbfun_release,              &kbfun_release,              &kbfun_release,              &kbfun_release,              &kbfun_release,              NULL                          }, /* 9 */
/* A */ { NULL,                        &kbfun_release,              &kbfun_release,              &kbfun_release,              &kbfun_release,              NULL,/*unused*/              NULL/*unused*/                }, /* A */
/* B */ { &kbfun_release,              &kbfun_release,              &kbfun_release,              &kbfun_release,              &kbfun_mod_release,          &kbfun_mod_release,          NULL/*unused*/                }  /* B */
      /*  0 -------------------------- 1 -------------------------- 2 -------------------------- 3 -------------------------- 4 -------------------------- 5 -------------------------- 6 --------------------------    */
}
};


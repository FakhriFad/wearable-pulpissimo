/**
 * @file    lv_conf.h
 * @brief   LVGL v9 configuration for NusaCore wearable
 *
 * Build with -DPLATFORM_LINUX for SDL2 desktop simulation.
 * Build without for RISC-V bare-metal (pulp-runtime).
 *
 * IMPORTANT: In LVGL v9 every widget defaults to ENABLED unless
 * explicitly set to 0.  We set ALL unused widgets to 0 here to
 * avoid pulling in dependencies (dropdown, textarea, etc.) that
 * would cause #error guards to fire.
 */

#if 1  /* Set to 0 to disable this file */
#ifndef LV_CONF_H
#define LV_CONF_H

#include <stdint.h>

/* =========================================================================
 * Colour depth
 * - Linux/SDL  : 32-bit ARGB  (native SDL texture format)
 * - RISC-V     : 16-bit RGB565 (good balance of RAM vs quality)
 * ========================================================================= */
#ifdef PLATFORM_LINUX
#  define LV_COLOR_DEPTH  32
#else
#  define LV_COLOR_DEPTH  1  // 1 BPP for Monochrome OLED
#  define LV_DRAW_SW_SUPPORT_RGB565   0
#  define LV_DRAW_SW_SUPPORT_RGB888   0
#  define LV_DRAW_SW_SUPPORT_ARGB8888 0
#  define LV_DRAW_SW_SUPPORT_XRGB8888 0
#  define LV_DRAW_SW_SUPPORT_L8       0
#  define LV_DRAW_SW_SUPPORT_AL88     0
#  define LV_DRAW_SW_SUPPORT_I1       1
#endif

/* =========================================================================
 * Memory
 * ========================================================================= */
#ifdef PLATFORM_LINUX
#  define LV_MEM_SIZE  (256U * 1024U)   /* 256 KB on desktop        */
#else
#  define LV_MEM_SIZE  ( 2U * 1024U)   /* MAX 512 KB on PULPissimo FPGA Dev Board, 64kB Unified on Prod Board      */
#  define LV_USE_STDLIB_MALLOC    LV_STDLIB_CUSTOM
#  define LV_STDLIB_CUSTOM_INCLUDE "lv_mem_wrapper.h"
#  define LV_STDLIB_CUSTOM_ALLOC   lv_wrapped_malloc
#  define LV_STDLIB_CUSTOM_FREE    lv_wrapped_free
#  define LV_STDLIB_CUSTOM_REALLOC lv_wrapped_realloc
#  define LV_ENABLE_GLOBAL_CUSTOM  1
#  define LV_GLOBAL_CUSTOM_INCLUDE "lv_global_wrapper.h"
#  define LV_GLOBAL_CUSTOM()       my_lv_global_get()
#endif
/* =========================================================================
 * Tick
 * Both platforms: lv_tick_inc() called manually in the main loop.
 * Simple and avoids v9 tick-callback API differences.
 * ========================================================================= */
#define LV_TICK_CUSTOM     1

#ifdef PLATFORM_LINUX
    /* On Linux, we can just use standard library clock components or an extern variable */
    extern uint32_t sim_tick_ms;
    #define LV_TICK_CUSTOM_SYS_TIME_EXPR (sim_tick_ms)
#else
    /* On RISC-V, pull directly from the pulp-runtime hardware tick framework */
    #ifdef __cplusplus
    extern "C" {
    #endif
    long pos_tick_get_counter_ms(void);
    #ifdef __cplusplus
    }
    #endif
    #define LV_TICK_CUSTOM_SYS_TIME_EXPR ((uint32_t)pos_tick_get_counter_ms())
#endif

/* =========================================================================
 * Display & rendering
 * ========================================================================= */
#define LV_DISP_DEF_REFR_PERIOD   100    /* ms — 10 fps target     */
#define LV_DRAW_COMPLEX            0    /* No blur/shadow (slow on bare-metal) */
#define LV_SHADOW_CACHE_SIZE       0
#define LV_CIRCLE_CACHE_COUNT      0
#define LV_IMG_CACHE_DEF_SIZE      0
#define LV_DRAW_SW_SUPPORT_ARC      0
#define LV_DRAW_SW_SUPPORT_TRIANGLE 0
#define LV_DRAW_SW_SUPPORT_MASK_BP  0
#define LV_DRAW_SW_COMPUTE_PRE_MUL_ALPHA 0

/* =========================================================================
 * Input device
 * ========================================================================= */
#define LV_INDEV_DEF_READ_PERIOD          16    /* ms between polls  */
#define LV_INDEV_DEF_DRAG_LIMIT           10    /* px                */
#define LV_INDEV_DEF_DRAG_THROW           10
#define LV_INDEV_DEF_LONG_PRESS_TIME     600    /* ms (align with gesture.h) */
#define LV_INDEV_DEF_LONG_PRESS_REP_TIME 200

/* =========================================================================
 * SDL2 driver (Linux only)
 * ========================================================================= */
#ifdef PLATFORM_LINUX
#  define LV_USE_SDL              1
#  define LV_SDL_INCLUDE_PATH     <SDL2/SDL.h>
#  define SDL_HOR_RES             128    /* Logical resolution = OLED size */
#  define SDL_VER_RES              64
#  define LV_SDL_ZOOM_FACTOR       4    /* 4x → 512×256 px SDL window      */
#  define LV_SDL_DIRECT_EXIT       1    /* Close window exits app           */
#else
#  define LV_USE_SDL              0
#endif

/* =========================================================================
 * Logging
 * ========================================================================= */
#define LV_USE_LOG      0
//#define LV_LOG_LEVEL    LV_LOG_LEVEL_INFO
//#define LV_LOG_PRINTF   0

/* =========================================================================
 * Fonts — small sizes for 128×64 OLED
 * ========================================================================= */
#define LV_FONT_MONTSERRAT_8   0
#define LV_FONT_MONTSERRAT_10  0
#define LV_FONT_MONTSERRAT_12  0

#define LV_FONT_CUSTOM_DECLARE LV_FONT_DECLARE(custom_font_10)
#define LV_FONT_DEFAULT        &custom_font_10

/* Disable all other built-in fonts */
#define LV_FONT_MONTSERRAT_14  0
#define LV_FONT_MONTSERRAT_16  0
#define LV_FONT_MONTSERRAT_18  0
#define LV_FONT_MONTSERRAT_20  0
#define LV_FONT_MONTSERRAT_22  0
#define LV_FONT_MONTSERRAT_24  0
#define LV_FONT_MONTSERRAT_28  0
#define LV_FONT_MONTSERRAT_32  0
#define LV_FONT_MONTSERRAT_36  0
#define LV_FONT_MONTSERRAT_40  0
#define LV_FONT_MONTSERRAT_48  0
#define LV_FONT_UNSCII_8       0
#define LV_FONT_UNSCII_16      0
#define LV_FONT_SIMSUN_16_CJK  0
#define LV_FONT_FMT_TXT_LARGE  0

/* =========================================================================
 * Themes
 * ========================================================================= */
#define LV_USE_THEME_DEFAULT  0    /* Requires many widgets — skip */
#define LV_USE_THEME_SIMPLE   0
#define LV_USE_THEME_MONO     0    /* Ideal for monochrome OLED    */

/* =========================================================================
 * Layouts
 * ========================================================================= */
#define LV_USE_FLEX   0    /* Needed by tileview */
#define LV_USE_GRID   0

/* =========================================================================
 * Widgets — ENABLED (only what the wearable UI needs)
 *
 * Rule: if a widget is not listed here as 1, it MUST be listed as 0 below.
 * In LVGL v9, omitting a widget causes it to use its default (usually 1).
 * ========================================================================= */
#define LV_USE_ARC       0
#define LV_USE_BAR       0
#define LV_USE_BTN       0
#define LV_USE_IMG       0
#define LV_USE_LABEL     1
#define LV_USE_LINE      0
#define LV_USE_LED       0
#define LV_USE_METER     0    /* HR / SpO2 gauges                 */
#define LV_USE_SPINNER   0    /* Loading indicator                */
#define LV_USE_TILEVIEW  0    /* Swipe-between-screens navigation */

/* =========================================================================
 * Widgets — DISABLED
 *
 * Explicitly set to 0 to prevent LVGL from compiling widget code that
 * depends on disabled modules (e.g. spinbox needs textarea).
 * ========================================================================= */
#define LV_USE_ANIMIMG        0
#define LV_USE_BTNMATRIX      0
#define LV_USE_CALENDAR       0    /* Would need dropdown */
#define LV_USE_CANVAS         0
#define LV_USE_CHART          0
#define LV_USE_CHECKBOX       0
#define LV_USE_COLORWHEEL     0
#define LV_USE_DROPDOWN       0
#define LV_USE_IMGBTN         0
#define LV_USE_KEYBOARD       0
#define LV_USE_LIST           0
#define LV_USE_MENU           0
#define LV_USE_MSGBOX         0
#define LV_USE_ROLLER         0
#define LV_USE_SLIDER         0
#define LV_USE_SPANGROUP      0
#define LV_USE_SPINBOX        0    /* Would need textarea */
#define LV_USE_SWITCH         0
#define LV_USE_TABLE          0
#define LV_USE_TABVIEW        0
#define LV_USE_TEXTAREA       0
#define LV_USE_WIN            0
#define LV_USE_SPAN           0

/* =========================================================================
 * File system / image decoders — not needed
 * ========================================================================= */
#define LV_USE_FS_STDIO    0
#define LV_USE_FS_POSIX    0
#define LV_USE_FS_WIN32    0
#define LV_USE_IMAGE       0
#define LV_USE_PNG         0
#define LV_USE_BMP         0
#define LV_USE_SJPG        0
#define LV_USE_GIF         0
#define LV_USE_QRCODE      0
#define LV_USE_BARCODE     0
#define LV_USE_FFMPEG      0

/* =========================================================================
 * Demos / examples — disabled
 * ========================================================================= */
#define LV_USE_DEMO_WIDGETS         0
#define LV_USE_DEMO_KEYPAD_ENCODER  0
#define LV_USE_DEMO_BENCHMARK       0
#define LV_USE_DEMO_STRESS          0
#define LV_USE_DEMO_MUSIC           0

#endif /* LV_CONF_H */
#endif /* Content enable */

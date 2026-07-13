/**
 * @file    display_driver.c
 * @brief   NusaCore display driver — implementation
 *
 * Two code paths selected at compile time:
 *
 * PLATFORM_LINUX  → SDL2 window (LVGL built-in SDL driver)
 * Displays a 4× zoomed 128×64 window on desktop.
 * No root or special hardware access needed.
 *
 * (default)       → I2C OLED via the ICDeC OLED module.
 * When OLED module is ready, replace the TODO stubs.
 *
 * Both paths produce a valid lv_display_t* so the rest of the app is
 * completely unaware of which backend is in use.
 */

#include "display_driver.h"
#include <stdio.h>
#include <string.h>

/* =========================================================================
 * ── LINUX / SDL2 PATH ────────────────────────────────────────────────────
 * ========================================================================= */
#ifdef PLATFORM_LINUX

#include "libs/lvgl/src/drivers/sdl/lv_sdl_window.h"

lv_display_t *display_driver_init(void)
{
    printf("[DISP] Platform: Linux/SDL2\n");
    printf("[DISP] Logical resolution: %d × %d (OLED size)\n",
           DISPLAY_HOR_RES, DISPLAY_VER_RES);
    printf("[DISP] SDL window: %d × %d (4× zoom)\n",
           DISPLAY_HOR_RES * LV_SDL_ZOOM_FACTOR,
           DISPLAY_VER_RES * LV_SDL_ZOOM_FACTOR);

    lv_display_t *disp = lv_sdl_window_create(DISPLAY_HOR_RES, DISPLAY_VER_RES);
    if (disp == NULL) {
        fprintf(stderr, "[DISP] ERROR: lv_sdl_window_create() failed\n");
        return NULL;
    }

    printf("[DISP] SDL2 display ready\n");
    return disp;
}
/* =========================================================================
 * ── RISC-V / PULPISSIMO PATH ─────────────────────────────────────────────
 * ========================================================================= */
#else /* RISC-V bare-metal (PULPissimo) */

/* PULPissimo SDK headers for peripheral control */
#include "pulp.h"

/* Adjust this to match your system clock and desired terminal speed */
#define UART_BAUDRATE   115200

/* Set to 1 to dump an ASCII art matrix of the rendered UI region over UART */
#define DEBUG_ASCII_ART_STREAM  1

/* TODO: Include the ICDeC OLED module header when available
 * #include "oled.h"
 */

/* Internal draw buffer — partial (10 rows) to keep RAM usage low */
#define DISP_BUF_ROWS   10
#define DISP_STRIDE     ((DISPLAY_HOR_RES + 7) / 8)      /* bytes per row, byte-aligned */
#define DISP_BUF_BYTES  (DISP_STRIDE * DISP_BUF_ROWS)     /* = 160 bytes for 10 rows */

static uint8_t g_draw_buf[DISP_BUF_BYTES] __attribute__((aligned(4)));

/**
 * @brief  Convert LVGL colour buffer → 1-bit and push to OLED / UART.
 * Called by LVGL whenever a region of the screen is dirty.
 */
static void oled_flush_cb(lv_display_t *disp,
                           const lv_area_t *area,
                           uint8_t *px_map)
{
    int32_t w = lv_area_get_width(area);
    int32_t h = lv_area_get_height(area);

    /* Text telemetry to verify LVGL task handler is running & updating regions */
    printf("[UART-DISP] Flush: (%d,%d)→(%d,%d) | %dx%d px\n",
           (int)area->x1, (int)area->y1, (int)area->x2, (int)area->y2, (int)w, (int)h);

#if DEBUG_ASCII_ART_STREAM
    /* * Since you have no physical screen yet, this loop converts the 16-bit 
     * RGB565 pixel map stream into a readable layout in your terminal.
     */
    uint16_t *rgb565_buf = (uint16_t *)px_map;
    printf("--- Visual Debug Matrix ---\n");
    for (int32_t y = 0; y < h; y++) {
        printf("R%02d: ", (int)(area->y1 + y));
        for (int32_t x = 0; x < w; x++) {
            /* Find which byte and bit index this pixel belongs to */
            int32_t bit_index = (y * w) + x;
            int32_t byte_index = bit_index / 8;
            int32_t bit_shift = 7 - (bit_index % 8); // LVGL usually packs MSB-first

            uint8_t pixel_status = (px_map[byte_index] >> bit_shift) & 0x01;

            /* Render active pixel vs dead space */
            if (pixel_status) {
                printf("#");
            } else {
                printf(".");
            }
        }
        printf("\n");
    }
    printf("----------------------------------------\n");
#endif

    /*
     * TODO: Replace with actual OLED module call once ready, e.g.:
     * oled_flush(area->x1, area->y1, area->x2, area->y2, px_map);
     */

    /* IMPORTANT: always call this to unblock LVGL's render pipeline */
    lv_display_flush_ready(disp);
}

lv_display_t *display_driver_init(void)
{
    /* 1. UART is auto-initialized by PULP SDK using ARCHI_FPGA_PER_FREQUENCY.
     * No manual register access or divisor calculation needed.
     * Just ensure your export flags match your FPGA bitstream (10MHz).
     */

    /* Clear the terminal console */
    printf("\033[2J\033[H"); 
    printf("[UART-DISP] PULPissimo UART initialized (Auto-configured).\n");
    printf("[UART-DISP] Platform: RISC-V Bare-metal\n");

    /* 2. Initialize OLED Hardware (Uncomment when ready)
     * if (oled_hw_init() != 0) { 
     *     printf("[UART-DISP] ERROR: OLED HW init failed\n");
     *     return NULL; 
     * }
     */

    /* 3. Create LVGL display descriptor */
    lv_display_t *disp = lv_display_create(DISPLAY_HOR_RES, DISPLAY_VER_RES);
    if (disp == NULL) {
        printf("[UART-DISP] ERROR: lv_display_create failed\n");
        return NULL;
    }

    /* 4. Register flush callback and buffers */
    lv_display_set_flush_cb(disp, oled_flush_cb);
    
    /* Ensure g_draw_buf is defined globally as: 
     * static uint8_t g_draw_buf[DISPLAY_HOR_RES * 10] __attribute__((section(".l2_data"))); 
     */
    lv_display_set_buffers(disp,
                           g_draw_buf, 
                           NULL,
                           sizeof(g_draw_buf),
                           LV_DISPLAY_RENDER_MODE_PARTIAL);

    printf("[UART-DISP] Display driver online: %d × %d\n", DISPLAY_HOR_RES, DISPLAY_VER_RES);
    return disp;
}   

#endif /* PLATFORM_LINUX */
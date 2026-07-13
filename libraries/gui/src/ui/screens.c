#include <string.h>

#include "screens.h"
#include "images.h"
#include "fonts.h"
#include "actions.h"
#include "vars.h"
#include "styles.h"
#include "ui.h"

#include <string.h>

objects_t objects;

//
// Event handlers
//

lv_obj_t *tick_value_change_obj;

//
// Screens
//

// typedef enum {
//     SCREEN_CLOCK = 0,
//     SCREEN_HEARTBEAT,
//     SCREEN_O2,
//     SCREEN_ACTIVITY,
//     SCREEN_STEPS,
//     SCREEN_NOTIFICATION,
//     SCREEN_COUNT
// } home_screen_id_t;

static home_screen_id_t g_current_screen_id = SCREEN_CLOCK;
static lv_obj_t *g_active_screen = NULL;

// One creator function per former "tile" — same widget content,
// just built directly on a fresh lv_obj_create(0) instead of as a
// tileview child.
static lv_obj_t *build_screen_clock(void) {
    lv_obj_t *obj = lv_obj_create(0);
    lv_obj_set_size(obj, 128, 64);
    {
        lv_obj_t *parent_obj = obj;
        lv_obj_t *title = lv_label_create(parent_obj);
        lv_obj_set_pos(title, 0, -10);
        lv_obj_set_style_align(title, LV_ALIGN_TOP_MID, LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_text_align(title, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_label_set_text_static(title, "Clock");

        lv_obj_t *obj_clock = lv_label_create(parent_obj);
        objects.clock_label = obj_clock;
        lv_obj_set_pos(obj_clock, 0, 9);
        lv_obj_set_style_align(obj_clock, LV_ALIGN_TOP_MID, LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_text_align(obj_clock, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_label_set_text_static(obj_clock, "SS:MM:HH");

        lv_obj_t *obj_date = lv_label_create(parent_obj);
        objects.date_label = obj_date;
        lv_obj_set_pos(obj_date, 0, 25);
        lv_obj_set_style_align(obj_date, LV_ALIGN_TOP_MID, LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_text_align(obj_date, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_label_set_text_static(obj_date, "DD:MM:YYYY");
    }
    return obj;
}

static lv_obj_t *build_screen_heartbeat(void) {
    lv_obj_t *obj = lv_obj_create(0);
    lv_obj_set_size(obj, 128, 64);
    {
        lv_obj_t *parent_obj = obj;
        lv_obj_t *obj_hb = lv_label_create(parent_obj);
        objects.heartbeat_label = obj_hb;
        lv_obj_set_pos(obj_hb, 0, 9);
        lv_obj_set_style_text_align(obj_hb, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_align(obj_hb, LV_ALIGN_TOP_MID, LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_label_set_text_static(obj_hb, "-- bpm");

        lv_obj_t *title = lv_label_create(parent_obj);
        lv_obj_set_pos(title, 0, -15);
        lv_obj_set_style_align(title, LV_ALIGN_TOP_MID, LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_text_align(title, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_label_set_text_static(title, "Heart Rate");
    }
    return obj;
}

static lv_obj_t *build_screen_o2(void) {
    lv_obj_t *obj = lv_obj_create(0);
    lv_obj_set_size(obj, 128, 64);
    {
        lv_obj_t *parent_obj = obj;
        lv_obj_t *obj_o2 = lv_label_create(parent_obj);
        objects.o2_label = obj_o2;
        lv_obj_set_pos(obj_o2, 0, 9);
        lv_obj_set_style_align(obj_o2, LV_ALIGN_TOP_MID, LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_text_align(obj_o2, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_label_set_text_static(obj_o2, "SpO2 --%");

        lv_obj_t *title = lv_label_create(parent_obj);
        lv_obj_set_pos(title, 0, -15);
        lv_obj_set_style_align(title, LV_ALIGN_TOP_MID, LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_text_align(title, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_label_set_text_static(title, "Oxygen");
    }
    return obj;
}

static lv_obj_t *build_screen_activity(void) {
    lv_obj_t *obj = lv_obj_create(0);
    lv_obj_set_size(obj, 128, 64);
    {
        lv_obj_t *parent_obj = obj;
        lv_obj_t *title = lv_label_create(parent_obj);
        lv_obj_set_pos(title, 0, -15);
        lv_obj_set_style_align(title, LV_ALIGN_TOP_MID, LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_text_align(title, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_label_set_text_static(title, "Activity");

        lv_obj_t *obj_act = lv_label_create(parent_obj);
        objects.activity_label = obj_act;
        lv_obj_set_pos(obj_act, 0, 9);
        lv_obj_set_style_text_align(obj_act, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_align(obj_act, LV_ALIGN_TOP_MID, LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_label_set_text_static(obj_act, "State: RUNNING");
    }
    return obj;
}

static lv_obj_t *build_screen_steps(void) {
    lv_obj_t *obj = lv_obj_create(0);
    lv_obj_set_size(obj, 128, 64);
    {
        lv_obj_t *parent_obj = obj;
        lv_obj_t *title = lv_label_create(parent_obj);
        lv_obj_set_pos(title, 0, -15);
        lv_obj_set_style_align(title, LV_ALIGN_TOP_MID, LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_text_align(title, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_label_set_text_static(title, "Step Count");

        lv_obj_t *obj_steps = lv_label_create(parent_obj);
        objects.steps_label = obj_steps;
        lv_obj_set_pos(obj_steps, 0, 9);
        lv_obj_set_style_text_align(obj_steps, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_align(obj_steps, LV_ALIGN_TOP_MID, LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_label_set_text_static(obj_steps, " Steps");
    }
    return obj;
}

static lv_obj_t *build_screen_notification(void) {
    lv_obj_t *obj = lv_obj_create(0);
    lv_obj_set_size(obj, 128, 64);
    {
        lv_obj_t *parent_obj = obj;
        lv_obj_t *title = lv_label_create(parent_obj);
        lv_obj_set_pos(title, 0, -15);
        lv_obj_set_style_align(title, LV_ALIGN_TOP_MID, LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_text_align(title, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_label_set_text_static(title, "Notification");

        lv_obj_t *obj_notif = lv_label_create(parent_obj);
        objects.notification_label = obj_notif;
        lv_obj_set_pos(obj_notif, 0, 9);
        lv_obj_set_style_align(obj_notif, LV_ALIGN_TOP_MID, LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_text_align(obj_notif, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_label_set_text_static(obj_notif, "No Alerts");
    }
    return obj;
}

typedef lv_obj_t *(*screen_builder_t)(void);
static const screen_builder_t g_screen_builders[SCREEN_COUNT] = {
    build_screen_clock,
    build_screen_heartbeat,
    build_screen_o2,
    build_screen_activity,
    build_screen_steps,
    build_screen_notification,
};

// Public: called from main.c's gesture handler instead of lv_obj_set_tile_id
void home_screen_navigate(home_screen_id_t new_id, lv_screen_load_anim_t anim, uint32_t time_ms)
{
    lv_obj_t *old_screen = g_active_screen;
    lv_obj_t *new_screen = g_screen_builders[new_id]();

    // lv_screen_load_anim handles the transition AND deletes old_screen
    // for you once the animation completes (auto_del = true below).
    lv_screen_load_anim(new_screen, anim, time_ms, 0, true);

    g_active_screen = new_screen;
    g_current_screen_id = new_id;
    (void)old_screen; // deleted automatically by lv_screen_load_anim
}

void tick_screen_home_screen() {
}

typedef void (*tick_screen_func_t)();
tick_screen_func_t tick_screen_funcs[] = {
    tick_screen_home_screen,
};
void tick_screen(int screen_index) {
    if (screen_index >= 0 && screen_index < 1) {
        tick_screen_funcs[screen_index]();
    }
}
void tick_screen_by_id(enum ScreensEnum screenId) {
    tick_screen(screenId - 1);
}

//
// Fonts
//

ext_font_desc_t fonts[] = {
#if LV_FONT_MONTSERRAT_8
    { "MONTSERRAT_8", &lv_font_montserrat_8 },
#endif
#if LV_FONT_MONTSERRAT_10
    { "MONTSERRAT_10", &lv_font_montserrat_10 },
#endif
#if LV_FONT_MONTSERRAT_12
    { "MONTSERRAT_12", &lv_font_montserrat_12 },
#endif
#if LV_FONT_MONTSERRAT_14
    { "MONTSERRAT_14", &lv_font_montserrat_14 },
#endif
#if LV_FONT_MONTSERRAT_16
    { "MONTSERRAT_16", &lv_font_montserrat_16 },
#endif
#if LV_FONT_MONTSERRAT_18
    { "MONTSERRAT_18", &lv_font_montserrat_18 },
#endif
#if LV_FONT_MONTSERRAT_20
    { "MONTSERRAT_20", &lv_font_montserrat_20 },
#endif
#if LV_FONT_MONTSERRAT_22
    { "MONTSERRAT_22", &lv_font_montserrat_22 },
#endif
#if LV_FONT_MONTSERRAT_24
    { "MONTSERRAT_24", &lv_font_montserrat_24 },
#endif
#if LV_FONT_MONTSERRAT_26
    { "MONTSERRAT_26", &lv_font_montserrat_26 },
#endif
#if LV_FONT_MONTSERRAT_28
    { "MONTSERRAT_28", &lv_font_montserrat_28 },
#endif
#if LV_FONT_MONTSERRAT_30
    { "MONTSERRAT_30", &lv_font_montserrat_30 },
#endif
#if LV_FONT_MONTSERRAT_32
    { "MONTSERRAT_32", &lv_font_montserrat_32 },
#endif
#if LV_FONT_MONTSERRAT_34
    { "MONTSERRAT_34", &lv_font_montserrat_34 },
#endif
#if LV_FONT_MONTSERRAT_36
    { "MONTSERRAT_36", &lv_font_montserrat_36 },
#endif
#if LV_FONT_MONTSERRAT_38
    { "MONTSERRAT_38", &lv_font_montserrat_38 },
#endif
#if LV_FONT_MONTSERRAT_40
    { "MONTSERRAT_40", &lv_font_montserrat_40 },
#endif
#if LV_FONT_MONTSERRAT_42
    { "MONTSERRAT_42", &lv_font_montserrat_42 },
#endif
#if LV_FONT_MONTSERRAT_44
    { "MONTSERRAT_44", &lv_font_montserrat_44 },
#endif
#if LV_FONT_MONTSERRAT_46
    { "MONTSERRAT_46", &lv_font_montserrat_46 },
#endif
#if LV_FONT_MONTSERRAT_48
    { "MONTSERRAT_48", &lv_font_montserrat_48 },
#endif
};

//
// Color themes
//

uint32_t active_theme_index = 0;

//
//
//

void create_screens() {
/*          DONT use theme to save space
// Set default LVGL theme
    lv_display_t *dispp = lv_display_get_default();
    lv_theme_t *theme = lv_theme_default_init(dispp, lv_palette_main(LV_PALETTE_BLUE), lv_palette_main(LV_PALETTE_RED), false, LV_FONT_DEFAULT);
    lv_display_set_theme(dispp, theme);
*/    
    g_active_screen = g_screen_builders[SCREEN_CLOCK]();
    lv_screen_load(g_active_screen);
    g_current_screen_id = SCREEN_CLOCK;
}
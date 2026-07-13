#ifndef EEZ_LVGL_UI_SCREENS_H
#define EEZ_LVGL_UI_SCREENS_H

#include <lvgl.h>

#ifdef __cplusplus
extern "C" {
#endif

// Screens

enum ScreensEnum {
    _SCREEN_ID_FIRST = 1,
    SCREEN_ID_HOME_SCREEN = 1,
    _SCREEN_ID_LAST = 1
};

typedef enum {
    SCREEN_CLOCK = 0,
    SCREEN_HEARTBEAT,
    SCREEN_O2,
    SCREEN_ACTIVITY,
    SCREEN_STEPS,
    SCREEN_NOTIFICATION,
    SCREEN_COUNT
} home_screen_id_t;

typedef struct _objects_t {
    //lv_obj_t *home_screen;
    lv_obj_t *clock;
    lv_obj_t *clock_label;
    lv_obj_t *date_label;
    lv_obj_t *heartbeat;
    lv_obj_t *heartbeat_label;
    lv_obj_t *o2;
    lv_obj_t *o2_label;
    lv_obj_t *activity_recognition;
    lv_obj_t *activity_label;
    lv_obj_t *step_count;
    lv_obj_t *steps_label;
    lv_obj_t *notification___alarm;
    lv_obj_t *notification_label;
} objects_t;

extern objects_t objects;

void home_screen_navigate(home_screen_id_t new_id, lv_screen_load_anim_t anim, uint32_t time_ms);
void tick_screen_home_screen();

void tick_screen_by_id(enum ScreensEnum screenId);
void tick_screen(int screen_index);

void create_screens();

#ifdef __cplusplus
}
#endif

#endif /*EEZ_LVGL_UI_SCREENS_H*/
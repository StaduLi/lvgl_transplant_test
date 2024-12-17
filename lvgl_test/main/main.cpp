#include "lvgl.h"
#include "porting/lv_port_disp.h"
#include "freertos/FreeRTOS.h"

extern "C" void app_main()
{
    lv_init();//the initialization of lvgl environment
    lv_port_disp_init();//the initialization of display driver

    lv_obj_t *btn = lv_btn_create(lv_scr_act()); /*Add a button to the current screen*/
    lv_obj_set_pos(btn, 10, 128-50);                 /*Set its position*/
    lv_obj_set_size(btn, 100, 50);               /*Set its size*/

    lv_obj_t *label = lv_label_create(btn); /*Add a label to the button*/
    lv_label_set_text(label, "Button");     /*Set the labels text*/
    lv_obj_center(label);
    /*Align the label to */


    while (1)
    {
        lv_task_handler();
        lv_tick_inc(5);
        vTaskDelay(1);
    }
}
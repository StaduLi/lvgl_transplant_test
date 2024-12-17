# Some attentions that should be taken into account

### 1.Encounter the snowflake lace

that means that ur start position is error or the end position is error.You need to set in the driver header the start position and the end position again.
like this:
(this is setting for screen whose chip is st7735s and whose resolution is 128*128 )

```
#define COLSTART            0
#define ROWSTART            32
```

# Below is the transplant lvgl_v8.2 steps:
### 1.Download lvgl library and lvgl_esp32_drivers from github
you can download them by the following website:
[lvgl_v8.2](https://github.com/lvgl/lvgl/tree/release/v8.2)
[the esp32 drivers for lvgl](https://github.com/lvgl/lvgl_esp32_drivers)
### 2.Create a new project in the ESP-IDF
### 3.Copy the lvgl_relase_v8.2 and lvgl_esp32_drivers to the project
1) create the folder named "components" in the root of project
2) copy the lvgl_relase_v8.2 and lvgl_esp32_drivers to the folder "components"
3) change the name of the folder "lvgl-release-v8.2" to "lvgl" and the name of the folder "lvgl_esp32_drivers-master" to "lvgl_esp32_drivers"
4) create the folder named "porting" under the components
### 4.Edit the cmake file under the root of project
befor the  code:
```cmake
include($ENV{IDF_PATH}/tools/cmake/project.cmake)
```
add the code:
```cmake
set(EXTRA_COMPONENT_DIRS components/lvgl components/lvgl_esp32_drivers components/porting)
```
### 5.Compile for the first time and than check the error
1) **cnnot open the sorce file "driver/gpio.h"**
solution: change code in the cmake file under the lvgl_esp32_drivers line 87:
```cmake
idf_component_register(SRCS ${SOURCES}
                       INCLUDE_DIRS ${LVGL_INCLUDE_DIRS}
                       REQUIRES lvgl driver)
```
and then compile again
2) **'LV_HOR_RES_MAX' undeclared**
solution: add code in the file "lvgl_header.h" under the lvgl_esp32_drivers:
```c
#ifndef LV_HOR_RES_MAX
#define LV_HOR_RES_MAX  128
#endif
#ifndef LV_VER_RES_MAX
#define LV_VER_RES_MAX  128
#endif
```
3) **'ledc_timer_config_t' has no member named 'bit_num'**
this is a bug which caused by the variable "bit_num" in the file .
solution: correct the 
```c
.bit_num
```
to 
```c
.duty_resolution
```
4) **implicit declaration of function 'gpio_matrix_out'; did you mean 'gpio_iomux_out'** 
solution: include the file rom/gpio.h in the file.
5) **implicit declaration of function 'gpio_pad_select_gpio'; did you mean 'esp_rom_gpio_pad_select_gpio'**
solution: include the file "soc/gpio_sig_map.h" in the file.
6) **implicit declaration of function 'gpio_pad_select_gpio'; did you mean 'esp_rom_gpio_pad_select_gpio'**
solution: include the file "rom/gpio.h" in the file "disp_spi.h".
7) **'portTICK_RATE_MS' undeclared**
this is a bug which caused by the variable "portTICK_RATE_MS" name change.
solution: replace the "portTICK_RATE_MS" to "portTICK_PERIOD_MS" in all file 

and then compile again.
### 6.Edit the porting folder
1. open the folder **components\lvgl\examples** 

2. copy the folder **porting** to the **components**

3. create the cmake file called **"CMakeLists.txt"** and edit it:

   ```cmake
   idf_component_register(SRCS  "lv_port_disp.c"
                          INCLUDE_DIRS ${LVGL_INCLUDE_DIRS}
                          REQUIRES lvgl lvgl_esp32_drivers)
   
   ```
4. revise the file name revise the file "lv_port_disp_template.h" to "lv_port_disp.h"
5. open the content of file "lv_port_disp.c" and file "lv_port_disp.h"
open the **"#if 0" to "#if 1"** in this two file.
### 7.Config the porting file
1. open the file "lv_port_disp.c" 
2. include the file **"lvgl_esp32_drivers/lvgl_helpers.h"**
3. select the buffer size in the function "lv_port_disp_init"
    if you want to select the Example 1,you should comment out other example and change the code 
```c
    /* Example for 1) */
    static lv_disp_draw_buf_t draw_buf_dsc_1;
    static lv_color_t buf_1[DISP_BUF_SIZE];                          /*A buffer for 10 rows*/
    lv_disp_draw_buf_init(&draw_buf_dsc_1, buf_1, NULL, DISP_BUF_SIZE);   /*Initialize the display buffer*/
```
**attention that only the rgb screen should select example 3.**
4.set the display resolution in the function "lv_port_disp_init"
in the line 103,revise the code:
```c
    /*Set the resolution of the display*/
    disp_drv.hor_res = LV_HOR_RES_MAX;
    disp_drv.ver_res = LV_VER_RES_MAX;
```
5.select the refresh method
in the line 107,revise the code:
```c
    /*Used to copy the buffer's content to the display*/
    disp_drv.flush_cb = disp_driver_flush;
```
6. change the function "disp_init"
add code in this function:
```c
    /*You code here*/
    lvgl_driver_init();
```

**compile again,if you call the function "lvgl_port_disp_init" in the main file and the terminal report the error "lvgl_port_disp_init is not find",then you should full clean and rebuild the project.**
### 8.configure the physical port
you should open the idf.py menuconfig and then select the pin and select the chip you use.
or you can use the sdk configure tool to configure the port.
these two method all work.

![image-20241217132847648](C:\Users\LiYou\AppData\Roaming\Typora\typora-user-images\image-20241217132847648.png)








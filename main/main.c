#include <stdio.h>
#include "driver/gpio.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "string.h"

#include "epd_driver.h"
#include "epd_highlevel.h"

EpdiyHighlevelState hl;

void app_main(void) {
    heap_caps_print_heap_info(MALLOC_CAP_INTERNAL);
    heap_caps_print_heap_info(MALLOC_CAP_SPIRAM);

    epd_init(EPD_OPTIONS_DEFAULT);
    epd_set_rotation(EPD_ROT_LANDSCAPE);

    hl = epd_hl_init(EPD_BUILTIN_WAVEFORM);
    printf("Dimensions after rotation, width: %d height: %d\n\n",
           epd_rotated_display_width(),
           epd_rotated_display_height());

    uint32_t width  = epd_rotated_display_width();
    uint32_t height = epd_rotated_display_height();

    uint8_t* fb = epd_hl_get_framebuffer(&hl);
    epd_poweron();
    epd_fullclear(&hl, 18);

    uint16_t color       = 0;
    uint16_t x_coord     = 0;
    uint16_t line_length = 200;
    for (int x = x_coord; x < 300; x += 50) {
        for (int y = 0; y < line_length; y++) {
            epd_draw_pixel(x, y, color, fb);
        }
    }

    enum EpdDrawError err = epd_hl_update_screen(&hl, MODE_GC16, 18);
    if (err != EPD_DRAW_SUCCESS) {
        printf("%d", err);
    }

    printf("Done, powering off\r\n");
    epd_poweroff();
}

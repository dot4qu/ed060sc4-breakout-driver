#include <stdio.h>
#include "driver/gpio.h"
#include "esp_timer.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "string.h"

#include "epd_driver.h"
#include "epd_highlevel.h"
#include "firasans_20.h"

#define FONT FiraSans_20

void delay(uint64_t millis) {
    vTaskDelay(millis / portTICK_PERIOD_MS);
}

/*
void app_main(void) {
    epd_init(EPD_OPTIONS_DEFAULT);
    delay(300);
    epd_poweron();
    epd_clear();
    epd_poweroff();
}
*/

EpdiyHighlevelState hl;

void app_main(void) {
    epd_init(EPD_LUT_1K);
    hl = epd_hl_init(EPD_BUILTIN_WAVEFORM);
    epd_set_rotation(EPD_ROT_LANDSCAPE);
    printf("Dimensions after rotation, width: %d height: %d\n\n",
           epd_rotated_display_width(),
           epd_rotated_display_height());

    uint32_t width  = epd_rotated_display_width();
    uint32_t height = epd_rotated_display_height();

    uint8_t* fb = epd_hl_get_framebuffer(&hl);

    epd_poweron();
    epd_fullclear(&hl, 18);
    epd_poweroff();

    uint16_t color       = 0;
    uint16_t x_coord     = 0;
    uint16_t line_length = 50;
    for (int x = x_coord; x < 800; x += 50) {
        for (int y = 0; y < line_length; y++) {
            epd_draw_pixel(x, y, color, fb);
        }
    }

    char msg[100] = {0};
    // sprintf(msg, "sssshhhhhhhhhhaka\nMAHALOOOO MY CHILLER");
    sprintf(msg, "tada!");
    int               cursor_x   = 0;
    int               cursor_y   = height / 2;
    EpdFontProperties font_props = epd_font_properties_default();
    font_props.flags             = EPD_DRAW_ALIGN_LEFT;
    printf("drawing string at %d,%d\r\n", cursor_x, cursor_y);
    epd_write_string(&FONT, msg, &cursor_x, &cursor_y, fb, &font_props);

    epd_poweron();
    enum EpdDrawError err = epd_hl_update_screen(&hl, MODE_GL16, 18);
    epd_poweroff();
    if (err != EPD_DRAW_SUCCESS) {
        printf("%d", err);
    }

    epd_poweron();
    err = epd_hl_update_screen(&hl, MODE_GL16, 18);
    epd_poweroff();
    if (err != EPD_DRAW_SUCCESS) {
        printf("%d", err);
    }

    printf("Done, powering off\r\n");
    while (1) {
        vTaskDelay(100);
    }
}

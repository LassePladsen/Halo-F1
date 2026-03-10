// TOUCH SCREEN
#ifdef TOUCH_CAPACITIVE
#include <bb_captouch.h>

BBCapTouch bbct;
#else
BB_SPI_LCD * lcd;
#endif

uint16_t touchMinX = TOUCH_MIN_X, touchMaxX = TOUCH_MAX_X, touchMinY = TOUCH_MIN_Y, touchMaxY = TOUCH_MAX_Y;
TOUCHINFO ti;

void touch_read( lv_indev_t * indev, lv_indev_data_t * data ) {

#ifdef TOUCH_CAPACITIVE
  // Capacitive touch needs to be mapped to display pixels
  if(bbct.getSamples(&ti)) {
    /*Serial.print("raw touch x: ");
    Serial.print(ti.x[0]);
    Serial.print(" y: ");
    Serial.println(ti.y[0]);*/

    if(ti.x[0] < touchMinX) touchMinX = ti.x[0];
    if(ti.x[0] > touchMaxX) touchMaxX = ti.x[0];
    if(ti.y[0] < touchMinY) touchMinY = ti.y[0];
    if(ti.y[0] > touchMaxY) touchMaxY = ti.y[0];

    //Map this to the pixel position
    data->point.y = lv_display_get_vertical_resolution(NULL) - map(ti.x[0], touchMinX, touchMaxX, 1, lv_display_get_vertical_resolution(NULL)); // X touch mapping
    data->point.x = map(ti.y[0], touchMinY, touchMaxY, 1, lv_display_get_horizontal_resolution(NULL)); // Y touch mapping
    data->state = LV_INDEV_STATE_PRESSED;
#else
  // Resistive touch is already mapped by the bb_spi_lcd library
  if(lcd->rtReadTouch(&ti)) {
    if(ti.x[0] < touchMinX) touchMinX = ti.x[0];
    if(ti.x[0] > touchMaxX) touchMaxX = ti.x[0];
    if(ti.y[0] < touchMinY) touchMinY = ti.y[0];
    if(ti.y[0] > touchMaxY) touchMaxY = ti.y[0];


    data->point.x = lv_display_get_horizontal_resolution(NULL) - ti.x[0];
    data->point.y = map(ti.y[0], touchMinY, touchMaxY, 1, lv_display_get_vertical_resolution(NULL));
    data->state = LV_INDEV_STATE_PRESSED;
#endif


    /*Serial.print("mapped touch x: ");
    Serial.print(data->point.x);
    Serial.print(" y: ");
    Serial.println(data->point.y);*/
  } else {
    data->state = LV_INDEV_STATE_RELEASED;
  }
}
// END TOUCH SCREEN
#include <lvgl.h>
#include <TFT_eSPI.h>

// Definiciones del ADC y del pin
#define ADC_PIN 34
#define PWM_PIN 27
#define RED_PIN 13        //nota, en el modulo RGB EL rojo y el verde estan cambiados, poner el rojo en el verde y viceversa
#define GREEN_PIN 12
#define BLUE_PIN 14

// Definiciones de la resolución de la pantalla
#define SCREEN_WIDTH 320
#define SCREEN_HEIGHT 240

// Definiciones generales
#define BUTTON_PR 1

// Declaración del objeto TFT
TFT_eSPI tft = TFT_eSPI();

// Variables y constantes
char arc_text[16], button_text[5];
int adc_value, button_clicked;
bool button_state = 0, RED = 0, GREEN = 0, BLUE = 0;
float raw_slider = 0, adapted_slider = 0;

// Buffer para LVGL
static lv_disp_draw_buf_t draw_buf;
static lv_color_t buf1[SCREEN_WIDTH * 10];
static lv_color_t buf2[SCREEN_WIDTH * 10];

// Variables del arco y del botón
lv_obj_t *arc;
lv_obj_t *button;
lv_obj_t *button2;
lv_obj_t *slider;
lv_obj_t *sw;
lv_obj_t *sw2;
lv_obj_t *sw3;
lv_obj_t *label_button;
lv_obj_t *label_button2;
lv_obj_t *label_arc;
lv_obj_t *label_slider;

// Flush de LVGL para actualizar datos de la pantalla
void my_disp_flush(lv_disp_drv_t *disp, const lv_area_t *area, lv_color_t *color_p) {
    tft.startWrite();
    tft.setAddrWindow(area->x1, area->y1, area->x2 - area->x1 + 1, area->y2 - area->y1 + 1);
    tft.pushColors(&color_p->full, (area->x2 - area->x1 + 1) * (area->y2 - area->y1 + 1), true);
    tft.endWrite();

    lv_disp_flush_ready(disp);
}

// Lectura de pantalla táctil
void my_touchpad_read(lv_indev_drv_t *indev_driver, lv_indev_data_t *data) {
    uint16_t touchX, touchY;
    bool touched = tft.getTouch(&touchX, &touchY);

    if (!touched) {
        data->state = LV_INDEV_STATE_REL;
    } else {
        data->state = LV_INDEV_STATE_PR;
        data->point.x = touchX;
        data->point.y = touchY;
    }
}

// Handler evento botón
void button_event_handler(lv_event_t *e) {
    button_state = !button_state;
}

// Segundo handler para el segundo botón, de callback diferente
void button2_event_handler(lv_event_t *e) {
  button_clicked++;
    snprintf(button_text, sizeof(button_text), "%d", button_clicked);
    lv_label_set_text(label_button2, button_text);
}


// Handler slider
void slider_event_cb(lv_event_t *e)
{
    lv_obj_t *slider = lv_event_get_target(e);

    // Valor slider
    lv_label_set_text_fmt(label_slider, "%"LV_PRId32, lv_slider_get_value(slider));
    lv_obj_align_to(label_slider, slider, LV_ALIGN_OUT_TOP_MID, 0, -15);    /*Align top of the slider*/
}

void sw_cback_handler(lv_event_t *e){
  if (lv_obj_has_state(sw, LV_STATE_CHECKED)) {
        digitalWrite(RED_PIN, HIGH);
    } else {
        digitalWrite(RED_PIN, LOW);
    }
}

void sw2_cback_handler(lv_event_t *e){
  if (lv_obj_has_state(sw2, LV_STATE_CHECKED)) {      // Comprobacion si el switch esta en estado ON
        digitalWrite(GREEN_PIN, HIGH);
    } else {
        digitalWrite(GREEN_PIN, LOW);
    }
}

void sw3_cback_handler(lv_event_t *e){
  if (lv_obj_has_state(sw3, LV_STATE_CHECKED)) {
        digitalWrite(BLUE_PIN, HIGH);
    } else {
        digitalWrite(BLUE_PIN, LOW);
    }
}


// Función de calibración de la pantalla. Necesaria para un correcto funcionamiento de la pantalla
void touch_calibrate() {
    uint16_t calData[5];
    tft.fillScreen(TFT_BLACK);
    tft.setCursor(20, 0);
    tft.setTextFont(2);
    tft.setTextSize(1);
    tft.setTextColor(TFT_WHITE, TFT_BLACK);

    tft.println("Pulsa las flechas que aparezcan");

    tft.setTextFont(1);
    tft.println();

    tft.calibrateTouch(calData, TFT_MAGENTA, TFT_BLACK, 15);
    tft.setTouch(calData);
}

void LVGL_INIT_TEMPLATE() {   //Llamar a esta función en el setup

// Inicialización de LVGL
    lv_init();

  // Inicialización del buffer de dibujo
    lv_disp_draw_buf_init(&draw_buf, buf1, buf2, SCREEN_WIDTH * 10);

    // Inicialización del controlador de la pantalla
    static lv_disp_drv_t disp_drv;
    lv_disp_drv_init(&disp_drv);
    disp_drv.hor_res = SCREEN_WIDTH;
    disp_drv.ver_res = SCREEN_HEIGHT;
    disp_drv.flush_cb = my_disp_flush;
    disp_drv.draw_buf = &draw_buf;
    lv_disp_drv_register(&disp_drv);

    // Inicialización del controlador táctil
    static lv_indev_drv_t indev_drv;
    lv_indev_drv_init(&indev_drv);
    indev_drv.type = LV_INDEV_TYPE_POINTER;
    indev_drv.read_cb = my_touchpad_read;
    lv_indev_drv_register(&indev_drv);

}

void setup() {
    // Inicialización de UART
    Serial.begin(115200);

    // Inicialización de la pantalla
    tft.begin();
    tft.setRotation(1);

    // Calibración de la pantalla táctil
    touch_calibrate();

    // Inicialización de LVGL
    LVGL_INIT_TEMPLATE();

    // Crear un arco
    arc = lv_arc_create(lv_scr_act());
    lv_obj_set_size(arc, 100, 100);
    lv_obj_align(arc, LV_ALIGN_CENTER, 0, -50);
    lv_arc_set_range(arc, 0, 4095); // Rango del ADC (0-4095)
    lv_obj_clear_flag(arc, LV_OBJ_FLAG_CLICKABLE); // Desactivar interactividad

    // Crear un botón
    button = lv_btn_create(lv_scr_act());
    lv_obj_set_size(button, 50, 50);
    lv_obj_align(button, LV_ALIGN_CENTER, -50, 25);
    lv_obj_add_event_cb(button, button_event_handler, LV_EVENT_CLICKED, NULL);

    // Crear un segundo botón
    button2 = lv_btn_create(lv_scr_act());
    lv_obj_set_size(button2, 50, 50);
    lv_obj_align(button2, LV_ALIGN_CENTER, 50, 25);
    lv_obj_add_event_cb(button2, button2_event_handler/*La callback puede tener el nombre q quieras*/, LV_EVENT_CLICKED, NULL);

    // Crear un slider
    slider = lv_slider_create(lv_scr_act());
    lv_obj_set_width(slider, 200);
    lv_obj_align(slider, LV_ALIGN_CENTER, 0, 85);
    lv_obj_add_event_cb(slider, slider_event_cb, LV_EVENT_VALUE_CHANGED, NULL);   // Misma manera de añadir interactividad que el arco

    // Crear una etiqueta para el botón
    label_button = lv_label_create(button);
    lv_label_set_text(label_button, "Medir");
    lv_obj_center(label_button);

    label_button2 = lv_label_create(button2);
    lv_label_set_text(label_button2, "0");
    lv_obj_center(label_button2);
    lv_obj_set_style_bg_color(button2, lv_color_hex(0xFF0000), LV_PART_MAIN); // Rojo
    //lv_obj_set_style_bg_grad_color(button2, lv_color_hex(0x880000), LV_PART_MAIN); // Gradiente rojo oscuro
    lv_obj_set_style_bg_color(button, lv_color_hex(0x00FF00), LV_PART_MAIN | LV_STATE_PRESSED); // Verde cuando está presionado

    // Crear una etiqueta para el arco
    label_arc = lv_label_create(arc);     // -> Si se desea actualizar el valor de label, se crea un lv_obj_t *xx donde xx es el nombre de la label, se pasa el parametro por snprintf, y se escribe como si fuera una etiqueta normal
    lv_label_set_text(label_arc, "ADC: 0");
    lv_obj_align(label_arc, LV_ALIGN_CENTER, 0, 0);

    // Crear una etiqueta para el deslizador
    label_slider = lv_label_create(lv_scr_act());
    lv_label_set_text(label_slider, "0");
    lv_obj_align_to(label_slider, slider, LV_ALIGN_OUT_TOP_MID, 0, -15);

    analogReadResolution(12); // Resolución del ADC a 12 bits (0-4095)
    pinMode(RED_PIN, OUTPUT);
    pinMode(GREEN_PIN, OUTPUT);
    pinMode(BLUE_PIN, OUTPUT);
    digitalWrite(RED_PIN, LOW);
    digitalWrite(GREEN_PIN, LOW);
    digitalWrite(BLUE_PIN, LOW);



    // Switch RED
    sw = lv_switch_create(lv_scr_act());
    lv_obj_add_event_cb(sw, sw_cback_handler, LV_EVENT_VALUE_CHANGED, NULL);
    lv_obj_align(sw, LV_ALIGN_CENTER, -115, -90);

    // Switch GREEN
    sw2 = lv_switch_create(lv_scr_act());
    lv_obj_add_event_cb(sw2, sw2_cback_handler, LV_EVENT_VALUE_CHANGED, NULL);
    lv_obj_align(sw2, LV_ALIGN_CENTER, -115, -50);

    // Switch BLUE
    sw3 = lv_switch_create(lv_scr_act());
    lv_obj_add_event_cb(sw3, sw3_cback_handler, LV_EVENT_VALUE_CHANGED, NULL);
    lv_obj_align(sw3, LV_ALIGN_CENTER, -115, -10);
    
}

void loop() {

  if (button_state == BUTTON_PR){
    // Leer el valor del ADC cuando se presiona el botón
    adc_value = analogRead(ADC_PIN);

    // Mostrar el valor en el arco
    lv_arc_set_value(arc, adc_value);
    snprintf(arc_text, sizeof(arc_text), "ADC: %d", adc_value);
    lv_label_set_text(label_arc, arc_text);
  }

  raw_slider = lv_slider_get_value(slider);
  adapted_slider = raw_slider * 2.55;   // Sin FPU va bastante mal
  analogWrite (PWM_PIN, adapted_slider);

    // Manejar las tareas de LVGL
    lv_timer_handler();
    delay(5);
}

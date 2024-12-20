#include <lvgl.h>
#include <TFT_eSPI.h>
#include <Wire.h>
#include <SPI.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>
#include <esp_sleep.h>
#include <SNESController.h>


// Prototipos

/*bme280 pinout scl22 sda21*/

void load_bme_screen();
void load_list_screen();
void load_home_screen();
void LVGL_INIT_TEMPLATE();

#define SCREEN_WIDTH 320
#define SCREEN_HEIGHT 240

#define BACKLIGHT_PIN 32
#define WAKEUP_PIN GPIO_NUM_26    // Esto hay que definirlo asi por algun motivo raro, no funciona si defines como int directamente
#define LATCH_PIN
#define SNES_CLOCK_PIN
#define DATA_PIN

// Invocar objeto TFT
TFT_eSPI tft = TFT_eSPI();

// Invocar BME280
Adafruit_BME280 bme; // I2C

// Invocar mando SNES
SNESController snes(LATCH_PIN, SNES_CLOCK_PIN, DATA_PIN);

// Buffer para LVGL (doble buffer)
static lv_disp_draw_buf_t draw_buf;
static lv_color_t buf1[SCREEN_WIDTH * 10];
static lv_color_t buf2[SCREEN_WIDTH * 10];

static lv_obj_t *list1;                  // Main screen
static lv_obj_t *home_screen = NULL;    // Subscreen
static lv_obj_t *bme_screen = NULL;    // Subscreen
static lv_obj_t *bme_chart = NULL;    // Subscreen


float temperature = 0;
struct TimerData {
    lv_obj_t *chart;
    lv_chart_series_t *ser;
    lv_obj_t *temp_label;
};


// FunciOn para cambiar a la pantalla de lista
void load_list_screen() {
    // Crear pantalla de lista
    list1 = lv_list_create(NULL); // Crear una nueva pantalla
    lv_obj_set_size(list1, 180, 220);
    lv_obj_center(list1);

    lv_list_add_text(list1, "Opciones principales");
    lv_obj_t *btn_home = lv_list_add_btn(list1, LV_SYMBOL_HOME, "Menu");
    lv_obj_add_event_cb(btn_home, [](lv_event_t *e) {
        LV_LOG_USER("Home button clicked");
        load_home_screen();
    }, LV_EVENT_CLICKED, NULL);

    lv_obj_t *btn_measure = lv_list_add_btn(list1, LV_SYMBOL_DIRECTORY, "Medicion");
    lv_obj_add_event_cb(btn_measure, [](lv_event_t *e) {    // Callback para el measure, ahora solo muestra direct chart
      LV_LOG_USER("Measure button clicked");
      load_bme_screen();
       }, LV_EVENT_CLICKED, NULL);
    
    lv_list_add_text(list1, "Suspender");
    lv_obj_t *btn_close = lv_list_add_btn(list1, LV_SYMBOL_POWER, "Suspension");
    lv_obj_add_event_cb(btn_close, [](lv_event_t *e) {
       LV_LOG_USER("Suspension button clicked");
       digitalWrite(BACKLIGHT_PIN, LOW);
       //esp_sleep_enable_timer_wakeup(10 * 1000000);   // Wake up por timer a 10s
       esp_sleep_enable_ext0_wakeup(WAKEUP_PIN, 1);     // Wake up por evento externo. Pull down externo, activo por evento (flanco de subida)
       Serial.flush();
       esp_deep_sleep_start(); 
       }, LV_EVENT_CLICKED, NULL);

    // Cargar la pantalla de la lista
    lv_scr_load(list1);
}

void load_bme_screen() {
    bme_screen = lv_obj_create(NULL);

    lv_obj_t *chart = lv_chart_create(bme_screen);
    lv_obj_set_size(chart, 200, 150);
    lv_obj_align(chart, LV_ALIGN_CENTER, 0, -20);
    lv_chart_set_type(chart, LV_CHART_TYPE_LINE); // Modo linea

    // Agregar una serie al grafico (solo medimos temperatura)
    lv_chart_series_t *ser1 = lv_chart_add_series(chart, lv_palette_main(LV_PALETTE_RED), LV_CHART_AXIS_PRIMARY_Y);

    // Inicializar datos iniciales en el grafico
    for (int i = 0; i < 10; i++) {
        lv_chart_set_next_value(chart, ser1, 0);
    }

    // Etiqueta de titulo del grafico
    lv_obj_t *chart_title = lv_label_create(bme_screen);
    lv_label_set_text(chart_title, "Grafico de Temperatura (°C)");
    lv_obj_align(chart_title, LV_ALIGN_TOP_MID, 0, 10);

    // Etiqueta para mostrar el valor actual de la temperatura
    lv_obj_t *temp_label = lv_label_create(bme_screen);
    lv_label_set_text(temp_label, "Temperatura actual: -- °C");
    lv_obj_align(temp_label, LV_ALIGN_BOTTOM_MID, 0, -10);

    // Boton para volver a la lista
    lv_obj_t *back_btn = lv_btn_create(bme_screen);
    lv_obj_set_size(back_btn, 40, 20);
    lv_obj_align(back_btn, LV_ALIGN_TOP_LEFT, 5, 5);
    lv_obj_add_event_cb(back_btn, [](lv_event_t *e) {
        LV_LOG_USER("Back to list clicked");
        load_list_screen();
    }, LV_EVENT_CLICKED, NULL);

    lv_obj_t *back_label = lv_label_create(back_btn);
    lv_label_set_text(back_label, "Back");
    lv_obj_center(back_label);

    // Crear un temporizador para actualizar el grafico y la etiqueta de temperatura
    TimerData *data = new TimerData{chart, ser1, temp_label}; // Incluye temp_label en la estructura
    lv_timer_create([](lv_timer_t *timer) {
        TimerData *data = static_cast<TimerData *>(timer->user_data);
        temperature = bme.readTemperature();

        if (isnan(temperature)) {
            LV_LOG_USER("Error: No se pudo leer la temperatura del BME280");
        } else {
            LV_LOG_USER("Temperatura actual: %.2f °C", temperature);
            lv_chart_set_next_value(data->chart, data->ser, temperature);
            lv_chart_refresh(data->chart);

            // Actualizar la etiqueta de temperatura
            char temp_str[50];
            snprintf(temp_str, sizeof(temp_str), "Temperatura actual: %.2f °C", temperature);
            lv_label_set_text(data->temp_label, temp_str); // Usa la etiqueta desde TimerData
        }
    }, 400, data);

    lv_scr_load(bme_screen);
}


// Funcion para cambiar a la pantalla de inicio
void load_home_screen() {
    // Crear una nueva pantalla de inicio
    home_screen = lv_obj_create(NULL);

    char time_str[50] = "19/12/2024 19:00:00";    // Esto es solo un ejemplo. Se puede cambiar a RTC o, ¿conseguir el tiempo local por internet?

    // Crea y agrega elementos a la pantalla de inicio
    lv_obj_t *time_label = lv_label_create(home_screen);
    lv_label_set_text(time_label, time_str);
    lv_obj_align(time_label, LV_ALIGN_TOP_MID, 0, 20);

    // Boton para volver a la lista inicial
    lv_obj_t *back_btn = lv_btn_create(home_screen);
    lv_obj_set_size(back_btn, 40, 20); 
    lv_obj_align(back_btn, LV_ALIGN_TOP_LEFT, 5, 5);
    lv_obj_add_event_cb(back_btn, [](lv_event_t *e) {
        LV_LOG_USER("Back to list clicked");
        load_list_screen();
    }, LV_EVENT_CLICKED, NULL);

    lv_obj_t *back_label = lv_label_create(back_btn);
    lv_label_set_text(back_label, "Back");
    lv_obj_center(back_label);

    // Cargar la nueva pantalla
    lv_scr_load(home_screen);
}




// Plantilla para inicial LVGL. Es asi para la mayoria de proyectos simples (creada como funcion instanciable)
void LVGL_INIT_TEMPLATE() {
    // Inicia LVGL
    lv_init();

    // Inicia buffer
    lv_disp_draw_buf_init(&draw_buf, buf1, buf2, SCREEN_WIDTH * 10);

    // Inicia controlador de pantalla (hacer los cambios pertinentes de controlador en TFT_eSPI)
    static lv_disp_drv_t disp_drv;
    lv_disp_drv_init(&disp_drv);
    disp_drv.hor_res = SCREEN_WIDTH;
    disp_drv.ver_res = SCREEN_HEIGHT;
    disp_drv.flush_cb = [](lv_disp_drv_t *disp, const lv_area_t *area, lv_color_t *color_p) {
        tft.startWrite();
        tft.setAddrWindow(area->x1, area->y1, area->x2 - area->x1 + 1, area->y2 - area->y1 + 1);
        tft.pushColors(&color_p->full, (area->x2 - area->x1 + 1) * (area->y2 - area->y1 + 1), true);
        tft.endWrite();
        lv_disp_flush_ready(disp);
    };
    disp_drv.draw_buf = &draw_buf;
    lv_disp_drv_register(&disp_drv);

    // Inicia controlador de pantalla tactil
    static lv_indev_drv_t indev_drv;
    lv_indev_drv_init(&indev_drv);
    indev_drv.type = LV_INDEV_TYPE_POINTER;
    indev_drv.read_cb = [](lv_indev_drv_t *indev_driver, lv_indev_data_t *data) {
        uint16_t touchX, touchY;
        bool touched = tft.getTouch(&touchX, &touchY);
        data->state = touched ? LV_INDEV_STATE_PR : LV_INDEV_STATE_REL;
        if (touched) {
            data->point.x = touchX;
            data->point.y = touchY;
        }
    };
    lv_indev_drv_register(&indev_drv);
}

void touch_calibrate() {
    //uint16_t calData[5];
    //tft.fillScreen(TFT_BLACK);
    //tft.setCursor(20, 0);
    //tft.setTextFont(2);
    //tft.setTextSize(1);
    //tft.setTextColor(TFT_WHITE, TFT_BLACK);
    //tft.println("Pulsa las flechas que aparezcan");
    //tft.setTextFont(1);
    //tft.println();
    //tft.calibrateTouch(calData, TFT_MAGENTA, TFT_BLACK, 15);
    //tft.setTouch(calData);

  // Generico para mi pantalla. Comentar los datos de calibracion y descomentar las lineas de arriba para calibrar por cada iniciacion
  uint16_t calData[5] = { 316, 3559, 218, 3510, 7 };
  tft.setTouch(calData);    //
}

void setup() {
    Serial.begin(115200);

    pinMode (BACKLIGHT_PIN, OUTPUT);
    //pinMode (WAKEUP_PIN, INPUT);
    digitalWrite (BACKLIGHT_PIN, HIGH);

    // Inicia pantalla
    tft.begin();
    tft.setRotation(1);

    // Calibra pantalla tactil
    touch_calibrate();

    // Inicio BME
    bme.begin(0x76);    // Añadir direccion (normalmente 0x76)


    // Inicializar LVGL
    LVGL_INIT_TEMPLATE();

    // Cargar pantalla inicial
    load_list_screen();
}

void loop() {
    lv_timer_handler();
    delay(5);             // Dejamos a la GUI trabajar y a la CPU hacer sus tareas
}

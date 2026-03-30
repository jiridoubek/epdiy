#include "esp_adc/adc_oneshot.h"
#include "epd_board.h"
#include "esp_log.h"

static const adc_channel_t channel = ADC_CHANNEL_7;
static adc_oneshot_unit_handle_t adc1_handle;

#define NUMBER_OF_SAMPLES 100
// Approximate mV per ADC step at 12-bit, 6dB attenuation (~0-1.75V range)
#define ADC_MV_PER_STEP (1750.0 / 4095.0)

void epd_board_temperature_init_v2() {
    adc_oneshot_unit_init_cfg_t init_config = {
        .unit_id = ADC_UNIT_1,
    };
    adc_oneshot_new_unit(&init_config, &adc1_handle);

    adc_oneshot_chan_cfg_t chan_config = {
        .bitwidth = ADC_BITWIDTH_12,
        .atten = ADC_ATTEN_DB_6,
    };
    adc_oneshot_config_channel(adc1_handle, channel, &chan_config);
    ESP_LOGI("epd_temperature", "ADC initialized for temperature sensing\n");
}

float epd_board_ambient_temperature_v2() {
    int raw = 0;
    uint32_t value = 0;
    for (int i = 0; i < NUMBER_OF_SAMPLES; i++) {
        adc_oneshot_read(adc1_handle, channel, &raw);
        value += raw;
    }
    value /= NUMBER_OF_SAMPLES;
    // Approximate voltage conversion without calibration
    float voltage = value * ADC_MV_PER_STEP;
    return (voltage - 500.0) / 10.0;
}

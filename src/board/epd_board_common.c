#include "esp_adc/adc_oneshot.h"
#include "esp_adc/adc_cali.h"
#include "esp_adc/adc_cali_scheme.h"
#include "epd_board.h"
#include "esp_log.h"

static const adc_channel_t channel = ADC_CHANNEL_7;
static adc_oneshot_unit_handle_t adc1_handle;
static adc_cali_handle_t adc1_cali_handle;

#define NUMBER_OF_SAMPLES 100

void epd_board_temperature_init_v2() {
    // Configure ADC oneshot
    adc_oneshot_unit_init_cfg_t init_config = {
        .unit_id = ADC_UNIT_1,
    };
    adc_oneshot_new_unit(&init_config, &adc1_handle);

    adc_oneshot_chan_cfg_t chan_config = {
        .bitwidth = ADC_BITWIDTH_12,
        .atten = ADC_ATTEN_DB_6,
    };
    adc_oneshot_config_channel(adc1_handle, channel, &chan_config);

    // Configure calibration
    adc_cali_curve_fitting_config_t cali_config = {
        .unit_id = ADC_UNIT_1,
        .atten = ADC_ATTEN_DB_6,
        .bitwidth = ADC_BITWIDTH_12,
    };
    esp_err_t ret = adc_cali_create_scheme_curve_fitting(&cali_config, &adc1_cali_handle);
    if (ret == ESP_OK) {
        ESP_LOGI("epd_temperature", "Calibration scheme: curve fitting\n");
    } else {
        ESP_LOGW("epd_temperature", "Calibration not available\n");
    }
}

float epd_board_ambient_temperature_v2() {
    int raw = 0;
    uint32_t value = 0;
    for (int i = 0; i < NUMBER_OF_SAMPLES; i++) {
        adc_oneshot_read(adc1_handle, channel, &raw);
        value += raw;
    }
    value /= NUMBER_OF_SAMPLES;

    int voltage = 0;
    adc_cali_raw_to_voltage(adc1_cali_handle, value, &voltage);
    return ((float)voltage - 500.0) / 10.0;
}

#include "epd_board.h"
#include "esp_log.h"

// Temperature sensing is not critical for display operation.
// The esp_adc headers require CMake-generated files unavailable in PlatformIO.
// Stub these out with safe defaults.

void epd_board_temperature_init_v2() {
    ESP_LOGI("epd_temperature", "Temperature sensing stubbed (PlatformIO build)\n");
}

float epd_board_ambient_temperature_v2() {
    // Return 25°C as a safe default room temperature.
    // This is used for waveform selection; 25°C gives standard waveforms.
    return 25.0;
}

// by Khalil Adawi
// Updated on 11/9/2025

/*
  This code demonstrates how to use the ESP32-S3's touch pads (GPIO 8 and 9)
  to wake it up from deep sleep.

  How it works:
  1. On the first boot (or after any other type of reset), the code calibrates
     the touch pads by taking an initial reading.
  2. It calculates a threshold based on this reading. A touch is detected when
     the sensor value drops below this threshold.
  3. The ESP32-S3 is configured to wake up from deep sleep if either touch pad 8 or 9
     is triggered.
  4. The device then enters deep sleep to conserve power.
  5. When you touch one of the pads, the ESP32-S3 wakes up, prints which pad was
     touched, and then goes back to sleep.
*/

// --- Configuration ---
// The threshold is a percentage of the initial reading. A lower percentage means
// the touch sensor will be more sensitive. 80% means the reading has to drop
// by 20% from its baseline to trigger a wakeup.
#define TOUCH_THRESHOLD_PERCENTAGE 80

// Define the GPIO pins for the touch pads. On the ESP32-S3, these GPIOs
// correspond directly to the touch channel numbers.
#define TOUCH_PAD_8_PIN   8  // Corresponds to TOUCH_PAD_NUM8
#define TOUCH_PAD_9_PIN   9  // Corresponds to TOUCH_PAD_NUM9
// Note: GPIO 7 is not actively used for the wakeup logic, but is defined here
// to acknowledge the original code's reference pin.
#define REFERENCE_PAD_PIN 7  // Corresponds to TOUCH_PAD_NUM7


// --- Function Prototypes ---
void print_wakeup_reason();
void print_wakeup_touchpad();


void setup() {
  Serial.begin(115200);
  delay(1000); // Wait for the serial monitor to connect.

  // First, let's check why we woke up and print it.
  print_wakeup_reason();

  // If the wakeup was caused by a touch, let's find out which pad.
  if (esp_sleep_get_wakeup_cause() == ESP_SLEEP_WAKEUP_TOUCHPAD) {
    print_wakeup_touchpad();
  }

  Serial.println("Configuring touch pads for deep sleep wakeup...");

  // Step 1: Initialize the touch peripheral
  touch_pad_init();

  // Step 2: Set the voltage range for touch sensing.
  // These are standard values that work well in most cases.
  touch_pad_set_voltage(TOUCH_HVOLT_2V7, TOUCH_LVOLT_0V5, TOUCH_HVOLT_ATTEN_1V);

  // Step 3: Configure the specific touch pads we want to use for wakeup.
  touch_pad_config(TOUCH_PAD_NUM8);
  touch_pad_config(TOUCH_PAD_NUM9);

  // Step 4: Calibrate the pads to set a dynamic wakeup threshold.
  uint16_t touch_value_8, touch_value_9;

  // Read the initial, baseline values from the pads.
  touch_pad_read_filtered(TOUCH_PAD_NUM8, &touch_value_8);
  touch_pad_read_filtered(TOUCH_PAD_NUM9, &touch_value_9);

  Serial.printf("Initial Baseline Value on Pad 8: %d\n", touch_value_8);
  Serial.printf("Initial Baseline Value on Pad 9: %d\n", touch_value_9);

  // Calculate the wakeup threshold for each pad based on the percentage defined.
  uint16_t threshold_8 = touch_value_8 * TOUCH_THRESHOLD_PERCENTAGE / 100;
  uint16_t threshold_9 = touch_value_9 * TOUCH_THRESHOLD_PERCENTAGE / 100;

  // Apply the calculated threshold to each pad.
  touch_pad_set_thresh(TOUCH_PAD_NUM8, threshold_8);
  touch_pad_set_thresh(TOUCH_PAD_NUM9, threshold_9);

  Serial.printf("Wakeup Threshold for Pad 8 set to: %d\n", threshold_8);
  Serial.printf("Wakeup Threshold for Pad 9 set to: %d\n", threshold_9);

  // Step 5: Configure the trigger mode. ESP_TOUCH_TRIGGER_BELOW means the
  // wakeup will happen when the sensor value drops BELOW the threshold.
  touch_pad_set_trigger_mode(ESP_TOUCH_TRIGGER_BELOW);

  // Step 6: Enable the touchpad as a wakeup source.
  esp_sleep_enable_touchpad_wakeup();

  Serial.println("------------------------------------------");
  Serial.println("ESP32-S3 is going into deep sleep now.");
  Serial.println("Touch Pad 8 or 9 to wake it up.");
  Serial.println("------------------------------------------");
  Serial.flush(); // Ensure all serial messages are sent before sleeping.

  // Step 7: Enter deep sleep.
  esp_deep_sleep_start();
}

void loop() {
  // This part of the code is never reached.
  // The ESP32 wakes up and restarts the setup() function.
}

/**
 * @brief Prints the reason why the ESP32 has woken up from sleep.
 */
void print_wakeup_reason() {
  esp_sleep_wakeup_cause_t wakeup_reason = esp_sleep_get_wakeup_cause();

  switch (wakeup_reason) {
    case ESP_SLEEP_WAKEUP_TOUCHPAD:
      Serial.println("Wakeup caused by Touch Pad");
      break;
    case ESP_SLEEP_WAKEUP_TIMER:
      Serial.println("Wakeup caused by Timer");
      break;
    case ESP_SLEEP_WAKEUP_EXT0:
      Serial.println("Wakeup caused by external signal using RTC_IO");
      break;
    case ESP_SLEEP_WAKEUP_EXT1:
      Serial.println("Wakeup caused by external signal using RTC_CNTL");
      break;
    case ESP_SLEEP_WAKEUP_ULP:
      Serial.println("Wakeup caused by ULP program");
      break;
    default:
      Serial.printf("Wakeup reason: %d (Not from deep sleep)\n", wakeup_reason);
      break;
  }
}

/**
 * @brief Identifies which touch pad caused the wakeup and prints it.
 */
void print_wakeup_touchpad() {
  touch_pad_t touch_pad_num = esp_sleep_get_touchpad_wakeup_status();

  switch (touch_pad_num) {
    case TOUCH_PAD_NUM8:
      Serial.println("********** Wakeup detected on Pad 8 **********");
      break;
    case TOUCH_PAD_NUM9:
      Serial.println("********** Wakeup detected on Pad 9 **********");
      break;
    default:
      Serial.println("Unknown touch pad triggered wakeup");
      break;
  }
}

#include "displayapp/screens/WatchFacePraxiomDark.h"
#include <date/date.h>
#include <lvgl/lvgl.h>
#include <cstdio>
#include <cmath>
#include "displayapp/screens/BatteryIcon.h"
#include "displayapp/screens/Symbols.h"
#include "displayapp/DisplayApp.h"
#include "components/settings/Settings.h"

using namespace Pinetime::Applications::Screens;

namespace {
  void RefreshTaskCallback(lv_task_t* task) {
    auto* screen = static_cast<WatchFacePraxiomDark*>(task->user_data);
    screen->Refresh();
  }
}

WatchFacePraxiomDark::WatchFacePraxiomDark(DisplayApp* app,
                                         Controllers::DateTime& dateTimeController,
                                         const Controllers::Battery& batteryController,
                                         Controllers::Ble& bleController,
                                         Controllers::NotificationManager& notificationManager,
                                         Controllers::Settings& settingsController,
                                         Controllers::HeartRateController& heartRateController,
                                         Controllers::MotionController& motionController)
  : Screen(app),
    dateTimeController {dateTimeController},
    batteryController {batteryController},
    bleController {bleController},
    notificationManager {notificationManager},
    settingsController {settingsController},
    heartRateController {heartRateController},
    motionController {motionController} {

  // Dark theme background - Deep purple to black gradient
  backgroundLabel = lv_label_create(lv_scr_act(), nullptr);
  lv_obj_set_style_local_bg_color(backgroundLabel, LV_LABEL_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_MAKE(0x0A, 0x0A, 0x1A));
  lv_obj_set_style_local_bg_opa(backgroundLabel, LV_LABEL_PART_MAIN, LV_STATE_DEFAULT, LV_OPA_COVER);
  lv_label_set_long_mode(backgroundLabel, LV_LABEL_LONG_CROP);
  lv_obj_set_size(backgroundLabel, 240, 240);
  lv_obj_set_pos(backgroundLabel, 0, 0);
  lv_label_set_text_static(backgroundLabel, "");

  // DNA Helix decorative elements (Purple/Blue gradient colors with glow effect)
  dnaHelix1 = lv_obj_create(lv_scr_act(), nullptr);
  lv_obj_set_style_local_bg_color(dnaHelix1, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_MAKE(0x66, 0x33, 0xFF));
  lv_obj_set_style_local_bg_opa(dnaHelix1, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, LV_OPA_70);
  lv_obj_set_style_local_radius(dnaHelix1, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, LV_RADIUS_CIRCLE);
  lv_obj_set_style_local_border_color(dnaHelix1, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_MAKE(0x99, 0x66, 0xFF));
  lv_obj_set_style_local_border_width(dnaHelix1, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, 1);
  lv_obj_set_style_local_border_opa(dnaHelix1, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, LV_OPA_50);
  lv_obj_set_size(dnaHelix1, 10, 10);
  lv_obj_set_pos(dnaHelix1, 20, 80);

  dnaHelix2 = lv_obj_create(lv_scr_act(), nullptr);
  lv_obj_set_style_local_bg_color(dnaHelix2, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_MAKE(0xFF, 0x33, 0x99));
  lv_obj_set_style_local_bg_opa(dnaHelix2, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, LV_OPA_70);
  lv_obj_set_style_local_radius(dnaHelix2, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, LV_RADIUS_CIRCLE);
  lv_obj_set_style_local_border_color(dnaHelix2, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_MAKE(0xFF, 0x66, 0xCC));
  lv_obj_set_style_local_border_width(dnaHelix2, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, 1);
  lv_obj_set_style_local_border_opa(dnaHelix2, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, LV_OPA_50);
  lv_obj_set_size(dnaHelix2, 10, 10);
  lv_obj_set_pos(dnaHelix2, 210, 80);

  // Additional DNA strands for visual effect
  for (int i = 0; i < 3; i++) {
    lv_obj_t* strand = lv_obj_create(lv_scr_act(), nullptr);
    lv_obj_set_style_local_bg_color(strand, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_MAKE(0x99, 0x33, 0xFF));
    lv_obj_set_style_local_bg_opa(strand, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, LV_OPA_20);
    lv_obj_set_size(strand, 4, 4);
    lv_obj_set_pos(strand, 20 + (i * 95), 100 + (i * 10));
  }

  // PRAXIOM branding - Glowing purple-blue gradient text
  praxiomLogo = lv_label_create(lv_scr_act(), nullptr);
  lv_obj_set_style_local_text_color(praxiomLogo, LV_LABEL_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_MAKE(0x99, 0x66, 0xFF));
  lv_label_set_text_static(praxiomLogo, "PRAXIOM HEALTH");
  lv_obj_align(praxiomLogo, nullptr, LV_ALIGN_IN_TOP_MID, 0, 5);

  // Time display - Bright white with subtle glow
  label_time = lv_label_create(lv_scr_act(), nullptr);
  lv_obj_set_style_local_text_color(label_time, LV_LABEL_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_WHITE);
  lv_label_set_text(label_time, "00:00");
  lv_obj_align(label_time, nullptr, LV_ALIGN_CENTER, 0, -40);

  // Date display - Soft blue
  label_date = lv_label_create(lv_scr_act(), nullptr);
  lv_obj_set_style_local_text_color(label_date, LV_LABEL_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_MAKE(0x66, 0xCC, 0xFF));
  lv_label_set_text(label_date, "MON 01 JAN");
  lv_obj_align(label_date, nullptr, LV_ALIGN_CENTER, 0, -10);

  // Health Score Section (Praxiom Algorithm) - Glowing effect
  healthScoreLabel = lv_label_create(lv_scr_act(), nullptr);
  lv_obj_set_style_local_text_color(healthScoreLabel, LV_LABEL_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_MAKE(0x00, 0xFF, 0xCC));
  lv_label_set_text_static(healthScoreLabel, "Health Score");
  lv_obj_align(healthScoreLabel, nullptr, LV_ALIGN_CENTER, 0, 20);

  healthFeedback = lv_label_create(lv_scr_act(), nullptr);
  lv_label_set_text(healthFeedback, "OPTIMAL");
  lv_obj_align(healthFeedback, healthScoreLabel, LV_ALIGN_OUT_BOTTOM_MID, 0, 5);

  // Create health metrics container with dark background
  lv_obj_t* metricsContainer = lv_cont_create(lv_scr_act(), nullptr);
  lv_obj_set_style_local_bg_color(metricsContainer, LV_CONT_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_MAKE(0x00, 0x00, 0x00));
  lv_obj_set_style_local_bg_opa(metricsContainer, LV_CONT_PART_MAIN, LV_STATE_DEFAULT, LV_OPA_50);
  lv_obj_set_style_local_border_color(metricsContainer, LV_CONT_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_MAKE(0x66, 0x33, 0xFF));
  lv_obj_set_style_local_border_width(metricsContainer, LV_CONT_PART_MAIN, LV_STATE_DEFAULT, 1);
  lv_obj_set_style_local_border_opa(metricsContainer, LV_CONT_PART_MAIN, LV_STATE_DEFAULT, LV_OPA_60);
  lv_obj_set_style_local_radius(metricsContainer, LV_CONT_PART_MAIN, LV_STATE_DEFAULT, 10);
  lv_obj_set_size(metricsContainer, 220, 60);
  lv_obj_align(metricsContainer, nullptr, LV_ALIGN_IN_BOTTOM_MID, 0, -10);

  // Heart Rate with HRV - Glowing red
  heartRateLabel = lv_label_create(metricsContainer, nullptr);
  lv_obj_set_style_local_text_color(heartRateLabel, LV_LABEL_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_MAKE(0xFF, 0x33, 0x66));
  lv_label_set_text_static(heartRateLabel, Symbols::heartBeat);
  lv_obj_align(heartRateLabel, nullptr, LV_ALIGN_IN_LEFT_MID, 10, -10);

  heartRateValue = lv_label_create(metricsContainer, nullptr);
  lv_obj_set_style_local_text_color(heartRateValue, LV_LABEL_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_WHITE);
  lv_label_set_text(heartRateValue, "--");
  lv_obj_align(heartRateValue, heartRateLabel, LV_ALIGN_OUT_RIGHT_MID, 5, 0);

  // HRV indicator
  hrvLabel = lv_label_create(metricsContainer, nullptr);
  lv_obj_set_style_local_text_color(hrvLabel, LV_LABEL_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_MAKE(0xFF, 0x99, 0xCC));
  lv_label_set_text_static(hrvLabel, "HRV:");
  lv_obj_align(hrvLabel, heartRateLabel, LV_ALIGN_OUT_BOTTOM_LEFT, 0, 5);

  hrvValueLabel = lv_label_create(metricsContainer, nullptr);
  lv_obj_set_style_local_text_color(hrvValueLabel, LV_LABEL_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_WHITE);
  lv_label_set_text(hrvValueLabel, "--");
  lv_obj_align(hrvValueLabel, hrvLabel, LV_ALIGN_OUT_RIGHT_MID, 5, 0);

  // Steps - Glowing blue
  stepsLabel = lv_label_create(metricsContainer, nullptr);
  lv_obj_set_style_local_text_color(stepsLabel, LV_LABEL_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_MAKE(0x33, 0xCC, 0xFF));
  lv_label_set_text_static(stepsLabel, Symbols::shoe);
  lv_obj_align(stepsLabel, nullptr, LV_ALIGN_IN_RIGHT_MID, -60, -10);

  stepsValue = lv_label_create(metricsContainer, nullptr);
  lv_obj_set_style_local_text_color(stepsValue, LV_LABEL_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_WHITE);
  lv_label_set_text(stepsValue, "0");
  lv_obj_align(stepsValue, stepsLabel, LV_ALIGN_OUT_RIGHT_MID, 5, 0);

  // Activity Score - Glowing purple
  activityLabel = lv_label_create(metricsContainer, nullptr);
  lv_obj_set_style_local_text_color(activityLabel, LV_LABEL_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_MAKE(0xCC, 0x66, 0xFF));
  lv_label_set_text_static(activityLabel, "Act:");
  lv_obj_align(activityLabel, stepsLabel, LV_ALIGN_OUT_BOTTOM_LEFT, 0, 5);

  activityValue = lv_label_create(metricsContainer, nullptr);
  lv_obj_set_style_local_text_color(activityValue, LV_LABEL_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_WHITE);
  lv_label_set_text(activityValue, "0%");
  lv_obj_align(activityValue, activityLabel, LV_ALIGN_OUT_RIGHT_MID, 5, 0);

  // Battery icon (subtle, top right)
  batteryIcon = lv_label_create(lv_scr_act(), nullptr);
  lv_obj_set_style_local_text_color(batteryIcon, LV_LABEL_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_MAKE(0x99, 0x99, 0x99));
  lv_label_set_text(batteryIcon, Symbols::batteryFull);
  lv_obj_align(batteryIcon, nullptr, LV_ALIGN_IN_TOP_RIGHT, -10, 5);

  taskRefresh = lv_task_create(RefreshTaskCallback, LV_DISP_DEF_REFR_PERIOD, LV_TASK_PRIO_MID, this);
  Refresh();
}

// The rest of the implementation is identical to Light theme but inherits from WatchFacePraxiomDark
// Copy the same methods from WatchFacePraxiomLight but update class references

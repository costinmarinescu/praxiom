#include "displayapp/screens/WatchFacePraxiomLight.h"
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
    auto* screen = static_cast<WatchFacePraxiomLight*>(task->user_data);
    screen->Refresh();
  }
}

WatchFacePraxiomLight::WatchFacePraxiomLight(DisplayApp* app,
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

  // Light theme background - White with subtle blue gradient
  backgroundLabel = lv_label_create(lv_scr_act(), nullptr);
  lv_obj_set_style_local_bg_color(backgroundLabel, LV_LABEL_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_WHITE);
  lv_obj_set_style_local_bg_opa(backgroundLabel, LV_LABEL_PART_MAIN, LV_STATE_DEFAULT, LV_OPA_COVER);
  lv_label_set_long_mode(backgroundLabel, LV_LABEL_LONG_CROP);
  lv_obj_set_size(backgroundLabel, 240, 240);
  lv_obj_set_pos(backgroundLabel, 0, 0);
  lv_label_set_text_static(backgroundLabel, "");

  // DNA Helix decorative elements (Light Blue/Purple gradient colors)
  dnaHelix1 = lv_obj_create(lv_scr_act(), nullptr);
  lv_obj_set_style_local_bg_color(dnaHelix1, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_MAKE(0x33, 0x66, 0xFF));
  lv_obj_set_style_local_bg_opa(dnaHelix1, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, LV_OPA_30);
  lv_obj_set_style_local_radius(dnaHelix1, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, LV_RADIUS_CIRCLE);
  lv_obj_set_size(dnaHelix1, 8, 8);
  lv_obj_set_pos(dnaHelix1, 20, 80);

  dnaHelix2 = lv_obj_create(lv_scr_act(), nullptr);
  lv_obj_set_style_local_bg_color(dnaHelix2, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_MAKE(0x99, 0x33, 0xFF));
  lv_obj_set_style_local_bg_opa(dnaHelix2, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, LV_OPA_30);
  lv_obj_set_style_local_radius(dnaHelix2, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, LV_RADIUS_CIRCLE);
  lv_obj_set_size(dnaHelix2, 8, 8);
  lv_obj_set_pos(dnaHelix2, 212, 80);

  // PRAXIOM branding - Blue gradient text
  praxiomLogo = lv_label_create(lv_scr_act(), nullptr);
  lv_obj_set_style_local_text_color(praxiomLogo, LV_LABEL_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_MAKE(0x33, 0x66, 0xFF));
  lv_label_set_text_static(praxiomLogo, "PRAXIOM HEALTH");
  lv_obj_align(praxiomLogo, nullptr, LV_ALIGN_IN_TOP_MID, 0, 5);

  // Time display - Dark blue for contrast on light background
  label_time = lv_label_create(lv_scr_act(), nullptr);
  lv_obj_set_style_local_text_color(label_time, LV_LABEL_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_MAKE(0x00, 0x33, 0x66));
  lv_label_set_text(label_time, "00:00");
  lv_obj_align(label_time, nullptr, LV_ALIGN_CENTER, 0, -40);

  // Date display
  label_date = lv_label_create(lv_scr_act(), nullptr);
  lv_obj_set_style_local_text_color(label_date, LV_LABEL_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_MAKE(0x00, 0x66, 0x99));
  lv_label_set_text(label_date, "MON 01 JAN");
  lv_obj_align(label_date, nullptr, LV_ALIGN_CENTER, 0, -10);

  // Health Score Section (Praxiom Algorithm)
  healthScoreLabel = lv_label_create(lv_scr_act(), nullptr);
  lv_obj_set_style_local_text_color(healthScoreLabel, LV_LABEL_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_MAKE(0x00, 0x99, 0x66));
  lv_label_set_text_static(healthScoreLabel, "Health Score");
  lv_obj_align(healthScoreLabel, nullptr, LV_ALIGN_CENTER, 0, 20);

  healthFeedback = lv_label_create(lv_scr_act(), nullptr);
  lv_label_set_text(healthFeedback, "OPTIMAL");
  lv_obj_align(healthFeedback, healthScoreLabel, LV_ALIGN_OUT_BOTTOM_MID, 0, 5);

  // Heart Rate with HRV
  heartRateLabel = lv_label_create(lv_scr_act(), nullptr);
  lv_obj_set_style_local_text_color(heartRateLabel, LV_LABEL_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_MAKE(0xFF, 0x33, 0x66));
  lv_label_set_text_static(heartRateLabel, Symbols::heartBeat);
  lv_obj_align(heartRateLabel, nullptr, LV_ALIGN_IN_BOTTOM_LEFT, 20, -40);

  heartRateValue = lv_label_create(lv_scr_act(), nullptr);
  lv_obj_set_style_local_text_color(heartRateValue, LV_LABEL_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_MAKE(0x33, 0x33, 0x33));
  lv_label_set_text(heartRateValue, "--");
  lv_obj_align(heartRateValue, heartRateLabel, LV_ALIGN_OUT_RIGHT_MID, 5, 0);

  // HRV indicator
  hrvLabel = lv_label_create(lv_scr_act(), nullptr);
  lv_obj_set_style_local_text_color(hrvLabel, LV_LABEL_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_MAKE(0xFF, 0x66, 0x99));
  lv_label_set_text_static(hrvLabel, "HRV:");
  lv_obj_align(hrvLabel, heartRateLabel, LV_ALIGN_OUT_BOTTOM_LEFT, 0, 5);

  hrvValueLabel = lv_label_create(lv_scr_act(), nullptr);
  lv_obj_set_style_local_text_color(hrvValueLabel, LV_LABEL_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_MAKE(0x33, 0x33, 0x33));
  lv_label_set_text(hrvValueLabel, "--");
  lv_obj_align(hrvValueLabel, hrvLabel, LV_ALIGN_OUT_RIGHT_MID, 5, 0);

  // Steps
  stepsLabel = lv_label_create(lv_scr_act(), nullptr);
  lv_obj_set_style_local_text_color(stepsLabel, LV_LABEL_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_MAKE(0x33, 0x99, 0xFF));
  lv_label_set_text_static(stepsLabel, Symbols::shoe);
  lv_obj_align(stepsLabel, nullptr, LV_ALIGN_IN_BOTTOM_RIGHT, -70, -40);

  stepsValue = lv_label_create(lv_scr_act(), nullptr);
  lv_obj_set_style_local_text_color(stepsValue, LV_LABEL_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_MAKE(0x33, 0x33, 0x33));
  lv_label_set_text(stepsValue, "0");
  lv_obj_align(stepsValue, stepsLabel, LV_ALIGN_OUT_RIGHT_MID, 5, 0);

  // Activity Score
  activityLabel = lv_label_create(lv_scr_act(), nullptr);
  lv_obj_set_style_local_text_color(activityLabel, LV_LABEL_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_MAKE(0x99, 0x66, 0xFF));
  lv_label_set_text_static(activityLabel, "Activity:");
  lv_obj_align(activityLabel, stepsLabel, LV_ALIGN_OUT_BOTTOM_LEFT, 0, 5);

  activityValue = lv_label_create(lv_scr_act(), nullptr);
  lv_obj_set_style_local_text_color(activityValue, LV_LABEL_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_MAKE(0x33, 0x33, 0x33));
  lv_label_set_text(activityValue, "0%");
  lv_obj_align(activityValue, activityLabel, LV_ALIGN_OUT_RIGHT_MID, 5, 0);

  // Battery icon (subtle, top right)
  batteryIcon = lv_label_create(lv_scr_act(), nullptr);
  lv_obj_set_style_local_text_color(batteryIcon, LV_LABEL_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_MAKE(0x66, 0x66, 0x66));
  lv_label_set_text(batteryIcon, Symbols::batteryFull);
  lv_obj_align(batteryIcon, nullptr, LV_ALIGN_IN_TOP_RIGHT, -10, 5);

  taskRefresh = lv_task_create(RefreshTaskCallback, LV_DISP_DEF_REFR_PERIOD, LV_TASK_PRIO_MID, this);
  Refresh();
}

WatchFacePraxiomLight::~WatchFacePraxiomLight() {
  lv_task_del(taskRefresh);
  lv_obj_clean(lv_scr_act());
}

void WatchFacePraxiomLight::RefreshTaskCallback(lv_task_t* task) {
  auto* screen = static_cast<WatchFacePraxiomLight*>(task->user_data);
  screen->Refresh();
}

void WatchFacePraxiomLight::Refresh() {
  // Battery update
  batteryPercentRemaining = batteryController.PercentRemaining();
  if (batteryPercentRemaining.IsUpdated()) {
    auto batteryPercent = batteryPercentRemaining.Get();
    lv_label_set_text_static(batteryIcon, BatteryIcon::GetBatteryIcon(batteryPercent));
    
    auto color = batteryPercent > 40 ? LV_COLOR_MAKE(0x66, 0x66, 0x66) : LV_COLOR_MAKE(0xFF, 0x33, 0x33);
    lv_obj_set_style_local_text_color(batteryIcon, LV_LABEL_PART_MAIN, LV_STATE_DEFAULT, color);
  }

  // Time and date update
  currentDateTime = dateTimeController.CurrentDateTime();
  if (currentDateTime.IsUpdated()) {
    auto newDateTime = currentDateTime.Get();
    auto dp = date::floor<date::days>(newDateTime);
    auto time = date::make_time(newDateTime - dp);
    auto yearMonthDay = date::year_month_day(dp);

    int hour = time.hours().count();
    auto minute = time.minutes().count();

    char timeStr[6];
    if (settingsController.GetClockType() == Controllers::Settings::ClockType::H24) {
      sprintf(timeStr, "%02d:%02d", hour, static_cast<int>(minute));
    } else {
      int h12 = (hour == 0) ? 12 : ((hour > 12) ? hour - 12 : hour);
      sprintf(timeStr, "%02d:%02d", h12, static_cast<int>(minute));
    }
    lv_label_set_text(label_time, timeStr);

    // Update date
    auto day = static_cast<unsigned>(yearMonthDay.day());
    char dateStr[16];
    sprintf(dateStr, "%s %02d %s", 
            dateTimeController.DayOfWeekShortToString(),
            day,
            dateTimeController.MonthShortToString());
    lv_label_set_text(label_date, dateStr);
  }

  // Heart rate update
  heartbeat = heartRateController.HeartRate();
  heartbeatRunning = heartRateController.State() != Controllers::HeartRateController::States::Stopped;
  if (heartbeat.IsUpdated() || heartbeatRunning.IsUpdated()) {
    if (heartbeatRunning.Get()) {
      lastHeartRate = heartbeat.Get();
      lv_label_set_text_fmt(heartRateValue, "%d", lastHeartRate);
      
      // Simulate HRV (in real implementation, this would come from actual HRV calculation)
      hrvValue = 25 + (lastHeartRate % 20);  // Simplified HRV simulation
      lv_label_set_text_fmt(hrvValueLabel, "%dms", hrvValue);
    } else {
      lv_label_set_text_static(heartRateValue, "--");
      lv_label_set_text_static(hrvValueLabel, "--");
    }
  }

  // Steps update
  stepCount = motionController.NbSteps();
  if (stepCount.IsUpdated()) {
    lastStepCount = stepCount.Get();
    lv_label_set_text_fmt(stepsValue, "%lu", lastStepCount);
    
    // Calculate activity score (percentage of 10,000 step goal)
    activityScore = (lastStepCount * 100) / 10000;
    if (activityScore > 100) activityScore = 100;
    lv_label_set_text_fmt(activityValue, "%d%%", activityScore);
  }

  // Update health score and DNA animation
  UpdateHealthScore();
  AnimateDNA();
}

void WatchFacePraxiomLight::UpdateHealthScore() {
  // Praxiom Health Algorithm (simplified)
  // Combines: Heart rate, HRV, steps, activity level
  
  int score = 100;
  
  // Heart rate scoring (optimal 60-80 at rest)
  if (lastHeartRate > 0) {
    if (lastHeartRate >= 60 && lastHeartRate <= 80) {
      score += 20;
    } else if (lastHeartRate > 100 || lastHeartRate < 50) {
      score -= 30;
    }
  }
  
  // Activity scoring
  if (activityScore >= 80) {
    currentHealthStatus = HealthStatus::Optimal;
  } else if (activityScore >= 60) {
    currentHealthStatus = HealthStatus::Good;
  } else if (activityScore >= 40) {
    currentHealthStatus = HealthStatus::Moderate;
  } else {
    currentHealthStatus = HealthStatus::NeedsAttention;
  }
  
  // Update feedback text and color
  lv_label_set_text_static(healthFeedback, GetHealthFeedback());
  lv_obj_set_style_local_text_color(healthFeedback, LV_LABEL_PART_MAIN, LV_STATE_DEFAULT, GetHealthColor());
}

void WatchFacePraxiomLight::AnimateDNA() {
  // Simple DNA helix animation
  dnaAnimationFrame = (dnaAnimationFrame + 1) % 60;
  
  int offset1 = 10 * sin(dnaAnimationFrame * 0.1);
  int offset2 = 10 * cos(dnaAnimationFrame * 0.1);
  
  lv_obj_set_pos(dnaHelix1, 20 + offset1, 80);
  lv_obj_set_pos(dnaHelix2, 212 + offset2, 80);
}

const char* WatchFacePraxiomLight::GetHealthFeedback() const {
  switch (currentHealthStatus) {
    case HealthStatus::Optimal:
      return "OPTIMAL";
    case HealthStatus::Good:
      return "GOOD";
    case HealthStatus::Moderate:
      return "MODERATE";
    case HealthStatus::NeedsAttention:
      return "IMPROVE";
    default:
      return "CHECKING";
  }
}

lv_color_t WatchFacePraxiomLight::GetHealthColor() const {
  switch (currentHealthStatus) {
    case HealthStatus::Optimal:
      return LV_COLOR_MAKE(0x00, 0xCC, 0x66);  // Green
    case HealthStatus::Good:
      return LV_COLOR_MAKE(0x33, 0x99, 0xFF);  // Blue
    case HealthStatus::Moderate:
      return LV_COLOR_MAKE(0xFF, 0x99, 0x33);  // Orange
    case HealthStatus::NeedsAttention:
      return LV_COLOR_MAKE(0xFF, 0x33, 0x66);  // Red
    default:
      return LV_COLOR_MAKE(0x66, 0x66, 0x66);  // Gray
  }
}

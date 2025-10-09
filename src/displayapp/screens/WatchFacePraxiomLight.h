#pragma once

#include <lvgl/src/lv_core/lv_obj.h>
#include <chrono>
#include <cstdint>
#include <memory>
#include "displayapp/screens/Screen.h"
#include "components/datetime/DateTimeController.h"
#include "components/battery/BatteryController.h"
#include "components/ble/BleController.h"
#include "components/ble/NotificationManager.h"
#include "components/heartrate/HeartRateController.h"
#include "components/motion/MotionController.h"
#include "utility/DirtyValue.h"

namespace Pinetime {
  namespace Controllers {
    class Settings;
  }

  namespace Applications {
    namespace Screens {
      class WatchFacePraxiomLight : public Screen {
      public:
        WatchFacePraxiomLight(DisplayApp* app,
                             Controllers::DateTime& dateTimeController,
                             const Controllers::Battery& batteryController,
                             Controllers::Ble& bleController,
                             Controllers::NotificationManager& notificationManager,
                             Controllers::Settings& settingsController,
                             Controllers::HeartRateController& heartRateController,
                             Controllers::MotionController& motionController);
        
        ~WatchFacePraxiomLight() override;
        void Refresh() override;

      private:
        // DNA helix animation state
        uint8_t dnaAnimationFrame = 0;
        
        // Time tracking
        uint8_t sHour, sMinute, sSecond;
        uint16_t currentYear = 1970;
        Controllers::DateTime::Months currentMonth = Pinetime::Controllers::DateTime::Months::Unknown;
        Controllers::DateTime::Days currentDayOfWeek = Pinetime::Controllers::DateTime::Days::Unknown;
        uint8_t currentDay = 0;

        // Health metrics
        uint32_t lastStepCount = 0;
        uint8_t lastHeartRate = 0;
        uint16_t activityScore = 0;
        uint8_t hrvValue = 0;
        
        // Praxiom Health Algorithm feedback
        enum class HealthStatus : uint8_t {
          Optimal,
          Good,
          Moderate,
          NeedsAttention
        };
        HealthStatus currentHealthStatus = HealthStatus::Good;

        // Dirty values for efficient updates
        Utility::DirtyValue<uint8_t> batteryPercentRemaining {};
        Utility::DirtyValue<bool> powerPresent {};
        Utility::DirtyValue<bool> bleState {};
        Utility::DirtyValue<std::chrono::time_point<std::chrono::system_clock, std::chrono::nanoseconds>> currentDateTime {};
        Utility::DirtyValue<uint32_t> stepCount {};
        Utility::DirtyValue<uint8_t> heartbeat {};
        Utility::DirtyValue<bool> heartbeatRunning {};

        // LVGL objects
        lv_obj_t* backgroundLabel;
        lv_obj_t* dnaHelix1;
        lv_obj_t* dnaHelix2;
        lv_obj_t* label_time;
        lv_obj_t* label_date;
        lv_obj_t* healthScoreLabel;
        lv_obj_t* heartRateLabel;
        lv_obj_t* heartRateValue;
        lv_obj_t* stepsLabel;
        lv_obj_t* stepsValue;
        lv_obj_t* activityLabel;
        lv_obj_t* activityValue;
        lv_obj_t* hrvLabel;
        lv_obj_t* hrvValueLabel;
        lv_obj_t* praxiomLogo;
        lv_obj_t* healthFeedback;
        lv_obj_t* batteryIcon;
        
        // Controllers
        Controllers::DateTime& dateTimeController;
        const Controllers::Battery& batteryController;
        Controllers::Ble& bleController;
        Controllers::NotificationManager& notificationManager;
        Controllers::Settings& settingsController;
        Controllers::HeartRateController& heartRateController;
        Controllers::MotionController& motionController;

        lv_task_t* taskRefresh;
        
        static void RefreshTaskCallback(lv_task_t* task);
        void UpdateHealthScore();
        void AnimateDNA();
        const char* GetHealthFeedback() const;
        lv_color_t GetHealthColor() const;
      };
    }
  }
}

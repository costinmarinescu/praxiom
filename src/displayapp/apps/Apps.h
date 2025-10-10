#pragma once

#include <cstdint>
#include <cstddef>

namespace Pinetime {
  namespace Applications {
    
    // Health-focused apps only - ALL GAMES REMOVED
    enum class Apps : uint8_t {
      None,
      Clock,
      SysInfo,
      FirmwareUpdate,
      FirmwareValidation,
      NotificationsPreview,
      Notifications,
      FlashLight,
      BatteryInfo,
      
      // Health & Fitness Apps ONLY
      HeartRate,
      Motion,
      Steps,
      
      // Essential Tools (No Games!)
      StopWatch,
      Timer,
      Alarm,
      
      // Settings
      Settings,
      SettingWatchFace,
      SettingTimeFormat,
      SettingDisplay,
      SettingWakeUp,
      SettingSteps,
      SettingSetDateTime,
      SettingSetDate,
      SettingSetTime,
      SettingChimes,
      SettingShakeThreshold,
      SettingBluetooth,
      SettingBatteryHistory,
      
      // Music and Navigation (optional, remove if not needed)
      Music,
      Navigation,
      
      // Weather
      Weather,
      
      // NO GAMES - Paddle, Twos, Paint, Metronome, Calculator REMOVED
      
      Error
    };
  }
}

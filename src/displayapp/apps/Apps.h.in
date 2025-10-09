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
      
      // Health & Fitness Apps ONLY
      HeartRate,
      Steps,
      Motion,
      
      // Essential Tools (No Games!)
      Timer,
      Alarm,
      StopWatch,
      
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
      
      // System
      BatteryInfo,
      Notifications,
      FlashLight,
      
      Error
    };
    
    // Touch Events enum (if not defined elsewhere)
    enum class TouchEvents : uint8_t {
      None = 0,
      Tap = 1,
      SwipeLeft = 2,
      SwipeRight = 3,
      SwipeUp = 4,
      SwipeDown = 5,
      LongTap = 6,
      DoubleTap = 7
    };
    
    // Health metrics structure for Praxiom algorithm
    struct HealthMetrics {
      uint8_t heartRate;
      uint8_t hrvMs;          // Heart rate variability in milliseconds
      uint32_t steps;
      uint16_t activeMinutes;
      uint8_t stressLevel;    // 0-100 calculated stress
      uint8_t recoveryScore;  // 0-100 recovery score
    };
    
    // Praxiom Health Status
    enum class PraxiomHealthStatus : uint8_t {
      Optimal,      // All metrics excellent
      Good,         // Most metrics good
      Moderate,     // Some improvement needed
      Attention,    // Needs attention
      Alert         // Health alert condition
    };
  }
}

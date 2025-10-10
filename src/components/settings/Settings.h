#pragma once

#include <cstdint>
#include <bitset>
#include "components/brightness/BrightnessController.h"
#include "components/fs/FS.h"

namespace Pinetime {
  namespace Controllers {
    class Settings {
    public:
      // Only two watch faces: Praxiom Light and Praxiom Dark
      enum class ClockFace : uint8_t {
        PraxiomLight = 0,
        PraxiomDark = 1,
      };
      
      static constexpr uint8_t watchFaceCount = 2;  // Only 2 watch faces
      
      enum class Notification : uint8_t {
        On = 0,
        Off = 1,
        Sleep = 2
      };
      
      enum class ChimesOption : uint8_t {
        None = 0,
        Hours = 1,
        HalfHours = 2
      };
      
      enum class WakeUpMode : uint8_t {
        SingleTap = 0,
        DoubleTap = 1,
        RaiseWrist = 2,
        Shake = 3,
        LowerWrist = 4
      };
      
      enum class Colors : uint8_t {
        White = 0,
        Silver = 1,
        Gray = 2,
        Black = 3,
        Red = 4,
        Maroon = 5,
        Yellow = 6,
        Olive = 7,
        Lime = 8,
        Green = 9,
        Cyan = 10,
        Teal = 11,
        Blue = 12,
        Navy = 13,
        Magenta = 14,
        Purple = 15,
        Orange = 16,
        Pink = 17
      };
      
      enum class ClockType : uint8_t {
        H24 = 0,
        H12 = 1
      };
      
      struct PineTimeStyle {
        Colors ColorTime = Colors::Teal;
        Colors ColorBar = Colors::Teal;
        Colors ColorBG = Colors::Black;
      };
      
      Settings(Pinetime::Controllers::FS& fs);

      Settings(const Settings&) = delete;
      Settings& operator=(const Settings&) = delete;
      Settings(Settings&&) = delete;
      Settings& operator=(Settings&&) = delete;

      void Init();
      void SaveSettings();

      void SetClockFace(ClockFace face) {
        if (face != settings.clockFace) {
          settingsChanged = true;
        }
        settings.clockFace = face;
      }
      
      ClockFace GetClockFace() const {
        return settings.clockFace;
      }

      void SetTimeFormat(ClockType clockType) {
        if (clockType != settings.clockType) {
          settingsChanged = true;
        }
        settings.clockType = clockType;
      }
      
      ClockType GetClockType() const {
        return settings.clockType;
      }

      void SetNotificationStatus(Notification status) {
        if (status != settings.notificationStatus) {
          settingsChanged = true;
        }
        settings.notificationStatus = status;
      }
      
      Notification GetNotificationStatus() const {
        return settings.notificationStatus;
      }

      void SetScreenTimeOut(uint32_t timeout) {
        if (timeout != settings.screenTimeOut) {
          settingsChanged = true;
        }
        settings.screenTimeOut = timeout;
      }
      
      uint32_t GetScreenTimeOut() const {
        return settings.screenTimeOut;
      }

      void SetShakeThreshold(uint16_t thresh) {
        if (settings.shakeWakeThreshold != thresh) {
          settings.shakeWakeThreshold = thresh;
          settingsChanged = true;
        }
      }
      
      int16_t GetShakeThreshold() const {
        return settings.shakeWakeThreshold;
      }

      void SetBrightness(BrightnessController::Levels level) {
        if (level != settings.brightLevel) {
          settingsChanged = true;
        }
        settings.brightLevel = level;
      }
      
      BrightnessController::Levels GetBrightness() const {
        return settings.brightLevel;
      }

      void SetStepsGoal(uint32_t goal) {
        if (goal != settings.stepsGoal) {
          settingsChanged = true;
        }
        settings.stepsGoal = goal;
      }
      
      uint32_t GetStepsGoal() const {
        return settings.stepsGoal;
      }

      void SetBleRadioEnabled(bool enabled) {
        settings.bleRadioEnabled = enabled;
      }
      
      bool GetBleRadioEnabled() const {
        return settings.bleRadioEnabled;
      }

      void SetWakeUpMode(WakeUpMode wakeUp, bool enabled) {
        if (enabled != isWakeUpModeOn(wakeUp)) {
          settingsChanged = true;
        }
        settings.wakeUpMode.set(static_cast<size_t>(wakeUp), enabled);
      }
      
      std::bitset<5> getWakeUpModes() const {
        return settings.wakeUpMode;
      }
      
      bool isWakeUpModeOn(WakeUpMode mode) const {
        return getWakeUpModes()[static_cast<size_t>(mode)];
      }

      void SetChimeOption(ChimesOption chimeOption) {
        if (chimeOption != settings.chimesOption) {
          settingsChanged = true;
        }
        settings.chimesOption = chimeOption;
      }
      
      ChimesOption GetChimeOption() const {
        return settings.chimesOption;
      }

      void SetAppMenu(uint8_t menu) {
        appMenu = menu;
      }
      
      uint8_t GetAppMenu() const {
        return appMenu;
      }

      void SetSettingsMenu(uint8_t menu) {
        settingsMenu = menu;
      }
      
      uint8_t GetSettingsMenu() const {
        return settingsMenu;
      }

      void SetPTSColorTime(Colors colorTime) {
        if (colorTime != settings.PTS.ColorTime)
          settingsChanged = true;
        settings.PTS.ColorTime = colorTime;
      }
      
      Colors GetPTSColorTime() const {
        return settings.PTS.ColorTime;
      }

      void SetPTSColorBar(Colors colorBar) {
        if (colorBar != settings.PTS.ColorBar)
          settingsChanged = true;
        settings.PTS.ColorBar = colorBar;
      }
      
      Colors GetPTSColorBar() const {
        return settings.PTS.ColorBar;
      }

      void SetPTSColorBG(Colors colorBG) {
        if (colorBG != settings.PTS.ColorBG)
          settingsChanged = true;
        settings.PTS.ColorBG = colorBG;
      }
      
      Colors GetPTSColorBG() const {
        return settings.PTS.ColorBG;
      }

      void SetInfineatShowSideCover(bool show) {
        if (show != settings.watchFaceInfineat.showSideCover) {
          settings.watchFaceInfineat.showSideCover = show;
          settingsChanged = true;
        }
      }
      
      bool GetInfineatShowSideCover() const {
        return settings.watchFaceInfineat.showSideCover;
      }

      void SetInfineatColorIndex(uint8_t index) {
        if (index != settings.watchFaceInfineat.colorIndex) {
          settings.watchFaceInfineat.colorIndex = index;
          settingsChanged = true;
        }
      }
      
      uint8_t GetInfineatColorIndex() const {
        return settings.watchFaceInfineat.colorIndex;
      }

    private:
      Pinetime::Controllers::FS& fs;

      static constexpr uint32_t settingsVersion = 0x0007;
      
      struct SettingsData {
        uint32_t version = settingsVersion;
        uint32_t stepsGoal = 10000;
        uint32_t screenTimeOut = 15000;
        
        ClockType clockType = ClockType::H24;
        Notification notificationStatus = Notification::On;
        
        ClockFace clockFace = ClockFace::PraxiomLight;  // Default to light theme
        ChimesOption chimesOption = ChimesOption::None;
        
        PineTimeStyle PTS;
        
        std::bitset<5> wakeUpMode {0};
        uint16_t shakeWakeThreshold = 150;
        Controllers::BrightnessController::Levels brightLevel = Controllers::BrightnessController::Levels::Medium;
        
        struct WatchFaceInfineat {
          bool showSideCover = true;
          int colorIndex = 0;
        };
        
        WatchFaceInfineat watchFaceInfineat;
        
        bool bleRadioEnabled = true;
        uint8_t appMenu = 0;
        uint8_t settingsMenu = 0;
      };
      
      SettingsData settings;
      bool settingsChanged = false;
      
      uint8_t appMenu = 0;
      uint8_t settingsMenu = 0;

      void LoadSettingsFromFile();
      void SaveSettingsToFile();
    };
  }
}

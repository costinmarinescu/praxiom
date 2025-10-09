#pragma once

// This file provides forward declarations for controllers used by apps
// Place this file in src/displayapp/controllers/

#include <cstdint>

namespace Pinetime {
  namespace Controllers {
    // Forward declarations for all controllers
    class DateTime;
    class Battery;
    class Ble;
    class NotificationManager;
    class HeartRateController;
    class MotionController;
    class Settings;
    class MotorController;
    class AlarmController;
    class BrightnessController;
    class FS;
    class Timer;
    class TimerController;
    class Alarm;
    
    // Touch handler
    class TouchHandler;
    
    // System-related
    class SystemTask;
    class FirmwareValidator;
    
    // Display-related
    class DisplayApp;
  }
  
  namespace Applications {
    // Forward declarations for app-related classes
    class DisplayApp;
    class HeartRateTask;
    
    namespace Screens {
      class Screen;
    }
  }
  
  namespace System {
    class SystemTask;
    class BootloaderVersion;
  }
  
  namespace Drivers {
    class St7789;
    class Cst816S;
    class SpiMaster;
    class SpiNorFlash;
    class TwiMaster;
    class Hrs3300;
    class Bma421;
    class WatchdogView;
  }
}

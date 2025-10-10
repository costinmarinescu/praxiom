// File: src/displayapp/screens/settings/SettingWatchFace.h

#pragma once

#include <array>
#include <cstdint>
#include <memory>

#include "displayapp/screens/Screen.h"
#include "displayapp/screens/ScreenList.h"
#include "components/settings/Settings.h"

namespace Pinetime {
  namespace Controllers {
    class Settings;
    class FS;
  }

  namespace Applications {
    namespace Screens {
      class SettingWatchFace : public Screen {
      public:
        SettingWatchFace(DisplayApp* app,
                        Pinetime::Controllers::Settings& settingsController,
                        Pinetime::Controllers::FS& filesystem);
        ~SettingWatchFace() override;

        bool OnTouchEvent(TouchEvents event) override;

      private:
        struct Option {
          const char* name;
          uint32_t value;
          bool enabled = true;
        };

        Pinetime::Controllers::Settings& settingsController;
        Pinetime::Controllers::FS& filesystem;

        // Only 2 watch faces now
        static constexpr std::array<Option, 2> options;

        std::unique_ptr<Screen> CreateScreen();

        ScreenList<1> screens;  // Only 1 screen needed for 2 watch faces
      };
    }
  }
}

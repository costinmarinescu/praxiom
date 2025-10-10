// File: src/displayapp/screens/settings/SettingWatchFace.cpp

#include "displayapp/screens/settings/SettingWatchFace.h"
#include <lvgl/lvgl.h>
#include "displayapp/DisplayApp.h"
#include "displayapp/screens/CheckboxList.h"
#include "displayapp/screens/Screen.h"
#include "components/settings/Settings.h"

using namespace Pinetime::Applications::Screens;

constexpr std::array<SettingWatchFace::Option, 2> SettingWatchFace::options {{
  {"Praxiom Light", static_cast<uint32_t>(Pinetime::Controllers::Settings::ClockFace::PraxiomLight)},
  {"Praxiom Dark", static_cast<uint32_t>(Pinetime::Controllers::Settings::ClockFace::PraxiomDark)}
}};

SettingWatchFace::SettingWatchFace(Pinetime::Applications::DisplayApp* app,
                                   Pinetime::Controllers::Settings& settingsController,
                                   Pinetime::Controllers::FS& filesystem)
  : Screen(app),
    settingsController {settingsController},
    filesystem {filesystem},
    screens {app, 
            0,  // Only one page now with 2 watch faces
            {[this]() -> std::unique_ptr<Screen> {
              return CreateScreen();
            }},
            Screens::ScreenListModes::UpDown} {
}

SettingWatchFace::~SettingWatchFace() {
  lv_obj_clean(lv_scr_act());
}

bool SettingWatchFace::OnTouchEvent(Pinetime::Applications::TouchEvents event) {
  return screens.OnTouchEvent(event);
}

std::unique_ptr<Screen> SettingWatchFace::CreateScreen() {
  std::array<Screens::CheckboxList::Item, 2> watchFaceItems;
  for (size_t i = 0; i < options.size(); ++i) {
    watchFaceItems[i] = Screens::CheckboxList::Item {
      options[i].name,
      options[i].value == static_cast<uint32_t>(settingsController.GetClockFace()),
      options[i].enabled
    };
  }
  
  return std::make_unique<Screens::CheckboxList>(
    0,
    1,  // Only 1 screen
    app,
    "Watch face",
    Symbols::home,
    static_cast<uint32_t>(settingsController.GetClockFace()),
    [&settings = settingsController](uint32_t index) {
      settings.SetClockFace(static_cast<Controllers::Settings::ClockFace>(index));
      settings.SaveSettings();
    },
    watchFaceItems);
}

#pragma once

#include "Menu.h"
#include "MenuTypes.h"

#include <map>

namespace ssb64 {

class PortMenu : public Ship::Menu {
  public:
    PortMenu();
    ~PortMenu() override = default;

    void InitElement() override;
    void DrawElement() override;
    void UpdateElement() override;
    void Draw() override;

  protected:
    void AddSidebarEntry(std::string sectionName, std::string sidbarName, uint32_t columnCount);
    WidgetInfo& AddWidget(WidgetPath& pathInfo, std::string widgetName, WidgetType widgetType);
    void AddMenuElements();
    void AddMenuSettings();
    void AddMenuWindows();
    void AddMenuAssets();
    void AddMenuAbout();

  private:
    bool mShowReextractMessage = false;
    bool mMenuElementsInitialized = false;
};

} // namespace ssb64

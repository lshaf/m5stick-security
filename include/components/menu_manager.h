#pragma once
#include "menu_screen.h"
#include <vector>
#include <functional>
#include <M5StickCPlus2.h>

struct MenuItem {
    const char* label;
    std::function<void()> onSelect;
};

class MenuManager : public MenuScreen {
public:
    MenuManager();

    void updateScreen() override;
    void handleInput() override;
    void destroy() override {}

    int selected = 0;
    std::vector<MenuItem> menuItems;
};
#pragma once
#include "menu_screen.h"
#include <vector>
#include <functional>

struct MenuItem {
    const char* label;
    std::function<void()> onSelect;
};

class MenuManager : public MenuScreen {
public:
    MenuManager();

    void updateScreen() override;
    void handleInput() override;
    void destroy() override {};
    void renderMenu();

    int selected = 0;
    std::vector<MenuItem> menuItems;
    String title = "";
    int menuY = 18;
};
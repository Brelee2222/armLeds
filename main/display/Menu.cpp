#include "Menu.h"


Menu* Menu::currentMenu;

Menu::Menu() { // Switches menus when constructor is made to optimize memory
    this->parentMenu = Menu::currentMenu;

    Menu::currentMenu = this;
}

void Menu::back() {
    Menu* deleteMenu = Menu::currentMenu;

    currentMenu = deleteMenu->parentMenu;

    delete deleteMenu;
}
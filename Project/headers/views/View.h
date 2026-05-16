//
// Created by cmgm on 16/05/2026.
//

#ifndef FSOFT2026_1DC__VIEW_H
#define FSOFT2026_1DC__VIEW_H

class View {
public:
    static void showMainMenu();
    static int askMainOption();

    static void showClientMenu();
    static int askClientOption();

    static void showManagerMainMenu();
    static int askManagerMainOption();

};

#endif //FSOFT2026_1DC__VIEW_H

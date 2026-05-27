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

        static void showSupplierMainMenu();
        static int askSupplierMainOption();

        static void showSupplierLoggedMenu();
        static int askSupplierLoggedOption();

};
#endif //FSOFT2026_1DC__VIEW_H

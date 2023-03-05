#ifndef SERVER_PAGE_H
#define SERVER_PAGE_H

#include <array>
#include <string>


namespace Server {
    class Page
    {

    private:
        static const int NB_MANEUVERS = 22;
        void processPageInformation(std::string token, int index);

    public:
        Page();
        Page(std::string battlebookLine);
        ~Page();
        
        int number;
        std::array<int, NB_MANEUVERS> maneuvers;
        int damageDealt;
        int damageTaken;
        int warning;
        int lockOn;
        int view;
        int range;
        int facing;
    };
}

#endif
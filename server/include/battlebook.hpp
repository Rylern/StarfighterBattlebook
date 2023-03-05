#ifndef SERVER_BATTLEBOOK_H
#define SERVER_BATTLEBOOK_H

#include <string>
#include <vector>
#include "page.hpp"
#include "team.hpp"
#include "utils.hpp"


namespace Server {
    class Battlebook
    {

    public:
        static const int BEGINNING_PAGE = 170;
        static const int NUMBER_OF_PAGES = 223;

        Battlebook();
        Battlebook(Team team);
        ~Battlebook();
        Page* getPage(int number);
        int getNbManeuvers();
        std::vector<int> getPossibleHints(int maneuver);
        std::vector<int> getSurroundingsHints(int hint);
        int determineNewPage(int midPage, int maneuver);

    private:
        inline static const std::string REBELLION_BATTLEBOOK_FILENAME = Utils::joinPath({"res", "battlebookRebellion.txt"});
        inline static const std::string EMPIRE_BATTLEBOOK_FILENAME = Utils::joinPath({"res", "battlebookEmpire.txt"});
        static const int NB_MANEUVERS_REBELLION = 22;
        static const int NB_MANEUVERS_EMPIRE = 21;

        std::vector<Page*> pages;
        int nbManeuvers;
    };
}
#endif
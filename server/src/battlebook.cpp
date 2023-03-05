#include "battlebook.hpp"
#include <fstream>
#include "exception.hpp"

namespace Server {
    Battlebook::Battlebook() {}

    Battlebook::Battlebook(Team team) {
        std::string fileName;
        if (team == Team::REBELLION) {
            fileName = this->REBELLION_BATTLEBOOK_FILENAME;
        } else {
            fileName = this->EMPIRE_BATTLEBOOK_FILENAME;
        }
        std::fstream file(fileName, std::ios::in);

        if (file.is_open()) {
            std::string line;
            while (getline(file, line)) {
                this->pages.push_back(new Page(line));
            }
            file.close();
        } else {
            std::string errorMessage = "Error when reading " + fileName;
            throw Exception(errorMessage);
        }

        if(fileName == this->REBELLION_BATTLEBOOK_FILENAME) {
            this->nbManeuvers = this->NB_MANEUVERS_REBELLION;
        } else {
            this->nbManeuvers = this->NB_MANEUVERS_EMPIRE;
        }
    }

    Battlebook::~Battlebook() {
        size_t numberOfPages = this->pages.size();
        for (size_t i=0; i<numberOfPages; i++) {
            if (this->pages[i] != nullptr) {
                delete this->pages[i];
            }
        }
    }

    Page* Battlebook::getPage(int number) {
        return this->pages[number-this->pages[0]->number];
    }

    int Battlebook::getNbManeuvers() {
        return this->nbManeuvers;
    }

    std::vector<int> Battlebook::getPossibleHints(int maneuver) {
        std::vector<int> possibleHints;
        
        if (maneuver == 0) {
            possibleHints.push_back(maneuver);
            possibleHints.push_back(maneuver+1);
        } else if (maneuver == this->nbManeuvers) {
            possibleHints.push_back(maneuver-1);
            possibleHints.push_back(maneuver);
        } else {
            possibleHints.push_back(maneuver-1);
            possibleHints.push_back(maneuver);
            possibleHints.push_back(maneuver+1);
        }
        return possibleHints;
    }

    std::vector<int> Battlebook::getSurroundingsHints(int hint) {
        std::vector<int> surroundingsHints;
        if (hint == 0) {
            surroundingsHints.push_back(hint);
            surroundingsHints.push_back(hint + 1);
            surroundingsHints.push_back(hint + 2);
        } else if (hint == this->nbManeuvers) {
            surroundingsHints.push_back(hint - 2);
            surroundingsHints.push_back(hint - 1);
            surroundingsHints.push_back(hint);
        } else {
            surroundingsHints.push_back(hint - 1);
            surroundingsHints.push_back(hint);
            surroundingsHints.push_back(hint + 1);
        }
        return surroundingsHints;
    }

    int Battlebook::determineNewPage(int midPage, int maneuver) {
        return this->getPage(midPage)->maneuvers[maneuver];
    }
}
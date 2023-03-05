#include "page.hpp"

namespace Server {
    Page::Page() {}

    Page::Page(std::string battlebookLine) {
        std::string delimiter = "/";
        size_t pos = 0;

        int i = 0;
        while ((pos = battlebookLine.find(delimiter)) != std::string::npos) {
            processPageInformation(battlebookLine.substr(0, pos), i);
            battlebookLine.erase(0, pos + delimiter.length());
            i++;
        }
        processPageInformation(battlebookLine, i);
    }

    Page::~Page() {}

    void Page::processPageInformation(std::string token, int index) {
        if (index == 0) {
            this->number = std::stoi(token);
        } else if (index == 1) {
            std::string delimiter = ",";
            size_t pos = 0;
            int i = 0;
            while ((pos = token.find(delimiter)) != std::string::npos) {
                this->maneuvers[i] = std::stoi(token.substr(0, pos));
                token.erase(0, pos + delimiter.length());
                i++;
            }
            this->maneuvers[i] = std::stoi(token);
        } else if (index == 2) {
            this->damageDealt = std::stoi(token);
        } else if (index == 3) {
            this->damageTaken = std::stoi(token);
        } else if (index == 4) {
            this->warning = std::stoi(token);
        } else if (index == 5) {
            this->lockOn = std::stoi(token);
        } else if (index == 6) {
            this->view = std::stoi(token);
        } else if (index == 7) {
            this->range = std::stoi(token);
        } else if (index == 8) {
            this->facing = std::stoi(token);
        }
    }
}
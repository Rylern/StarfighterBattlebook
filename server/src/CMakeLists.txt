add_library(Server server.cpp battlebook.cpp page.cpp connection.cpp player.cpp game.cpp exception.cpp)

target_link_libraries(Server PRIVATE sfml-system sfml-network)


add_executable(StarfighterBattlebookServer main.cpp server.cpp battlebook.cpp page.cpp connection.cpp player.cpp game.cpp exception.cpp)

target_link_libraries(StarfighterBattlebookServer PRIVATE sfml-network sfml-system)

install(TARGETS StarfighterBattlebookServer DESTINATION StarfighterBattlebookServer)
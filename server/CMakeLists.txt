add_subdirectory(src)
add_subdirectory(include)

target_link_libraries(Server PUBLIC Utils)
target_link_libraries(StarfighterBattlebookServer PUBLIC Utils)
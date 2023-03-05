add_library(Ui widget.cpp label.cpp layout.cpp button.cpp window.cpp textfield.cpp image.cpp)

target_link_libraries(Ui PRIVATE sfml-graphics sfml-window sfml-system)

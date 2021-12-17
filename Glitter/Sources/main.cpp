#include "app.h"

int main(int argc, char **argv) {
    App::AppConfiguration config{};
    config.inital_zoom_factor = 1.0f;
    config.initial_cell_size = 20.0f;
    config.window_height = 800.0f;
    config.window_width = 800.0f;
    config.initial_fps = 10.0f;

    App app{config};
    app.loadPatternFromFile("../out/56P6H1V0.coord");
    app.run();
    return 0;
}
#include <iostream>
#include "application.h"

auto main(int argc, char** argv) -> int {
    wave_generator::Application application(argc, argv);

    auto return_code = application.Init();
    if (return_code != 0) return return_code;

    return_code = application.Run();
    return return_code;
}

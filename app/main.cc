#include "application.h"
#include "synthesizer_view.h"

namespace view = wave_generator::view;

auto main(int argc, char** argv) -> int {
    wave_generator::Application application(argc, argv);

    auto return_code = application.Init();
    if (return_code != 0) return return_code;

    std::unique_ptr<view::View> view = std::make_unique<view::SynthesizerView>();

    application.AddView(std::move(view));
    return_code = application.Run();
    return return_code;
}

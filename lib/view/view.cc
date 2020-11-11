#include "view.h"

namespace wave_generator::view {

auto View::WantClose() -> bool { return want_close_; }
void View::SetWantClose() { want_close_ = true; }

}  // namespace wave_generator::view
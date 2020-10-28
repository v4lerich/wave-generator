#include <SDL.h>
#include "glad2_loader.h"
#include "glad/gl.h"

namespace gl_loader {

auto LoadGl() -> bool {
    auto function_loader = reinterpret_cast<GLADloadfunc>(SDL_GL_GetProcAddress);
    return gladLoadGL(function_loader) == 0;
}

}

#ifndef WAVEGENERATOR_VIEW_H
#define WAVEGENERATOR_VIEW_H

namespace wave_generator::view {

class View {
  public:
    View() = default;
    virtual void Render() = 0;
    virtual ~View() = default;
};

}

#endif  // WAVEGENERATOR_VIEW_H

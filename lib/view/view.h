#ifndef WAVEGENERATOR_VIEW_H
#define WAVEGENERATOR_VIEW_H

namespace wave_generator::view {

class View {
  public:
    View() = default;
    virtual ~View() = default;

    virtual void Render() = 0;
    virtual auto WantClose() -> bool;

  protected:
    void SetWantClose();

  private:
    bool want_close_{false};
};

}  // namespace wave_generator::view

#endif  // WAVEGENERATOR_VIEW_H

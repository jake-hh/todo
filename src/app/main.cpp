#include <ftxui/component/component.hpp>
#include <ftxui/component/screen_interactive.hpp>
#include <ftxui/dom/elements.hpp>

using namespace ftxui;

int main() {
    auto screen = ScreenInteractive::Fullscreen();

    auto btn = Button("Quit", screen.ExitLoopClosure());

    auto renderer = Renderer(btn, [&] {
        return vbox({
            text("Hello, World!") | bold | hcenter,
            separator(),
            btn->Render() | hcenter,
        }) | center;
    });

    screen.Loop(renderer);
    return 0;
}

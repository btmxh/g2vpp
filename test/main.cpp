#include "g2v.hpp"
#include "g2v_cvffmpeg.hpp"

#include "glad.h"
#include <iostream>

int main() {
        std::cout << "cosi" << std::endl;
    try {
        g2v::Init();
        {
            g2v::RenderContext ctx(1280, 720);
            g2v::CVFFmpegEncoder enc(ctx, "output123.mkv", 25.0, 1000000);

            constexpr int FRAMES = 100;
            enc.SetVideoFrameCallback([&](){
                auto frame = ctx.GetFrameNumber();
                glClearColor(1.0f / FRAMES * frame, 0.0f, 1.0f, 1.0f);
                glClear(GL_COLOR_BUFFER_BIT);
                return ctx.GetFrameNumber() > FRAMES;
            });

            enc.Encode();
        }

        g2v::Terminate();
    } catch(std::exception& ex) {
        std::cout << ex.what() << std::endl;
    }
}
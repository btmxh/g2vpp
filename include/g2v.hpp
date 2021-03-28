#pragma once
#include <stdint.h>
#include <functional>

namespace g2v {
    void Init();
    void Terminate();

    using GLuint = uint32_t;
    using GLenum = int;

    class RenderContext {
    public:
        RenderContext(int width, int height);
        ~RenderContext();

        void Bind() const noexcept;
        void GetPixels(void* pixels, GLenum format) const noexcept;
        void NextFrame() noexcept;

        int GetWidth() const noexcept;
        int GetHeight() const noexcept;
        int GetFrameNumber() const noexcept;
    private:
        static constexpr int FRAMEBUFFERS = 2;

        int m_width, m_height, m_frame;
        GLuint m_framebuffers[FRAMEBUFFERS];
        GLuint m_attachments[FRAMEBUFFERS];
        GLuint m_pixelBuffers[FRAMEBUFFERS];
    };

    class Encoder {
    public:
        Encoder(RenderContext& ctx);
        virtual ~Encoder();

        virtual void Encode() = 0;

        void SetVideoFrameCallback(std::function<bool()> videocb);
    protected:
        RenderContext& m_ctx;
        std::function<bool()> m_videocb;
    };
}
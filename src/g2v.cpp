#include "g2v.hpp"

#include "glad.h"
#include "GLFW/glfw3.h"

#include <stdexcept>
#include <stdarg.h>

void err(const char* const format, ...) {
    char msg[256];
    va_list v;
    va_start(v, format);
    vsnprintf(msg, 256, format, v);
    va_end(v);
    throw std::runtime_error(msg);
}

static GLFWwindow* g2v_wnd;

void g2v::Init() {
    auto get_glfw_err = [](){ const char* err; glfwGetError(&err); return err; };
    if(!glfwInit()) err("Could not initialize GLFW:\n%s", get_glfw_err());
    
    glfwWindowHint(GLFW_VISIBLE, GLFW_FALSE);
    g2v_wnd = glfwCreateWindow(100, 100, "g2vpp", NULL, NULL);

    if(!g2v_wnd) {
        glfwTerminate();
        err("Could not create OpenGL window:\n%s", get_glfw_err());
    }   

    glfwMakeContextCurrent(g2v_wnd);
    if(!gladLoadGL()) {
        glfwDestroyWindow(g2v_wnd);
        glfwTerminate();
        err("Could not load OpenGL functions");
    }
}

void g2v::Terminate() {
    glfwDestroyWindow(g2v_wnd);
    glfwTerminate();
}

g2v::RenderContext::RenderContext(int width, int height): m_width(width), m_height(height), m_frame(0) {
    glGenFramebuffers(FRAMEBUFFERS, m_framebuffers);
    glGenBuffers(FRAMEBUFFERS, m_pixelBuffers);
    glGenTextures(FRAMEBUFFERS, m_attachments);

    for(int i = 0; i < FRAMEBUFFERS; i++) {
        glBindFramebuffer(GL_FRAMEBUFFER, m_framebuffers[i]);
        glBindTexture(GL_TEXTURE_2D, m_attachments[i]);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_attachments[i], 0);
        glBindBuffer(GL_PIXEL_PACK_BUFFER, m_pixelBuffers[i]);
        glBufferData(GL_PIXEL_PACK_BUFFER, width * height * 4, NULL, GL_STREAM_READ);
    }
}

g2v::RenderContext::~RenderContext() {
    glDeleteFramebuffers(FRAMEBUFFERS, m_framebuffers);
    glDeleteBuffers(FRAMEBUFFERS, m_pixelBuffers);
    glDeleteTextures(FRAMEBUFFERS, m_attachments);
}

void g2v::RenderContext::Bind() const noexcept {
    glBindFramebuffer(GL_FRAMEBUFFER, m_framebuffers[m_frame % FRAMEBUFFERS]);
    glViewport(0, 0, m_width, m_height);
}

void g2v::RenderContext::GetPixels(void* pixels, g2v::GLenum format) const noexcept {
    int read_pbo_idx = m_frame % FRAMEBUFFERS;
    int proc_pbo_idx = (read_pbo_idx + 1) % FRAMEBUFFERS;

    glBindBuffer(GL_PIXEL_PACK_BUFFER, m_pixelBuffers[read_pbo_idx]);
    // glBindBuffer(GL_PIXEL_PACK_BUFFER, 0);
    // int* buffer_content = malloc(m_width * m_height * sizeof(int));
    glReadPixels(0, 0, m_width, m_height, format, GL_UNSIGNED_BYTE, 0);

    glBindBuffer(GL_PIXEL_PACK_BUFFER, m_pixelBuffers[proc_pbo_idx]);
    auto buffer_content = reinterpret_cast<uint8_t*>(glMapBuffer(GL_PIXEL_PACK_BUFFER, GL_READ_ONLY));

    //Since glReadPixels returns pixel values upside down, we have to preprocess them
    // memcpy(pixels, buffer_content, m_width * m_height * 3);
    auto pix_data = reinterpret_cast<uint8_t*>(pixels) + m_width * (m_height - 1) * 3;
    do {
        memcpy(pix_data, buffer_content, m_width * 3);
        pix_data -= m_width * 3;
        buffer_content += m_width * 3;
    } while(pix_data != pixels);

    glUnmapBuffer(GL_PIXEL_PACK_BUFFER);
}

void g2v::RenderContext::NextFrame() noexcept {
    m_frame++;
}

int g2v::RenderContext::GetWidth() const noexcept {
    return m_width;
}

int g2v::RenderContext::GetHeight() const noexcept {
    return m_height;
}

int g2v::RenderContext::GetFrameNumber() const noexcept {
    return m_frame;
}

g2v::Encoder::Encoder(g2v::RenderContext& ctx): m_ctx(ctx) {}

g2v::Encoder::~Encoder() {}

void g2v::Encoder::SetVideoFrameCallback(std::function<bool()> videocb) {
    m_videocb = videocb;
}

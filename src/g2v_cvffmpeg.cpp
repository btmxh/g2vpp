#include "g2v_cvffmpeg.hpp"

#include "opencv2/videoio.hpp"
#include "glad.h"

#include <iostream>

g2v::CVFFmpegEncoder::CVFFmpegEncoder(g2v::RenderContext& ctx, const std::string& output, double fps, int64_t bitrate) : g2v::Encoder(ctx), m_writer(std::make_unique<cv::VideoWriter>()) {
    int codec = cv::VideoWriter::fourcc('X', '2', '6', '4');
    m_writer->open(output, codec, fps, {ctx.GetWidth(), ctx.GetHeight()}, true);

    std::cout << cv::getBuildInformation() << std::endl;
}

void g2v::CVFFmpegEncoder::Encode() {
    cv::Mat pixels(m_ctx.GetHeight(), m_ctx.GetWidth(), CV_8UC3);
    m_ctx.Bind();
    while(true) {
        auto ret = m_videocb();
        m_ctx.GetPixels(pixels.data, GL_BGR);
        m_writer->write(pixels);
        m_ctx.NextFrame();

        if(ret) return;
    }
    m_writer->release();
}

g2v::CVFFmpegEncoder::~CVFFmpegEncoder() {}

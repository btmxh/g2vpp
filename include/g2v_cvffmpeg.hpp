#pragma once

#include "g2v.hpp"

#include <memory>
#include <string>

namespace cv {
    class VideoWriter;
}

namespace g2v {
    class CVFFmpegEncoder : public Encoder {
    public:
        CVFFmpegEncoder(RenderContext& ctx, const std::string& output, double fps, int64_t bitrate);
        ~CVFFmpegEncoder();

        void Encode();
    private:
        std::unique_ptr<cv::VideoWriter> m_writer;
    };
}
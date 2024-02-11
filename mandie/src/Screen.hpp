#ifndef MANDIE_SCREEN_HPP
#define MANDIE_SCREEN_HPP

#include <cstdint>
#include <memory>

using namespace std;

constexpr int16_t SCREEN_WIDTH = 640;
constexpr int16_t SCREEN_HEIGHT = 480;

using FrameBuffer = std::array<uint16_t, SCREEN_WIDTH * SCREEN_HEIGHT>;

class Screen {
public:
    void clear() {
        frame_buffer->fill(0xFFFF);
    }

    FrameBuffer::pointer get_frame_buffer() {
        return frame_buffer->data();
    }

private:
    // On an ST, there would be issues around the alignment of the framebuffer. Thankfully, the Falcon has no such
    // issues
    std::unique_ptr<FrameBuffer> frame_buffer = make_unique<FrameBuffer>();
};

#endif //MANDIE_SCREEN_HPP

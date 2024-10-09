#include <cstddef>
#include <functional>
#include <optional>
#include <vector>
#include <algorithm>
#include <cstdint>
#include <iostream>
#include <span>

class Buffer {
public:
    explicit Buffer(std::size_t capacity):
        buf_(capacity)
    {
    }

    [[nodiscard]] auto buffer() const -> std::span<const std::uint8_t>
    {
        // using span to return a view of the buffer
        // self.buf.as_slice()[self.pos..self.filled]
        return {buf_.data() + pos_, filled_ - pos_};
    }

    [[nodiscard]] inline std::size_t capacity() const { return buf_.size(); }

    [[nodiscard]] inline std::size_t available() const { return filled_ - pos_; }

    inline void discard()
    {
        pos_ = 0;
        filled_ = 0;
    }

    inline void consume(std::size_t amt)
    {
        pos_ += amt;
        if (pos_ >= filled_) {
            discard();
        }
    }

    //
    std::optional<std::size_t> fill_buf(std::istream &reader)
    {
        if (pos_ >= filled_) {
            pos_ = 0;
            filled_ = 0;
        }

        reader.read(reinterpret_cast<char *>(buf_.data() + filled_), buf_.size() - filled_);
        filled_ += reader.gcount();
        return reader.gcount();
    }

private:
    std::vector<std::uint8_t> buf_;
    std::size_t pos_ = 0;
    std::size_t filled_ = 0;
};

int main()
{
    Buffer reader(1024);

    return 0;
}
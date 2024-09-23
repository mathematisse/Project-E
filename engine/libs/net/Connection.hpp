
#include <memory>
namespace net {

template<typename T>
class Connection : public std::enable_shared_from_this<Connection<T>> {

public:
    enum class Ownership {
        Client,
        Server,
    };

public:
    Connection(Ownership parent) { }
};

} // namespace net
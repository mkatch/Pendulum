#ifndef JG_SPAWNER_HPP
#define JG_SPAWNER_HPP

namespace jg {

template <typename T>
class Spawner
{
public:
    virtual T spawn() = 0; 

    virtual ~Spawner() {/* Do nothing. */}
};

} // namespace jg

#endif // JG_SPAWNER_HPP

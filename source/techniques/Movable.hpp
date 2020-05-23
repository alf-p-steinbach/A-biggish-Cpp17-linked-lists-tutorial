#pragma once

namespace techniques {
    class Movable
    {
        Movable( const Movable& )                       = delete;
        auto operator=( const Movable& ) -> Movable&    = delete;

    public:
        auto operator=( Movable&& ) noexcept -> Movable&    = default;
        Movable() noexcept                                  = default;
        Movable( Movable&& ) noexcept                       = default;
    };
}  // namespace techniques

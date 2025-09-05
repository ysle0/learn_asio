#include <fmt/core.h>
#include "asio.hpp"


namespace {
    constexpr int EXIT_OK = 0;
    constexpr int EXIT_ERR = 1;

    void do_timer_sync();
}

auto main() -> int {
    fmt::println("learn Boost::ASIO!");

    fmt::println("use a timer synchronously.");

    ::do_timer_sync();

    return EXIT_OK;
}

namespace {
    void do_timer_sync() {
        fmt::println("do_timer_sync start.");

        // asio requires at least one I/O exec context.
        // such as io_context or thread_pool context.
        // An I/O exec context provides access to I/O functionality.
        asio::io_context io_ctx;
        asio::steady_timer timer(
            io_ctx,
            asio::chrono::seconds(4)
        );

        timer.wait(); // <- block-wait for 4 seconds.
        // timer always has either one of two states:
        // 1. expired
        // 2. not expired
        // if the steady_timer::wait() is called on an expired timer,
        // then it will return immediately.
        fmt::println("do_timer_sync end.");

        fmt::println("wait again!");
        timer.wait(); // <- block-wait for 4 seconds.
        fmt::println("wait again end.");
    }
}

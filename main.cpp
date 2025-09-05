#include <fmt/core.h>
#include "asio.hpp"


namespace {
    constexpr int EXIT_OK = 0;
    constexpr int EXIT_ERR = 1;

    void do_timer_sync();

    void do_timer_async();
}

auto main() -> int {
    fmt::println("learn Boost::ASIO!");

    fmt::println("use a timer synchronously.");

    // ::do_timer_sync();
    ::do_timer_async();

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

    void do_timer_async() {
        // asynchronous functionality means supplying a completion token, which
        // determines how the result will be delivered to a completion handler
        // when an async operation completes.

        asio::io_context io_ctx;
        asio::steady_timer timer(io_ctx, asio::chrono::seconds(4));

        fmt::println("async_wait start.");

        // asio lib ensures that completion handlers will only be called from
        // threads that are currently calling asio::io_context::run().
        // therefor unless the asio::io_context::run() is called, the completion handler
        // for the asynchronous wait completion suspends.
        std::function handler = [](const asio::error_code &err) {
            fmt::println("async_wait: ec: {}", err.message());
        };
        timer.async_wait(handler);

        // give always some work to do before calling asio::io_context::run().
        const size_t exec_handler_cnt = io_ctx.run();
        fmt::println("exec_handler_cnt: {}", exec_handler_cnt);
    }
}

#include <spdlog/spdlog.h>
#include "asio.hpp"

namespace {
constexpr int EXIT_OK = 0;
constexpr int EXIT_ERR = 1;

void do_timer_sync();

void do_timer_async();

void print(
    const std::error_code &ec,
    const std::shared_ptr<asio::steady_timer> &timer,
    const std::shared_ptr<int> &count
);

void bind_args_to_completion_handler();

void bind_member_function_to_completion_handler();

class printer {
public:
    void print() {
        if (_count < 5) {
            spdlog::info("count: {}", _count);
            ++_count;

            _timer.expires_at(
                _timer.expiry() + asio::chrono::seconds(1));
            _timer.async_wait(std::bind(&printer::print, this));
        }
    }

public:
    printer(asio::io_context &io_ctx, const std::chrono::duration<long long> delayInSeconds)
        : _timer(io_ctx, delayInSeconds),
          _count(0) {
        _timer.async_wait(std::bind(&printer::print, this));
    }

    ~printer() {
        spdlog::info("printer destructed. final count was {}",
                     this->_count);
    }

protected:
    asio::steady_timer _timer;
    int _count;
};
}

auto main() -> int {
    spdlog::info("learn boost::asio!");
    spdlog::info("use a timer asynchronously.");

    // ::do_timer_sync();
    // ::do_timer_async();
    // ::bind_args_to_completion_handler();
    bind_member_function_to_completion_handler();

    return EXIT_OK;
}

namespace {
void do_timer_sync() {
    spdlog::info("do_timer_sync starts");

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
    spdlog::info("do_timer_sync ends");

    spdlog::info("wait again");
    timer.wait(); // <- block-wait for 4 seconds.
    spdlog::info("wait again end.");
}

void do_timer_async() {
    // asynchronous functionality means supplying a completion token, which
    // determines how the result will be delivered to a completion handler
    // when an async operation completes.

    asio::io_context io_ctx;
    asio::steady_timer timer(io_ctx, asio::chrono::seconds(4));

    spdlog::info("async_wait start.");

    // asio lib ensures that completion handlers will only be called from
    // threads that are currently calling asio::io_context::run().
    // therefor unless the asio::io_context::run() is called, the completion handler
    // for the asynchronous wait completion suspends.
    std::function handler = [](const asio::error_code &err) {
        spdlog::info("async_wait: ec: {}", err.message());
    };
    timer.async_wait(handler);

    // give always some work to do before calling asio::io_context::run().
    const size_t exec_handler_cnt = io_ctx.run();
    spdlog::info("exec_handler_cnt: {}", exec_handler_cnt);
}

void print(
    const std::error_code &ec,
    const std::shared_ptr<asio::steady_timer> &timer,
    const std::shared_ptr<int> &count
) {
    const int count_val = *count;
    if (count_val < 5) {
        spdlog::info("count: {}", count_val);
        ++*count;

        timer->expires_at(timer->expiry() + asio::chrono::seconds(1));
        timer->async_wait([timer, count](const asio::error_code &ec) {
            ::print(ec, timer, count);
        });
    }
}

void bind_args_to_completion_handler() {
    asio::io_context io_ctx;

    auto count = std::make_shared<int>(0);
    const auto timer = std::make_shared<asio::steady_timer>(
        io_ctx,
        asio::chrono::seconds(4)
    );
    timer->async_wait([timer, count](const std::error_code &ec) {
        ::print(ec, timer, count);
    });

    io_ctx.run();

    spdlog::info("final count is {}", *count);
}

void bind_member_function_to_completion_handler() {
    asio::io_context io_ctx;
    printer printer{io_ctx, asio::chrono::seconds(4)};
    io_ctx.run();
}
}

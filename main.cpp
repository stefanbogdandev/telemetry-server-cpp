#include "TelemetryServer.hpp"
#include <boost/asio.hpp>
#include <iostream>

int main() {
    try {
        boost::asio::io_context io_context;

        TelemetryServer server(io_context, 8080);
        server.run();

        std::cout << "Server running on http://localhost:8080\n";
        //io_context.run();

        const unsigned int thread_count = 4; //std::thread::hardware_concurrency();
        std::cout << "Starting " << thread_count << " threads for io_context.run()\n";

        // Create and run threads
        std::vector<std::thread> threads;
        for (unsigned int i = 0; i < thread_count; ++i) {
            threads.emplace_back([&io_context]() {
                io_context.run();
            });
        }

        // Main thread can also participate (optional but recommended)
        io_context.run();

        // Join all worker threads
        for (auto& t : threads) {
            t.join();
        }
    } catch (const std::exception& e) {
        std::cerr << "Exception: " << e.what() << "\n";
    }

    return 0;
}

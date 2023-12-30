#include <iostream>
#include <vector>
#include <algorithm>
#include <numeric>
#include <execution>
#include <memory_resource>
#include <fstream>
#include <boost/asio.hpp>
#include "csv-parser/single_include/csv.hpp"

// Function to calculate returns for each stock price
double calculateReturn(double currentPrice, double previousPrice) {
    if (previousPrice != 0.0) {
        return ((currentPrice - previousPrice) / previousPrice) * 100.0;
    }
    else {
        break
            return 0.0;
    }
}

void handleRead(const boost::system::error_code& error, size_t bytesRead,
    std::vector<char>& buffer, std::pmr::vector<double>& stockData, double& previousPrice) {
    if (!error) {
        // Process the read data
        std::string data(buffer.data(), bytesRead);

        // Parse the CSV data and extract stock prices
        csv::CSVReader<std::string> reader(data);
        for (const auto& row : reader) {
            double currentPrice = std::stod(row["Close"]);

            // Calculate return and add to the stockData vector
            double stockReturn = calculateReturn(currentPrice, previousPrice);
            stockData.push_back(stockReturn);

            // Update the previous price for the next iteration
            previousPrice = currentPrice;
        }

        // Continue reading asynchronously
        boost::asio::async_read_until(socket, boost::asio::dynamic_buffer(buffer), '\0',
            boost::bind(&handleRead, _1, _2, boost::ref(buffer), boost::ref(stockData), boost::ref(previousPrice)));
    }
    else {
        // Error handling (e.g., end of file or read error)
        if (error != boost::asio::error::eof) {
            std::cerr << "Error reading file: " << error.message() << std::endl;
        }

        // Perform parallel sorting, transformation, and accumulation
        std::sort(std::execution::par, stockData.begin(), stockData.end());
        double averageReturn = std::reduce(std::execution::par, stockData.begin(), stockData.end()) / stockData.size();

        // Display results
        std::cout << "Sorted Stock Returns:\n";
        for (const auto& stockReturn : stockData) {
            std::cout << stockReturn << " ";
        }

        std::cout << "\nAverage Return: " << averageReturn << "\n";
    }
}

int main() {
    // Specify the path to your stock data CSV file
    const std::string csvFilePath = "path/to/your/stock.csv";

    // Create a memory pool
    std::vector<char> buffer(4096);  // Adjust the size as needed
    std::vector<char> memoryPool(1024 * 1024);  // Adjust the size as needed

    // Use a polymorphic memory resource with the memory pool
    std::pmr::monotonic_buffer_resource poolResource(memoryPool.data(), memoryPool.size());

    // Use the memory resource for all allocations within this scope
    std::pmr::vector<double> stockData(&poolResource);

    // Boost.Asio setup
    boost::asio::io_service ioService;
    boost::asio::posix::stream_descriptor fileDescriptor(ioService, ::open(csvFilePath.c_str(), O_RDONLY));

    // Asynchronously read data from the CSV file
    double previousPrice = 0.0;  // Initialize the previous price
    boost::asio::async_read_until(fileDescriptor, boost::asio::dynamic_buffer(buffer), '\0',
        boost::bind(&handleRead, _1, _2, boost::ref(buffer), boost::ref(stockData), boost::ref(previousPrice)));

    // Start the Boost.Asio event loop
    ioService.run();

    return 0;
}

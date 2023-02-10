#include <iostream>
#include "boost/mpi.hpp"

namespace mpi = boost::mpi;

int main(int argc, char *argv[]) {
    const int MAX_NUMBERS = 100;
    int numbers[MAX_NUMBERS];
    int number_amount;
    mpi::environment environment(argc, argv);
    mpi::communicator world;
    std::cout << "Process with rank " << world.rank() << " was initialized." << std::endl;
    if (world.rank() == 0) {
        // Pick a random amount of integers to send to process one
        number_amount = (rand() / (float)RAND_MAX) * MAX_NUMBERS;
        for (int i = 0; i < number_amount; ++i) {
            numbers[i] = number_amount - i;
            std::cout << number_amount - i << " ";
        }
        std::cout << std::endl;

        // Send the amount of integers to process one
        world.send(1, 0, numbers, number_amount);
        std::cout << "0 sent " << number_amount << " numbers to 1" << std::endl;
    } else if (world.rank() == 1) {
        // Receive at most MAX_NUMBERS from process zero
        mpi::status status = world.recv(0, 0, numbers, 200);
        // After receiving the message, check the status to determine
        // how many numbers were actually received
        // Print off the amount of numbers, and also print additional
        // information in the status object
        if (status.count<int>().has_value()) {
            std::cout << "1 received " << status.count<int>().get() << " numbers from source =" << status.source() <<
                      ", tag =" << status.tag() << "." << std::endl;
            for (int i = 0; i < status.count<int>().get(); ++i) {
                std::cout << numbers[i] << " ";
            }
            std::cout << std::endl;
        } else {
            std::cout << "err" << std::endl;
        }
    }
}

#include <iostream>
#include "boost/mpi.hpp"
#include "random"

namespace mpi = boost::mpi;

int main(int argc, char *argv[]) {
    const int MAX_NUMBERS = 100;
    int numbers[MAX_NUMBERS];
    int number_amount;
    mpi::environment environment(argc, argv);
    mpi::communicator world;
    std::cout << "Process with rank " << world.rank() << " was initialized." << std::endl;
    if (world.rank() == 0) {
        std::random_device dev;
        std::mt19937 rng(dev());
        std::uniform_int_distribution<std::mt19937::result_type> distMN(1,MAX_NUMBERS); // distribution in range [1, 6]
        number_amount = static_cast<int>(distMN(rng));
        for (int i = 0; i < number_amount; ++i) {
            numbers[i] = number_amount - i;
            std::cout << number_amount - i << " ";
        }
        std::cout << std::endl;

        world.send(1, 0, numbers, number_amount);
        std::cout << "0 sent " << number_amount << " numbers to 1" << std::endl;
    } else if (world.rank() == 1) {

        mpi::status status = world.recv(0, 0, numbers, 200);

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

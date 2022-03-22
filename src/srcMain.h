#ifndef SRC_MAIN
#define SRC_MAIN

namespace batmobile {
    class MotorSet;
    class Robot;
    class ControllerScreen;
}

namespace ports {
    namespace individual {
        // Change these depending on ports
        constexpr int left_ports_top = 1;
        constexpr int left_ports_bottom = 2;
        constexpr int left_ports_middle = 5;

        constexpr int right_ports_top = 3;
        constexpr int right_ports_bottom = 4;
        constexpr int right_ports_middle = 6;
    }

    namespace ind_ports = ports::individual;

    std::vector<int> left_ports = std::vector<int> {ind_ports::left_ports_top, ind_ports::left_ports_middle, ind_ports::left_ports_bottom};
    std::vector<int> right_ports = std::vector<int> {ind_ports::right_ports_top, ind_ports::right_ports_middle, ind_ports::right_ports_bottom};
}

namespace ind_ports = ports::ind_ports;

#endif
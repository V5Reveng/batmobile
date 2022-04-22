#ifndef SRC_MAIN
#define SRC_MAIN

namespace batmobile {
    class MotorSet;
    class Robot;
    class ControllerScreen;
}

namespace config {
  constexpr int max_speed_v = -85;
}

namespace ports {
    namespace individual {
        // Change these depending on ports
        constexpr int left_ports_top = 20;
        constexpr int left_ports_bottom = 17;

        constexpr int right_ports_top = 19;
        constexpr int right_ports_bottom = 18;

        constexpr int messi = 16;
    }

    namespace ind_ports = ports::individual;

    std::vector<int> left_ports = std::vector<int> {ind_ports::left_ports_top, ind_ports::left_ports_bottom};
    std::vector<int> right_ports = std::vector<int> {ind_ports::right_ports_top, ind_ports::right_ports_bottom};
}

namespace ind_ports = ports::ind_ports;

#endif
#include <xylose/Timer.h>

#include <iostream>
#include <cmath>

int main() {
  double r = 0.0;
  xylose::Timer t;
  t.wall_time_label = "s";
  t.cpu_time_label = "s (cpu)";

  t.start();
  for (double i = 1e-8; i< 1.0; i+= 3e-8) {
    r += 1e-5 * std::log(i);
  }
  t.stop();

  std::cout << "Printing timing using ostream insertion operator<<:\n";
  std::cout << "        clock\t\tcpu\n"
               "Timer:  " << t << std::endl;

  std::cout << "Printing timing by accessing the data directly:\n";
  std::cout << "        clock\t\tcpu\n"
               "Timer:  " << t.dt << "s\t" << t.dt_cpu_time << "cpu-sec" << std::endl;

  std::cout << "Dummy variable 'r' was left at value '"<< r << "'\n"
            << std::flush;
  return 0;
}

/*
 * (C) Copyright 2022-2023 Larry Milne (https://www.larrycloud.ca)
 *
 * This code is distributed on "AS IS" BASIS,
 * WITHOUT WARRANTINES OR CONDITIONS OF ANY KIND.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 * @author: qawse3dr a.k.a Larry Milne
 */

#include "rpizero-single.hpp"
#include <iostream>
#include <pigpio.h> 

using cppedal::pwm_output::PwmOutput;
using cppedal::pwm_output::RpiZeroSinglePwmOutput;

RpiZeroSinglePwmOutput::RpiZeroSinglePwmOutput() {
  if (gpioInitialise() < 0){
      printf("gpio_init failed. Are you running as root??\n");
      exit(1);
    }

  
  gpioSetMode(18, PI_ALT5); 
}
RpiZeroSinglePwmOutput::~RpiZeroSinglePwmOutput() { gpioTerminate(); }

 void RpiZeroSinglePwmOutput::output(uint32_t out) { gpioHardwarePWM(18, 9600000/64,  out / 2048.0 * 1000000); }

extern "C" {
std::unique_ptr<PwmOutput> makePwmOutput() {
  return std::unique_ptr<PwmOutput>(new RpiZeroSinglePwmOutput);
}
}

/*
 * (C) Copyright 2022 Larry Milne (https://www.larrycloud.ca)
 *
 * This code is distributed on "AS IS" BASIS,
 * WITHOUT WARRANTINES OR CONDITIONS OF ANY KIND.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 * @author: qawse3dr a.k.a Larry Milne
 */

#include "rpi-single.hpp"

#include <wiringPi.h>

#include <iostream>

using cppedal::pwm_output::PwmOutput;
using cppedal::pwm_output::RpiSinglePwmOutput;

RpiSinglePwmOutput::RpiSinglePwmOutput() {
  if (wiringPiSetup() == -1) {
    std::cerr << "failed to setup wiringPI" << std::endl;
    exit(1);
  }
  pinMode(1, PWM_OUTPUT);
  pwmSetRange(2048);
  pwmSetClock(1);
  pwmSetMode(PWM_MODE_MS);
}
RpiSinglePwmOutput::~RpiSinglePwmOutput() {}

void RpiSinglePwmOutput::output(uint32_t out) { pwmWrite(1, out); }

extern "C" {
std::unique_ptr<PwmOutput> makePwmOutput() {
  return std::unique_ptr<PwmOutput>(new RpiSinglePwmOutput);
}
}

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

using cppedal::pwm_output::PwmOutput;
using cppedal::pwm_output::RpiSinglePwmOutput;

RpiSinglePwmOutput::RpiSinglePwmOutput() {
  pinMode(1, PWM_OUTPUT);
  pwmSetRange(4096);
  pwmSetClock(1);
  pwmSetMode(1);
}
RpiSinglePwmOutput::~RpiSinglePwmOutput() {}

void RpiSinglePwmOutput::output(int64_t out) { pwmWrite(1, out); }

std::unique_ptr<PwmOutput> cppedal::pwm_output::makePwmOutput() {
  return std::unique_ptr<PwmOutput>(new RpiSinglePwmOutput);
}

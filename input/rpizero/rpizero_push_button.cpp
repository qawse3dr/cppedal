/*
 * (C) Copyright 2023 Larry Milne (https://www.larrycloud.ca)
 *
 * This code is distributed on "AS IS" BASIS,
 * WITHOUT WARRANTINES OR CONDITIONS OF ANY KIND.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 * @author: qawse3dr a.k.a Larry Milne
 */

#include "rpizero_push_button.hpp"


#include <unistd.h>
#include <pigpio.h>

using cppedal::input::Input;
using cppedal::input::RpiZeroPushButton;

RpiZeroPushButton::RpiZeroPushButton(uint8_t pin, bool pull_up)
    : PushButton(pin, pull_up) {
  if (gpioInitialise() < 0);
  
  gpioSetMode(pin_, PI_INPUT);
  if (pull_up_) {
    gpioSetPullUpDown(pin_, PI_PUD_UP);
  } else {
    gpioSetPullUpDown(pin_, PI_PUD_DOWN);
  }
  // Setup button
  gpioGlitchFilter(pin_, 2000);
  gpioSetAlertFuncEx(pin_, _RpiZeroCallback, this);
}

RpiZeroPushButton::~RpiZeroPushButton() {
  gpioSetAlertFuncEx(pin_, 0, this);
  gpioTerminate();
}

void RpiZeroPushButton::_RpiZeroCallback(int gpio, int level, uint32_t tick, void* user) {
  static_cast<RpiZeroPushButton*>(user)->RpiZeroCallback(gpio, level, tick);
}

void RpiZeroPushButton::RpiZeroCallback(int gpio, int level, uint32_t tick) {
  if (pressed_ != level) {
    pressed_ = level;
    if (callback_) callback_(pressed_);
  }
  usleep(5000);
}

extern "C" {
std::unique_ptr<Input> makePushButton(uint8_t pin, bool pull_up) {
  return std::unique_ptr<Input>(new RpiZeroPushButton(pin, pull_up));
}
}

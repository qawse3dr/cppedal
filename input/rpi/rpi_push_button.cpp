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

#include "rpi_push_button.hpp"

#include <pthread.h>
#include <unistd.h>
#include <wiringPi.h>

using cppedal::input::Input;
using cppedal::input::RpiPushButton;

RpiPushButton::RpiPushButton(uint8_t pin, bool pull_up)
    : PushButton(pin, pull_up) {
  wiringPiSetup();
  pinMode(pin_, INPUT);
  if (pull_up_) {
    pullUpDnControl(pin_, PUD_UP);
  }
  // Setup button
  thread_ = std::thread(&RpiPushButton::workLoop, this);
}

RpiPushButton::~RpiPushButton() {
  if (thread_.joinable()) {
    pthread_cancel(thread_.native_handle());
    thread_.join();
  }
}

void RpiPushButton::workLoop() {
  int oldCState;
  bool state;
  while (true) {
    state = digitalRead(pin_);

    if (state != pressed_) {
      pressed_ = state;
      if (callback_) callback_(state);
    }

    // Wait for interupt only allow for cancelling when waiting
    // to avoid bads
    pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, &oldCState);
    usleep(50);
    pthread_setcancelstate(PTHREAD_CANCEL_DISABLE, &oldCState);
  }
}

extern "C" {
std::unique_ptr<Input> makePushButton(uint8_t pin, bool pull_up) {
  return std::unique_ptr<Input>(new RpiPushButton(pin, pull_up));
}
}
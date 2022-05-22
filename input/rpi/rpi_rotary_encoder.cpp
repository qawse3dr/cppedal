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

#include "rpi_rotary_encoder.hpp"

#include <unistd.h>
#include <wiringPi.h>

using cppedal::input::Input;
using cppedal::input::RpiRotaryEncoder;

RpiRotaryEncoder::RpiRotaryEncoder(uint8_t clk_pin, uint8_t data_pin,
                                   bool pull_up)
    : RotaryEncoder(clk_pin, data_pin, pull_up) {
  wiringPiSetup();
  pinMode(clk_pin, INPUT);
  pinMode(data_pin, INPUT);

  if (pull_up_) {
    pullUpDnControl(clk_pin, PUD_UP);
    pullUpDnControl(data_pin, PUD_UP);

  } else {
    pullUpDnControl(clk_pin, PUD_DOWN);
    pullUpDnControl(data_pin, PUD_DOWN);
  }
  // Setup button
  thread_ = std::thread(&RpiRotaryEncoder::workLoop, this);
}

RpiRotaryEncoder::~RpiRotaryEncoder() {
  if (thread_.joinable()) {
    pthread_cancel(thread_.native_handle());
    thread_.join();
  }
}

void RpiRotaryEncoder::workLoop() {
  int oldCState;
  bool state;
  bool old_state;
  while (true) {
    state = digitalRead(clk_pin_);

    if (state != old_state) {
      old_state = state;
      bool other = digitalRead(data_pin_);

      if (state == other) {
        value_--;
      } else {
        value_++;
      }
      if (callback_) callback_(value_);
    }

    // Wait for interupt only allow for cancelling when waiting
    // to avoid bads
    pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, &oldCState);
    usleep(50);
    pthread_setcancelstate(PTHREAD_CANCEL_DISABLE, &oldCState);
  }
}

extern "C" {
std::unique_ptr<Input> makeRotaryEncoder(uint8_t clk, uint8_t data,
                                         bool pull_up) {
  return std::unique_ptr<Input>(new RpiRotaryEncoder(clk, data, pull_up));
}
}
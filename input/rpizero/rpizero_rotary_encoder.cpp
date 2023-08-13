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

#include "rpizero_rotary_encoder.hpp"

#include <pigpio.h>

using cppedal::input::Input;
using cppedal::input::RpiZeroRotaryEncoder;

RpiZeroRotaryEncoder::RpiZeroRotaryEncoder(uint8_t clk_pin, uint8_t data_pin,
                                   bool pull_up)
    : RotaryEncoder(clk_pin, data_pin, pull_up) {
   
   if (gpioInitialise() < 0);
   gpioSetMode(clk_pin, PI_INPUT);
   gpioSetMode(data_pin, PI_INPUT);

   /* pull up is needed as encoder common is grounded */

   gpioSetPullUpDown(clk_pin, PI_PUD_UP);
   gpioSetPullUpDown(data_pin, PI_PUD_UP);

   /* monitor encoder level changes */
   gpioGlitchFilter(clk_pin, 0);
   gpioGlitchFilter(data_pin,0);
   gpioSetAlertFuncEx(clk_pin, _callback, this);
   gpioSetAlertFuncEx(data_pin, _callback, this);
}

void RpiZeroRotaryEncoder::callback(int gpio, int level, uint32_t tick) {
  if (gpio == clk_pin_) 
    clk_pin_value_ = level;
  else
    data_pin_value_ = level;
    
  if (gpio != last_gpio_) /* debounce */
   {
      last_gpio_ = gpio;

      if ((gpio == clk_pin_) && (level == 1))
      {
         if (data_pin_value_) setValue(value_ - 1);
      }
      else if ((gpio == data_pin_) && (level == 1))
      {
         if (clk_pin_value_) setValue(value_ + 1);
      }
   }
}

void RpiZeroRotaryEncoder::_callback(int gpio, int level, uint32_t tick, void *user) {
  static_cast<RpiZeroRotaryEncoder*>(user)->callback(gpio, level, tick);
}

RpiZeroRotaryEncoder::~RpiZeroRotaryEncoder() {
  gpioSetAlertFuncEx(clk_pin_, 0, this);
  gpioSetAlertFuncEx(data_pin_, 0, this);
  gpioTerminate();
}


extern "C" {
std::unique_ptr<Input> makeRotaryEncoder(uint8_t clk, uint8_t data,
                                         bool pull_up) {
  return std::unique_ptr<Input>(new RpiZeroRotaryEncoder(clk, data, pull_up));
}
}

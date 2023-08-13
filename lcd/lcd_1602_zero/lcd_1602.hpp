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
#pragma once

#include <lcd.h>
#include <pcf8574.h>
#include <wiringPi.h>
#include <wiringPiI2C.h>

#include <condition_variable>
#include <cppedal/lcd/lcd.hpp>
#include <mutex>
#include <queue>
#include <thread>

int pcf8574_address = 0x27;  // PCF8574T:0x27, PCF8574AT:0x3F
#define BASE 64              // BASE any number above 64
// Define the output pins of the PCF8574, which are directly connected to the
// LCD1602 pin.
#define RS BASE + 0
#define RW BASE + 1
#define EN BASE + 2
#define LED BASE + 3
#define D4 BASE + 4
#define D5 BASE + 5
#define D6 BASE + 6
#define D7 BASE + 7

namespace cppedal::lcd {

class LCD1602 : public LCD {
 private:
  int lcdhd;
  bool clear_ = false;
  std::thread thread_;
  std::mutex mutex_ = {};
  std::condition_variable cv_ = {};
  bool running_ = false;

  std::queue<std::tuple<std::string, int, int>> queue_;

  void loop();

 public:
  LCD1602();
  ~LCD1602();
  void print(const std::string& str, int line, int pos) override;
  void clear() override {
    clear_ = true;
    cv_.notify_one();
  };
};
}  // namespace cppedal::lcd
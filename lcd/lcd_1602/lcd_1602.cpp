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

#include "lcd_1602.hpp"

#include <pthread.h>
#include <unistd.h>

#include <iostream>

using cppedal::lcd::LCD;
using cppedal::lcd::LCD1602;

LCD1602::LCD1602() {
  wiringPiSetup();
  pcf8574Setup(BASE, pcf8574_address);
  for (int i = 0; i < 8; i++) {
    pinMode(BASE + i, OUTPUT);  // set PCF8574 port to output mode
  }
  digitalWrite(LED, HIGH);  // turn on LCD backlight
  digitalWrite(RW, LOW);

  lcdhd = lcdInit(2, 16, 4, RS, EN, D4, D5, D6, D7, 0, 0, 0, 0);
  if (lcdhd == -1) {
    std::cerr << "lcdInit failed" << std::endl;
    exit(-1);
  }

  lcdClear(lcdhd);
  lcdPosition(lcdhd, 3, 0);
  lcdPrintf(lcdhd, "Larrycloud");
  lcdPosition(lcdhd, 4, 1);
  lcdPrintf(lcdhd, "cppedal");

  delayMicroseconds(2000000);
  lcdClear(lcdhd);
  lcdPosition(lcdhd, 3, 0);
  lcdPrintf(lcdhd, "Larry Your   ");
  lcdPosition(lcdhd, 5, 1);
  lcdPrintf(lcdhd, "Life   ");
  delayMicroseconds(2000000);

  thread_ = std::thread(&LCD1602::loop, this);
}

void LCD1602::loop() {
  int oldCState;
  std::unique_lock<std::mutex> lck(mutex_);
  lck.unlock();
  while (1) {
    // Wait for interupt only allow for cancelling when waiting
    // to avoid bads

    lck.lock();
    if (queue_.empty()) {
      pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, &oldCState);
      cv_.wait(lck);
      pthread_setcancelstate(PTHREAD_CANCEL_DISABLE, &oldCState);
    }
    auto data = queue_.front();
    queue_.pop();

    lck.unlock();
    if (clear_) {
      clear_ = false;
      lcdClear(lcdhd);
    }

    lcdPosition(lcdhd, std::get<1>(data), std::get<2>(data));
    lcdPrintf(lcdhd, std::get<0>(data).c_str());
  }
}

void LCD1602::print(const std::string& str, int line, int pos) {
  {
    std::unique_lock<std::mutex> lck(mutex_);
    queue_.emplace(std::make_tuple(str, pos, line));
  }
  cv_.notify_one();
}

LCD1602::~LCD1602() {
  if (thread_.joinable()) {
    pthread_cancel(thread_.native_handle());
    thread_.join();
  }
}

extern "C" {
std::unique_ptr<LCD> makeLCD() { return std::unique_ptr<LCD>(new LCD1602); }
}

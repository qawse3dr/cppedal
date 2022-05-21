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

#include <spidev_lib++.h>

#include <memory>

#include "cppedal/ingestor/ingestor.hpp"

namespace cppedal::ingestor {

class mcp3002 {
 private:
  spi_config_t spi_config_ = {};
  std::unique_ptr<SPI> spi_ = {};

 public:
  mcp3002() {
    spi_config_.mode = 0;
    spi_config_.speed = 2000000;
    spi_config_.delay = 0;
    spi_config_.bits_per_word = 8;

    // do setup
    spi_ = std::unique_ptr<SPI>(new SPI("/dev/spidev0.0", &spi_config_));
    spi_->begin();
  }

  // only reads channel 1
  uint32_t read() {
    uint8_t mosi[10] = {0x01, 0x00, 0x00};  //< 10 bit input
    uint8_t miso[10] = {0};                 //< 10 bit output
    uint32_t input_signal;
    spi_->xfer(mosi, 10, miso, 12);
    input_signal = miso[2] + ((miso[1] & 0x0F) << 8);

    return input_signal;
  }
};

class MCP3002Ingestor : public Ingestor {
 public:
  MCP3002Ingestor();
  ~MCP3002Ingestor();
  mcp3002 adc;
  uint32_t ingest() override;
};

}  // namespace cppedal::ingestor

extern "C" {
std::unique_ptr<cppedal::ingestor::Ingestor> makeIngestor();
}
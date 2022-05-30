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

#include "mcp3002.hpp"

#include <wiringPi.h>
#include <wiringPiSPI.h>

#include <cstdlib>

using cppedal::ingestor::Ingestor;
using cppedal::ingestor::MCP3002Ingestor;

static int myAnalogRead(struct wiringPiNodeStruct *node, int pin) {
  unsigned char spiData[2];
  unsigned char chanBits;
  int chan = pin - node->pinBase;

  if (chan == 0)
    chanBits = 0b11010000;
  else
    chanBits = 0b11110000;

  spiData[0] = chanBits;
  spiData[1] = 0;

  wiringPiSPIDataRW(node->fd, spiData, 2);

  return (spiData[1] | ((spiData[0] & 0x0F) << 8));
}

#define CPPEDAL_BASE_BIN 80
MCP3002Ingestor::MCP3002Ingestor() {
  struct wiringPiNodeStruct *node;

  if (wiringPiSetup() || wiringPiSPISetup(0, 1000000) < 0) exit(-1);

  node = wiringPiNewNode(CPPEDAL_BASE_BIN, 2);

  node->fd = 0;
  node->analogRead = myAnalogRead;
}

MCP3002Ingestor::~MCP3002Ingestor() {}

uint32_t MCP3002Ingestor::ingest() { return analogRead(CPPEDAL_BASE_BIN); }

extern "C" {
std::unique_ptr<Ingestor> makeIngestor() {
  return std::unique_ptr<Ingestor>(new MCP3002Ingestor);
}
}

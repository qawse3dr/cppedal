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

#include <mcp3002.h>
#include <wiringPi.h>

using cppedal::ingestor::Ingestor;
using cppedal::ingestor::MCP3002Ingestor;

#define MCP3002_BASE_PIN 100
MCP3002Ingestor::MCP3002Ingestor() {
  wiringPiSetup();
  mcp3002Setup(MCP3002_BASE_PIN, 0);
}

MCP3002Ingestor::~MCP3002Ingestor() {}

int64_t MCP3002Ingestor::ingest() { return analogRead(MCP3002_BASE_PIN); }

std::unique_ptr<Ingestor> cppedal::ingestor::makeIngestor() {
  return std::unique_ptr<Ingestor>(new MCP3002Ingestor);
}

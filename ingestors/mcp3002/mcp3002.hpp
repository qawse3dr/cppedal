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

class MCP3002Ingestor : public Ingestor {
 public:
  MCP3002Ingestor();
  ~MCP3002Ingestor();
  uint32_t ingest() override;
};

}  // namespace cppedal::ingestor

extern "C" {
std::unique_ptr<cppedal::ingestor::Ingestor> makeIngestor();
}
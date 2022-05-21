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


#include "cppedal/ingestor/ingestor.hpp"

#include <memory>

namespace cppedal::ingestor {

class MCP3002Ingestor : public Ingestor {
public:
  MCP3002Ingestor();
  ~MCP3002Ingestor();

  int64_t ingest() override;
};

std::unique_ptr<Ingestor> makeIngestor();

} // namespace cppedal::ingestor
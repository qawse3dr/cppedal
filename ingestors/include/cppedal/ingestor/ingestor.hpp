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

#include <stdint.h>

#include <memory>

namespace cppedal::ingestor {

class Ingestor {
 public:
  Ingestor() = default;
  virtual ~Ingestor() = default;

  /**
   * @brief Pulls the data off of the adc
   *
   * @return int64_t the adc value.
   */
  virtual uint32_t ingest() = 0;
};

}  // namespace cppedal::ingestor

extern "C" {
typedef std::unique_ptr<cppedal::ingestor::Ingestor> (*makeIngestorFtn)();
}
#define CPPEDAL_MAKE_INGESTOR_NAME "makeIngestor"
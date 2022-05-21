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

#include <stdint.h>

namespace cppedal::ingestor {

class Ingestor {
public:
  Ingestor();
  virtual ~Ingestor() = 0;

  /**
   * @brief Pulls the data off of the adc
   *
   * @return int64_t the adc value.
   */
  virtual int64_t ingest() = 0;
};

} // namespace cppedal::ingestor
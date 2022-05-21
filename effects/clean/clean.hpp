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

#include "cppedal/effect/effect.hpp"

#include <memory>

namespace cppedal::effects {

class CleanEffect : public Effect {
 public:
  CleanEffect(const EffectConfig& cfg) : Effect(cfg) {}

  int64_t process(int64_t in) override;


};

// Makes the clean effect
// All effects must have this
Effect* makeEffect(const EffectConfig&);


} // namespace cppedal::effects
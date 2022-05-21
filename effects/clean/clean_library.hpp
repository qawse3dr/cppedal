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


#include "cppedal/effect/effect_library.hpp"

#include <memory>

namespace cppedal::effects {

class CleanEffectLibrary : public EffectLibrary {
public:
  explicit CleanEffectLibrary(const EffectLibraryConfig &cfg)
      : EffectLibrary(cfg) {}
  ~CleanEffectLibrary() = default;
  int64_t process(int64_t in) override;
};


} // namespace cppedal::effects

extern "C" {

// Makes the clean effect
// All effects must have this
std::unique_ptr<cppedal::effects::EffectLibrary> makeEffectLibrary(const cppedal::effects::EffectLibraryConfig &);
}
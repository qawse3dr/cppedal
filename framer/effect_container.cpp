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

#include "effect_container.hpp"

using cppedal::effects::EffectLibrary;
using cppedal::framer::EffectContainer;

EffectContainer::EffectContainer(
    const std::string& name,
    std::vector<std::shared_ptr<EffectLibrary>>&& effect)
    : name_(name), effects_(std::move(effect)) {}
uint32_t EffectContainer::process(uint32_t in) {
  for (const auto& effect : effects_) {
    in = effect->process(in);
  }
  return in;
}
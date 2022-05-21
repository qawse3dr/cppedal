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

#include <memory>
#include <map>

namespace cppedal::framer {

/**
 * @brief This class will hold the whole project together
 * Have
 * - ingestor
 * - pwm-output
 * - effects
 * 
 * and run the whole applications
 * 
 */
class Framer {
 private:
  // IO of sound
  std::unique_ptr<cppedal::ingestor::Ingestor> ingst_;
  std::unique_ptr<cppedal::pwm_ouput::PwmOuput> output_;

  std::map<std::string, std::unique_ptr<cppedal::effects::Effect>> effect_map_;

  
}

} // namespace cppedal::framer
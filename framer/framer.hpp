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

#include <map>
#include <memory>

// Includes for components
#include <cppedal/effect/effect_library.hpp>
#include <cppedal/pwm_output/pwm_output.hpp>
#include <cppedal/ingestor/ingestor.hpp>



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
  std::unique_ptr<cppedal::ingestor::Ingestor> ingst_ = {};
  std::unique_ptr<cppedal::pwm_output::PwmOutput> output_ = {};

  std::map<std::string, std::unique_ptr<cppedal::effects::EffectLibrary>> effect_map_ = {};



  public: 
  
  explicit Framer(const std::string& cfg_path);
  ~Framer() = default;

  bool start();

  void stop();
};

} // namespace cppedal::framer
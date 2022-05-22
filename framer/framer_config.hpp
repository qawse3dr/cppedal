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

#include <limits>
#include <string>
#include <vector>

namespace cppedal::framer {

struct FramerConfig {
  struct LibraryInfo {
    std::string name;
    std::string path;
    void* handler = nullptr;
  };

  struct PushButtonInfo {
    std::string name;
    int pin;
    bool pull_up;
  };
  struct RotaryEncoderInfo {
    std::string name;
    int clk;
    int data;
    bool pull_up;
  };

  struct EffectInputInfo {
    std::string target;
    std::string key;
    int64_t max = std::numeric_limits<int64_t>::max();
    int64_t min = std::numeric_limits<int64_t>::min();
    int64_t start = 0;
  };

  struct EffectInfo {
    std::string name;
    std::vector<std::string> effect_libraries;
    std::vector<EffectInputInfo> input_mapping;
  };

  // Libraries
  LibraryInfo ingestor;
  LibraryInfo pwm_output;
  LibraryInfo lcd;
  LibraryInfo input_library;

  std::vector<LibraryInfo> effects_libraries;

  // Inputs
  std::vector<PushButtonInfo> push_button_info;
  std::vector<RotaryEncoderInfo> rotary_encoder_info;

  std::vector<EffectInfo> effects_info;

  std::string prev_effect_button;
  std::string next_effect_button;
};

}  // namespace cppedal::framer
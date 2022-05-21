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

#include <memory>
#include <stdint.h>

#include <cppedal/pwm_output/pwm_output.hpp>

namespace cppedal::pwm_output {

class RpiSinglePwmOutput : public PwmOutput {
 public:
  RpiSinglePwmOutput();
  ~RpiSinglePwmOutput();

  void output(int64_t out) override;
};

std::unique_ptr<PwmOutput> makePwmOutput();


} // namespace cppedal::pwm_output
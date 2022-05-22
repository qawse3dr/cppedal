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

namespace cppedal::lcd {

class LCD {
 public:
  LCD() = default;
  virtual ~LCD(){};
  virtual void print(const std::string& str, int line, int pos) = 0;
  virtual void clear() = 0;
};

}  // namespace cppedal::lcd

extern "C" {
typedef std::unique_ptr<cppedal::lcd::LCD> (*makeLCDftn)();
}
#define CPPEDAL_MAKE_LCD_NAME "makeLCD"
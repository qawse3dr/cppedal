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

#include <chrono>
#include <iostream>
#include <thread>

#include "framer.hpp"

int main(int argc, char *argv[]) {
  std::cout << "Larrycloud CPPEDAL" << std::endl;

  cppedal::framer::Framer framer("../cppedal.json");

  framer.start();

  std::this_thread::sleep_for(std::chrono::seconds(60));

  framer.stop();
}
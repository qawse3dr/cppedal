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

#include <signal.h>

#include <chrono>
#include <iostream>

#include "framer.hpp"

int main(int argc, char *argv[]) {
  sigset_t sigset;
  sigaddset(&sigset, SIGTERM);
  sigaddset(&sigset, SIGINT);

  pthread_sigmask(SIG_BLOCK, &sigset, nullptr);

  std::cout << "Larrycloud CPPEDAL" << std::endl;

  if (argc != 2) {
    std::cerr << "Please pass config as first argument" << std::endl;
    return -1;
  }
  cppedal::framer::Framer framer(argv[1]);

  framer.start();

  while (true) {
    int sig = sigwaitinfo(&sigset, nullptr);
    if (sigismember(&sigset, sig)) {
      break;
    }
  }
  framer.stop();
}
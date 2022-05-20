#
# (C) Copyright 2022 Larry Milne (https://www.larrycloud.ca)
# This code is distributed on "AS IS" BASIS,
# WITHOUT WARRANTINES OR CONDITIONS OF ANY KIND.
# See the License for the specific language governing permissions and
# limitations under the License.
#
# @author: qawse3dr a.k.a Larry Milne
#

cmake_minimum_required (VERSION 3.18)
project(cppedal VERSION 1.0)

set (CXX_STANDARD C++20)


add_subdirectory(effects)
add_subdirectory(framer)
add_subdirectory(pwm-output)
add_subdirectory(ingestors)

# nlohmann JSON https://github.com/nlohmann/json
add_library(json INTERFACE)
target_include_directories(json INTERFACE third-party/json)


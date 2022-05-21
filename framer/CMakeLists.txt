#
# (C) Copyright 2022 Larry Milne (https://www.larrycloud.ca)
# This code is distributed on "AS IS" BASIS,
# WITHOUT WARRANTINES OR CONDITIONS OF ANY KIND.
# See the License for the specific language governing permissions and
# limitations under the License.
#
# @author: qawse3dr a.k.a Larry Milne
#


# Set build folder
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

add_executable(cppedal
  main.cpp
  framer.cpp
)

target_include_directories(cppedal
    PUBLIC
    $(CPPEDAL_JSON_HEADER_PATH)
)

target_link_libraries(cppedal
  PUBLIC
  ingestor
  pwm-output
  cppedal-effects
  cppedal-input
  dl
  pthread
)
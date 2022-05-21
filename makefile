#
# (C) Copyright 2022 Larry Milne (https://www.larrycloud.ca)
# This code is distributed on "AS IS" BASIS,
# WITHOUT WARRANTINES OR CONDITIONS OF ANY KIND.
# See the License for the specific language governing permissions and
# limitations under the License.
#
# @author: qawse3dr a.k.a Larry Milne
#


all: build


init:
	mkdir -p build; cd build; cmake ..

build: init
	cd build; make -j12

build-framer: init
	cd build; make -j12 framer

build-effect: init
	cd build; make -j12 effect

build-ingestor: init
	cd build; make -j12 mcp6002-ingestor

build-pwm: init
	cd build; make -j12 rpi4-pwm-output
deploy:
	echo "TODO"

run: build
	cd build; sudo ./bin/cppedal

run-no-build:
	cd build; sudo ./bin/cppedal

valgrind: build
	cd build; sudo valgrind ./bin/cppedal

clean:
	rm -rf ./build

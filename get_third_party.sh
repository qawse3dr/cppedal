#!/bin/bash

mkdir -p third-party


#json
mkdir -p third-party/json
pushd third-party/json
wget https://github.com/nlohmann/json/releases/download/v3.11.2/json.hpp
popd

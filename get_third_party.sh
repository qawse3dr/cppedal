#!/bin/bash

mkdir -p third-party


#json
mkdir -p third-party/json
pushd third-party/json
wget https://github.com/nlohmann/json/releases/download/v3.10.5/json.hpp third-party/json/
popd
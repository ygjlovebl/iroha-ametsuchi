/**
 * Copyright Soramitsu Co., Ltd. 2016 All Rights Reserved.
 * http://soramitsu.co.jp
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *        http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <ametsuchi/file/file.h>
#include <ametsuchi/globals.h>
#include <ametsuchi/serializer.h>
#include <benchmark/benchmark.h>


static void FileWrite(benchmark::State& state) {
  using ametsuchi::ByteArray;
  using ametsuchi::file::ReadWriteFile;

  while (state.KeepRunning()) {
    size_t size = state.range(0);

    std::string filename = "/tmp/test1";
    ReadWriteFile writeFile(filename);
    writeFile.remove();

    if (writeFile.open()) {
      for (uint32_t i = 0; i < size; i++) {
        ByteArray memory(4);
        uint8_t* ptr = memory.data();
        PUT_UINT(ptr, i, uint32_t);
        writeFile.append(memory);
      }
    }
    writeFile.close();
  }
}

static void FileRead(benchmark::State& state) {
  using ametsuchi::ByteArray;
  using ametsuchi::file::ReadOnlyFile;

  while (state.KeepRunning()) {
    size_t size = state.range(0);

    std::string filename = "/tmp/test1";
    ReadOnlyFile readFile(filename);

    if (readFile.open()) {
      for (uint32_t i = 0; i < size; i++) {
        ByteArray memory = readFile.read(4);
        uint8_t* ptr = memory.data();
        uint32_t a = 0;
        GET_UINT(&a, ptr, uint32_t);
        assert(a == i);
      }
    }
    readFile.close();
  }
}

BENCHMARK(FileWrite)->Range(1, 1 << 20);
BENCHMARK(FileRead)->Range(1, 1 << 20);
BENCHMARK_MAIN();

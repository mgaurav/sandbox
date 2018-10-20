#include <sstream>    // std::ostringstream
#include <string>     // std::string

#include <benchmark/benchmark.h>  // BENCHMARK

namespace {

void BM_stringAppend(benchmark::State& state) {
  std::string str;
  std::string datum(state.range(1), 'a');
  for (auto _ : state) {
    for (size_t i = 0; i < state.range(0); ++i)
      str.append(datum); 
  }
}

void BM_stringStream(benchmark::State& state) {
  std::ostringstream oss;
  std::string datum(state.range(1), 'a');
  for (auto _ : state) {
    for (size_t i = 0; i < state.range(0); ++i)
      oss << datum;
    oss.str();
  }
}

void BM_stringConcat(benchmark::State& state) {
  std::string str;
  std::string datum(state.range(1), 'a');
  for (auto _ : state) {
    for (size_t i = 0; i < state.range(0); ++i) {
      str = str + datum;
    }
  }
}

}  // namespace

BENCHMARK(BM_stringAppend)->Ranges({{1, 1000}, {1, 100}});
BENCHMARK(BM_stringStream)->Ranges({{1, 1000}, {1, 100}});
BENCHMARK(BM_stringConcat)->Ranges({{1, 1000}, {1, 100}});

BENCHMARK_MAIN();

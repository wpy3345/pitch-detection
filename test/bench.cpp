#include "pitch_detection.h"
#include "util.h"
#include <benchmark/benchmark.h>

static void
BM_Yin_Sinewave(benchmark::State &state)
{
	auto data = test_util::sinewave(state.range(0), 1337, 48000);
	for (auto _ : state)
		pitch::yin(data, 48000);
	state.SetComplexityN(state.range(0));
}

static void
BM_Mpm_Sinewave(benchmark::State &state)
{
	auto data = test_util::sinewave(state.range(0), 1337, 48000);
	for (auto _ : state)
		pitch::mpm(data, 48000);
	state.SetComplexityN(state.range(0));
}

static void
BM_Yin_Sinewave_Alloc(benchmark::State &state)
{
	auto data = test_util::sinewave(state.range(0), 1337, 48000);
	pitch_alloc::Yin ya(data.size());
	for (auto _ : state)
		pitch_alloc::yin(data, 48000, &ya);
	state.SetComplexityN(state.range(0));
}

static void
BM_Mpm_Sinewave_Alloc(benchmark::State &state)
{
	auto data = test_util::sinewave(state.range(0), 1337, 48000);
	pitch_alloc::Mpm ma(data.size());
	for (auto _ : state)
		pitch_alloc::mpm(data, 48000, &ma);
	state.SetComplexityN(state.range(0));
}

BENCHMARK(BM_Yin_Sinewave)->Range(1 << 10, 1 << 20)->Complexity();
BENCHMARK(BM_Mpm_Sinewave)->Range(1 << 10, 1 << 20)->Complexity();

BENCHMARK(BM_Yin_Sinewave_Alloc)->Range(1 << 10, 1 << 20)->Complexity();
BENCHMARK(BM_Mpm_Sinewave_Alloc)->Range(1 << 10, 1 << 20)->Complexity();

BENCHMARK_MAIN();

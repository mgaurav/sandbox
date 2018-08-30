#include <bases.h>                  // Bases::toBase36
#include <benchmark/benchmark.h>    // BENCHMARK, BENCHMARK_MAIN

namespace
{

void BM_baseline( benchmark::State& state )
{
    for( auto _: state )
    {
        std::string s( "100" );
    }
}

void BM_toBase36_size16( benchmark::State& state )
{
    std::string input( 16, 'f' );
    for( auto _: state )
    {
        Bases::toBase36( input );
    }
}

void BM_toBase36Horner_size16( benchmark::State& state )
{
    std::string input( 16, 'f' );
    for( auto _: state )
    {
        Bases::toBase36Horner( input );
    }
}

void BM_toBase36Horner_size1000( benchmark::State& state )
{
    std::string input( 1000, 'f' );
    for( auto _: state )
    {
        Bases::toBase36Horner( input );
    }
}

void BM_toBase36HornerOptimized_size1000( benchmark::State& state )
{
    std::string input( 1000, 'f' );
    for( auto _: state )
    {
        Bases::toBase36HornerOptimized( input );
    }
}

} // namespace

BENCHMARK( BM_baseline );
BENCHMARK( BM_toBase36_size16 );
BENCHMARK( BM_toBase36Horner_size16 );
BENCHMARK( BM_toBase36Horner_size1000 );
BENCHMARK( BM_toBase36HornerOptimized_size1000 );

BENCHMARK_MAIN();

#include <bases.h>          // Bases:toBase16, toBase36
#include <gtest/gtest.h>    // TEST, RUN_ALL_TESTS

namespace
{

TEST( ToBase36, Basic )
{
#define TO_BASE_36 Bases::toBase36HornerOptimized
    EXPECT_EQ( "0", TO_BASE_36( "0" ) );
    EXPECT_EQ( "1", TO_BASE_36( "1" ) );
    EXPECT_EQ( "2", TO_BASE_36( "2" ) );
    EXPECT_EQ( "3", TO_BASE_36( "3" ) );
    EXPECT_EQ( "4", TO_BASE_36( "4" ) );
    EXPECT_EQ( "5", TO_BASE_36( "5" ) );
    EXPECT_EQ( "6", TO_BASE_36( "6" ) );
    EXPECT_EQ( "7", TO_BASE_36( "7" ) );
    EXPECT_EQ( "8", TO_BASE_36( "8" ) );
    EXPECT_EQ( "9", TO_BASE_36( "9" ) );
    EXPECT_EQ( "a", TO_BASE_36( "a" ) );
    EXPECT_EQ( "b", TO_BASE_36( "b" ) );
    EXPECT_EQ( "c", TO_BASE_36( "c" ) );
    EXPECT_EQ( "d", TO_BASE_36( "d" ) );
    EXPECT_EQ( "e", TO_BASE_36( "e" ) );
    EXPECT_EQ( "f", TO_BASE_36( "f" ) );

    EXPECT_EQ( "g", TO_BASE_36( "10" ) );
    EXPECT_EQ( "v", TO_BASE_36( "1f" ) );
    EXPECT_EQ( "w", TO_BASE_36( "20" ) );
    EXPECT_EQ( "z", TO_BASE_36( "23" ) );
    EXPECT_EQ( "10", TO_BASE_36( "24" ) );
    EXPECT_EQ( "11", TO_BASE_36( "25" ) );
    EXPECT_EQ( "1b", TO_BASE_36( "2f" ) );
    EXPECT_EQ( "1c", TO_BASE_36( "30" ) );

    EXPECT_EQ( "74", TO_BASE_36( "100" ) );
#undef TO_BASE_36

}

TEST( ToBase16, Basic )
{
#define TO_BASE_16 Bases::toBase16Horner
    EXPECT_EQ( "0", TO_BASE_16( "0" ) );
    EXPECT_EQ( "1", TO_BASE_16( "1" ) );
    EXPECT_EQ( "2", TO_BASE_16( "2" ) );
    EXPECT_EQ( "3", TO_BASE_16( "3" ) );
    EXPECT_EQ( "4", TO_BASE_16( "4" ) );
    EXPECT_EQ( "5", TO_BASE_16( "5" ) );
    EXPECT_EQ( "6", TO_BASE_16( "6" ) );
    EXPECT_EQ( "7", TO_BASE_16( "7" ) );
    EXPECT_EQ( "8", TO_BASE_16( "8" ) );
    EXPECT_EQ( "9", TO_BASE_16( "9" ) );
    EXPECT_EQ( "a", TO_BASE_16( "a" ) );
    EXPECT_EQ( "b", TO_BASE_16( "b" ) );
    EXPECT_EQ( "c", TO_BASE_16( "c" ) );
    EXPECT_EQ( "d", TO_BASE_16( "d" ) );
    EXPECT_EQ( "e", TO_BASE_16( "e" ) );
    EXPECT_EQ( "f", TO_BASE_16( "f" ) );

    EXPECT_EQ( "10", TO_BASE_16( "g" ) );
    EXPECT_EQ( "1f", TO_BASE_16( "v" ) );
    EXPECT_EQ( "20", TO_BASE_16( "w" ) );
    EXPECT_EQ( "23", TO_BASE_16( "z" ) );
    EXPECT_EQ( "24", TO_BASE_16( "10" ) );
    EXPECT_EQ( "25", TO_BASE_16( "11" ) );
    EXPECT_EQ( "2f", TO_BASE_16( "1b" ) );
    EXPECT_EQ( "30", TO_BASE_16( "1c" ) );

    EXPECT_EQ( "100", TO_BASE_16( "74" ) );
#undef TO_BASE_16
}

TEST( Bases, Idempotence )
{
#define TO_BASE_36 Bases::toBase36HornerOptimized
    for( size_t i = 0; i < 100; ++i )
    {
        std::string base16 = std::string( i, 'f' );
        std::string base36 = TO_BASE_36( base16 );
        EXPECT_EQ( base16, Bases::toBase16Horner( base36 ) );
    }
    
    for( size_t i = 0; i < 100; ++i )
    {
        std::string base36 = std::string( i, 'z' );
        std::string base16 = Bases::toBase16Horner( base36 );
        EXPECT_EQ( base36, TO_BASE_36( base16 ) );
    }

#undef TO_BASE_36
}

} // namespace

int main( int argc, char** argv )
{
    testing::InitGoogleTest( &argc, argv );
    return RUN_ALL_TESTS();
}


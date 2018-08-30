#include <bases.h>      // Bases::toBase16, toBase36

#include <cmath>        // std::ceil, std::log2
#include <stdexcept>    // std::range_error
#include <string>       // std::string
#include <vector>       // std::vector

namespace
{

char const BASE_16_CHARS[] = "0123456789abcdef";
char const BASE_36_CHARS[] = "0123456789abcdefghijklmnopqrstuvwxyz";

static_assert( sizeof( BASE_16_CHARS ) == 16 + 1 );
static_assert( sizeof( BASE_36_CHARS ) == 36 + 1 );

constexpr double DIGITS_RATIO_16_TO_36 = std::log2( 16 ) / std::log2( 36 );
constexpr double DIGITS_RATIO_36_TO_16 = std::log2( 36 ) / std::log2( 16 );

size_t digitToIndex( char const digit )
{
    if( digit >= '0' && digit <= '9' )
        return digit - '0';
    else if( digit >= 'a' && digit <= 'z' )
        return 10 + ( digit - 'a' );
    else
        throw std::range_error( "Invalid input digit: " + digit );
}

} // namespace

namespace Bases
{

/*
** Base-36 to Base-16 conversion.
** Doesn't handle the case where evaluated value of Base-36 input
** can overflow size_t range.
*/
std::string toBase16( std::string const& input )
{
    if( input.empty() )
        return input;

    if( input.length() == 1 && input[ 0 ] <= 'f' )
        return input;

    size_t const numDigits = std::ceil( input.length() * DIGITS_RATIO_36_TO_16 );
    std::string result( numDigits, '0' );

    size_t value = digitToIndex( input[ 0 ] ); 
    for( size_t j = 1; j < input.length(); ++j )
        value = digitToIndex( input[ j ] ) + 36 * value;

    size_t i = 0;
    do
    {
        result[ numDigits - i - 1 ] = BASE_16_CHARS[ value % 16 ];
        value /= 16;
        ++i;
    } while( value != 0 );

    // Strip leading zeroes
    size_t pos = result.find_first_not_of( '0' );
    if( pos != std::string::npos )
        result.erase( 0, pos );
    return result;
}

/*
** Base-16 to Base-36 conversion.
** Doesn't handle the case where evaluated value of Base-16 input
** can overflow size_t range.
*/
std::string toBase36( std::string const& input )
{
    if( input.empty() || input.length() == 1 )
        return input;

    size_t const numDigits = std::ceil( input.length() * DIGITS_RATIO_16_TO_36 );
    std::string result( numDigits, '0' );

    size_t value = digitToIndex( input[ 0 ] ); 
    for( size_t j = 1; j < input.length(); ++j )
        value = digitToIndex( input[ j ] ) + 16 * value;

    size_t i = 0;
    do
    {
        result[ numDigits - i - 1 ] = BASE_36_CHARS[ value % 36 ];
        value /= 36;
        ++i;
    } while( value != 0 );

    // Strip leading zeroes
    size_t pos = result.find_first_not_of( '0' );
    if( pos != std::string::npos )
        result.erase( 0, pos );
    return result;
}

/*
** Base-36 to Base-16 conversion.
** Handles the overflow case.
*/
std::string toBase16Horner( std::string const& input )
{
    if( input.empty() )
        return input;

    if( input.length() == 1 && input[ 0 ] <= 'f' )
        return input;

    size_t const numDigits = std::ceil( input.length() * DIGITS_RATIO_36_TO_16 );
    std::vector< int > digits( numDigits );

    size_t value = digitToIndex( input[ 0 ] );
    // Single Base-36 digit can be represented by at most 2 Base-16 digits
    digits[ 0 ] = value % 16;
    digits[ 1 ] = value / 16;
    for( size_t j = 1; j < input.length(); ++j )
    {
        size_t carry = digitToIndex( input[ j ] );
        for( size_t i = 0; i < numDigits; ++i )
        {
            size_t const value = carry + digits[ i ] * 36;
            digits[ i ] = value % 16;
            carry = value / 16;
        }
    } 

    std::string result( numDigits, '0' );
    for( size_t i = 0; i < numDigits; ++i )
        result[ numDigits - i - 1 ] = BASE_16_CHARS[ digits[ i ] ];

    // Strip leading zeroes
    size_t pos = result.find_first_not_of( '0' );
    if( pos != std::string::npos )
        result.erase( 0, pos );
    return result;
}

/*
** Base-16 to Base-36 conversion.
** Handles the overflow case.
*/
std::string toBase36Horner( std::string const& input )
{
    if( input.empty() || input.length() == 1 )
        return input;

    size_t const numDigits = std::ceil( input.length() * DIGITS_RATIO_16_TO_36 );
    std::vector< int > digits( numDigits );

    size_t value = digitToIndex( input[ 0 ] );
    digits[ 0 ] = value % 36;
    for( size_t j = 1; j < input.length(); ++j )
    {
        size_t carry = digitToIndex( input[ j ] );
        for( size_t i = 0; i < numDigits; ++i )
        {
            size_t const value = carry + digits[ i ] * 16;
            digits[ i ] = value % 36;
            carry = value / 36;
        }
    } 

    std::string result( numDigits, '0' );
    for( size_t i = 0; i < numDigits; ++i )
        result[ numDigits - i - 1 ] = BASE_36_CHARS[ digits[ i ] ];

    // Strip leading zeroes
    size_t pos = result.find_first_not_of( '0' );
    if( pos != std::string::npos )
        result.erase( 0, pos );
    return result;
}

/*
** Base-16 to Base-36 conversion.
** Handles the overflow case and optimized to work with multiple
** digits at a time while performing arithmetic
*/
std::string toBase36HornerOptimized( std::string const& input )
{
    if( input.empty() || input.length() == 1 )
        return input;

    std::vector< uint8_t > inputDigits( input.length() );
    for( size_t i = 0; i < input.length(); ++i )
        inputDigits[ i ] = digitToIndex( input[ i ] );

    size_t const numBase36Digits = std::ceil( input.length() * DIGITS_RATIO_16_TO_36 );

    // Can handle 6 digits of Base-36 in one go without overflowing size_t
    // when performing arithmetic according to Horner's method
    size_t const base36ChunkSize = 6;
    std::vector< size_t > digits( std::ceil( static_cast< double >( numBase36Digits ) / base36ChunkSize ) );
    size_t const numDigits = digits.size();
    size_t constexpr base36Divisor = std::pow( 36, base36ChunkSize );

    // Can handle 8 digits of Base-16 in one go without overflowing size_t
    // when performing arithmetic according to Horner's method
    size_t const base16ChunkSize = 8;
    size_t const numChunks = input.length() / base16ChunkSize;
    size_t endOffset = input.length() % base16ChunkSize;

    auto evaluateBase16Value = [inputDigits]( size_t start, size_t end )
    {
        size_t value = inputDigits[ start ];
        for( size_t i = start + 1; i < end; ++i )
            value = inputDigits[ i ] + 16 * value;
        return value;
    };

    // Handle any extra digits at the beginning which is not included in chunks
    if( endOffset != 0 )
    {
        size_t carry = evaluateBase16Value( 0, endOffset );
        int i = 0;
        do
        {
            digits[ i++ ] = carry % base36Divisor;
            carry /= base36Divisor;
        } while( carry != 0 );
    }

    for( size_t j = 0; j < numChunks; ++j )
    {
        endOffset += base16ChunkSize;
        size_t carry = evaluateBase16Value( endOffset - base16ChunkSize, endOffset ); 
        for( size_t i = 0; i < numDigits; ++i )
        {
            size_t const value = carry + digits[ i ] * ( size_t{ 1 } << 32 ); // 16^8
            digits[ i ] = value % base36Divisor;
            carry = value / base36Divisor;
        }
    } 

    // Convert each chunk digit to 6 Base-36 digits
    std::string result( numDigits * base36ChunkSize, '0' );
    size_t const len = result.length();
    for( size_t i = 0; i < numDigits; ++i )
    {
        size_t value = digits[ i ];
        size_t j = 0;
        size_t const offset = len - base36ChunkSize * i;
        do
        {
            result[ offset - j - 1 ] = BASE_36_CHARS[ value % 36 ];
            value /= 36;
            ++j;
        } while( j < base36ChunkSize );
    }

    // Strip leading zeroes
    size_t pos = result.find_first_not_of( '0' );
    if( pos != std::string::npos )
        result.erase( 0, pos );
    return result;
}

} // namespace Bases

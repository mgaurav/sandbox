#ifndef BASES_H
#define BASES_H

#include <string> // std::string

namespace Bases
{

std::string toBase16( std::string const& input );

std::string toBase36( std::string const& input );

std::string toBase16Horner( std::string const& input );

std::string toBase36Horner( std::string const& input );

std::string toBase36HornerOptimized( std::string const& input );

} // namespace Bases

#endif // !BASES_H

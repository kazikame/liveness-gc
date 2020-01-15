#include "Output.h"

using namespace Scheme::output;

std::ostream & Scheme::output::indentTill(std::ostream & stream, unsigned depth) {
    if(depth)       stream.put('\n');
    while(depth--)  stream.put(' ');
    return stream;
}
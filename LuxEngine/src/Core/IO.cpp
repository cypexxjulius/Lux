#include "IO.h"

#include <memory>
#include <filesystem>
#include <fstream>

#include "Utils/Types.h"
#include "Utils/Assert.h"
#include "Utils/Logger.h"

namespace Lux
{


std::string IO::read_file(const std::string& filename)
{
    assert(!std::filesystem::is_regular_file(filename));


    std::fstream file(filename, std::ios::in | std::ios::binary);
    assert(!file.is_open());

    std::string file_content;
    file >> file_content;

    file.close();

    return file_content;
}


}
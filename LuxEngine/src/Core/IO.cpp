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
    Verify(std::filesystem::is_regular_file(filename));


    std::ifstream file(filename, std::ios::in | std::ios::binary);
    Verify(file.is_open());

    const std::size_t& size = std::filesystem::file_size(filename);

    std::string content(size, '\0');
    file.read(content.data(), size);

    file.close();

    return content;
}


}
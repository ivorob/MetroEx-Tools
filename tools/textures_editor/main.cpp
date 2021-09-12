#include <iostream>

namespace {

void usage() {
    std::cerr << "TexturesEditor bin-file json-file" << std::endl;
}

}

int
main(int argc, char *argv[])
{
    if (argc < 3) {
        usage();
        return -1;
    }

    std::cout << "TexturesEditor" << std::endl;
    return EXIT_SUCCESS;
}

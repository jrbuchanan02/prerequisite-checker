
#include <main.h++>

#include <fstream>
#include <iostream>

void handleTerminate()
{
    std::cout << "Terminate called... Aborting...\n";
    std::abort();
}

int main(int const argc, char const *const *const argv)
{
    //#ifdef __POSIX__
    // for (int i = 0; i < argc; i++ ) {
    //    std::cout << argv [ i ] << " ";
    //    std::cout << std::fstream ( argv[i] ).bad ( ) << "\n";
    //}
    //#endif
    std::set_terminate(&handleTerminate);
    try
    {
        parseArgs(argc, argv);
    }
    catch (std::exception &exception)
    {
        std::cout << exception.what() << "\n";
    }
    catch (...)
    {
        std::cout << "Something unknown was thrown!\n";
    }
}
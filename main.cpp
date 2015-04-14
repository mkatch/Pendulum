#include "application.hpp"

int main(int argc, char** argv)
{
    int retCode;
    try
    {
        jg::Application app(argc, argv);
        retCode = app.exec();
    }
    catch (std::exception& e)
    {
        std::cerr << "Exception caught:\n\t" << e.what() << std::endl;
        retCode = 1;
    }
    catch(...){ std::cerr << "Unknown exception caught." << std::endl; }

    return retCode;
}

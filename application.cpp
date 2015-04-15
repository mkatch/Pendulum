#include "application.hpp"

namespace jg {

Application::Application(
    int     argc,
    char**  argv
)
:   QApplication(argc, argv)
{
    mainWindow.setCentralWidget(&centralWidget);
    mainWindow.setMinimumSize(900, 770);
    mainWindow.show();
}

} // namespace jg

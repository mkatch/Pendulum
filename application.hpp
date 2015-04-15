#ifndef JG_APPLICATION_HPP
#define JG_APPLICATION_HPP

#include <QtGui>

#include "central_widget.hpp"

namespace jg
{

class Application : public QApplication
{
    Q_OBJECT

public:
    Application(int argc, char** argv);

private:
    QMainWindow     mainWindow;
    CentralWidget   centralWidget;
};

} // namespace jg

#endif // JG_APPLICATION_HPP

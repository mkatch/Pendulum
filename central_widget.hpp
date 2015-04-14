#ifndef JG_CENTRAL_WIDGET_HPP
#define JG_CENTRAL_WIDGET_HPP

#include <QtGui/QtGui>

#include "interface.hpp"
#include "canvas.hpp"

namespace jg {

class CentralWidget : public QWidget
{
public:
    CentralWidget(QWidget* parent = 0);

private:
    QHBoxLayout     layout;
    Canvas          canvas;
    Interface       iface;

    void resizeEvent(QResizeEvent* event);
};

} // namespace jg

#endif // JG_CENTRAL_WIDGET_HPP

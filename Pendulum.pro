#-------------------------------------------------
#
# Project created by QtCreator 2015-04-15T00:10:19
#
#-------------------------------------------------

QT       += core gui opengl

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Pendulum
TEMPLATE = app


SOURCES += \
    spinslider.cpp \
    pendulum.cpp \
    main.cpp \
    interface.cpp \
    central_widget.cpp \
    canvas.cpp \
    application.cpp

HEADERS  += \
    spinslider.hpp \
    spawner.hpp \
    queue.hpp \
    pendulum.hpp \
    ode.hpp \
    interface.hpp \
    central_widget.hpp \
    canvas.hpp \
    buffer.hpp \
    application.hpp \
    math/vector4.hpp \
    math/vector3.hpp \
    math/vector2.hpp \
    math/vector.hpp \
    math/tabproxy.hpp \
    math/quaternion.hpp \
    math/matrix4.hpp \
    math/matrix3.hpp \
    math/matrix2.hpp \
    math/matrix.hpp \
    math/matrix_prefix.hpp \
    math/math.hpp

OTHER_FILES +=

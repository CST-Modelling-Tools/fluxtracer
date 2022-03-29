include("../config.pri")

CONFIG += console
CONFIG += concurrent # for multithreading

#DEFINES += USE_AVX # compile vector instructions for 256 bits (4 doubles)
DEFINES += USE_FLOATS # force vector instructions for 128 bits (4 floats)

gcc {
#    QMAKE_CXXFLAGS += -march=native # enable intel avx instructions
    QMAKE_CXXFLAGS += -msse -msse2 -msse3 -mssse3 -msse4 -msse4.1 -msse4.2
#    QMAKE_CXXFLAGS += -mavx -mavx2
    QMAKE_CXXFLAGS_RELEASE -= -O2 # optimisation level
    QMAKE_CXXFLAGS_RELEASE += -O3
}
msvc {
#    QMAKE_CXXFLAGS += /arch:AVX
}

#DESTDIR = ..
#TARGET = FluxTracerKernel.exe

HEADERS += \
    common/qiostream.h \
    common/BinaryStream.h \
    common/InputReader.h \
    functors/AngleWithPlane.h \
    functors/AngularMatrix.h \
    math/math.h \
    math/equations.h \
    math/vec3i.h \
    math/vec3f.h \
    math/vec3d.h \
    math/vec3r.h \
    math/1D/Interval.h \
    math/1D/Grid.h \
    math/1D/BinGrid.h \
    math/2D/Matrix.h \
    math/2D/MatrixGrid.h \
    math/2D/FluxSurface.h \
    math/3D/Tensor3.h \
    math/3D/VoxelGrid.h \
    math/3D/BoundingBox.h \
    math/3D/Ray.h \
    math/3D/Triangle.h \
    math/XD/TensorX.h \
    readers/ParametersReader.h \
    readers/PhotonInfo.h \
    readers/PhotonReader.h \
    readers/RayReader.h \
    readers/FluxTracerReader.h \
    functors/RayFunctor.h \
    functors/SphericalReceiver.h \
    functors/SphericalReceiverAngular.h \
    functors/SphericalReceiverVoxelized.h \
    functors/TubularReceiver.h \
    functors/TubularReceiverAngular.h \
    functors/TubularReceiverVoxelized.h \
    functors/CylindricalReceiver.h \
    functors/VoxelTraversal.h \
    functors/FluxPlane.h \
    functors/FluxCylinder.h \
    server/RayThread.h \
    server/RayServer.h \
    extrusion/ExtrudedReceiver.h \
    extrusion/ExtrusionController.h \
    extrusion/ExtrusionReader.h \
    test.h \
    functors/HeliostatAngles.h

SOURCES += \
    functors/AngleWithPlane.cpp \
    functors/AngularMatrix.cpp \
    main.cpp \
    common/qiostream.cpp \
    common/BinaryStream.cpp \
    common/InputReader.cpp \
    math/math.cpp \
    math/equations.cpp \
    math/vec3i.cpp \
    math/vec3f.cpp \
    math/vec3d.cpp \
    math/vec3r.cpp \
    math/1D/Interval.cpp \
    math/1D/Grid.cpp \
    math/1D/BinGrid.cpp \
    math/2D/MatrixGrid.cpp \
    math/2D/FluxSurface.cpp \
    math/3D/VoxelGrid.cpp \
    math/3D/BoundingBox.cpp \
    math/3D/Ray.cpp \
    math/3D/Triangle.cpp \
    readers/ParametersReader.cpp \
    readers/PhotonInfo.cpp \
    readers/PhotonReader.cpp \
    readers/RayReader.cpp \
    readers/FluxTracerReader.cpp \
    server/RayThread.cpp \
    server/RayServer.cpp \
    functors/RayFunctor.cpp \
    functors/SphericalReceiver.cpp \
    functors/SphericalReceiverAngular.cpp \
    functors/SphericalReceiverVoxelized.cpp \
    functors/TubularReceiver.cpp \
    functors/TubularReceiverAngular.cpp \
    functors/TubularReceiverVoxelized.cpp \
    functors/CylindricalReceiver.cpp \
    functors/VoxelTraversal.cpp \
    functors/FluxPlane.cpp \
    functors/FluxCylinder.cpp \
    extrusion/ExtrudedReceiver.cpp \
    extrusion/ExtrusionController.cpp \
    extrusion/ExtrusionReader.cpp \
    test.cpp \
    functors/HeliostatAngles.cpp

DISTFILES += \
    notes/run.bat \
    notes/input.xml \
    notes/cytera.txt \
    $$DIR_SOLUTION/documentation/readme.txt \
    $$DIR_SOLUTION/visualization/VoxelTraversal.pvsm \
    $$DIR_SOLUTION/visualization/ReceiverRadial.nb \
    $$DIR_SOLUTION/visualization/ReceiverAngular.nb \
    $$DIR_SOLUTION/visualization/CylindricalReceiver.nb

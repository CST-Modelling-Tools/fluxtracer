#include <QCoreApplication>
#include <QCommandLineParser>

#include <QTime>

#include "common/qiostream.h"
#include "test.h"
#include "readers/FluxTracerReader.h"
#include "server/RayServer.h"
#include "extrusion/ExtrusionReader.h"

using namespace cyi;



int main(int argc, char** argv)
{
    // application
    QCoreApplication app(argc, argv);
    QCoreApplication::setApplicationName("FluxTracerKernel");
    QCoreApplication::setApplicationVersion("0.2");
    cout << app.arguments().join(" ") << endl << endl;

    // parser
    QCommandLineParser parser;
    parser.setApplicationDescription("Description of FluxTracerKernel");
    parser.addHelpOption(); // -h
    parser.addVersionOption(); // -v

    // options
    QCommandLineOption optionTest( // -t
        "t",
        "Test mode"
    );
    parser.addOption(optionTest);

    QCommandLineOption optionInput( // -i=input.xml
        "i", // option name
        "File with input parameters", // option description
        "file", // value name
        "input.xml" // value default
    );
    parser.addOption(optionInput);

    QCommandLineOption optionMaximalNodes( // -m=1
        "m", "Maximal nodes",
        "integer", "1"
    );
    parser.addOption(optionMaximalNodes);

    QCommandLineOption optionCurrentNode( // -n=0
        "n", "Current node",
        "integer", "0"
    );
    parser.addOption(optionCurrentNode);

    // processing
    parser.process(app);
    bool isTest = parser.isSet(optionTest);
    QString input = parser.value(optionInput);
    int nodes = parser.value(optionMaximalNodes).toInt();
    int node = parser.value(optionCurrentNode).toInt();

    // calculations
    QTime time;
    time.start();

    try {
        if (isTest) {
            findInertiaTensor();
//            testExtrudedCylinder();
//            testNLOPT();
        } else {
            InputReader root(input);
            QString rootName = root.readRoot();
            if (rootName == "FluxTracer") {
                FluxTracerReader reader(input, nodes, node);
                if (reader.fluxTracerXML().threads == 1) {
                    reader.run();
                } else {
                    RayServer server(&app, &reader);
                    server.run();
                    app.exec();
                }
            } else if (rootName == "ExtrudedReceiver") {
                ExtrusionReader reader(input);
            }
        }
    } catch (const char* msg) {
        cout << msg << endl << endl;
        return EXIT_FAILURE;
    } catch (const QString& msg) {
        cout << msg << endl << endl;
        return EXIT_FAILURE;
    }

    // timing
    int timing = time.elapsed()/1000;
    cout << QString("timing = %1:%2 = %3 m = %4 s\n\n")
            .arg(timing/60)
            .arg(timing%60, 2, 10, QChar('0'))
            .arg(timing/60., 0, 'f', 3)
            .arg(timing);
    return EXIT_SUCCESS;
}

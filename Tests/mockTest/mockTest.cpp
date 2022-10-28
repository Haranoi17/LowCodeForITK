// Qt headers
#include <QGuiApplication>
#include <QGuiApplication>
#include <QtQml>
#include <QQuickStyle>

// QuickQanava headers
#include <QuickQanava.h>
#include <iostream>
#include <itkImage.h>

using namespace qan;

int	main(int argc, char** argv)
{
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    QGuiApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
#endif
    QGuiApplication app(argc, argv);   // Necessary for Qt.labs ColorDialog
    QQuickStyle::setStyle("Material");
    QQmlApplicationEngine engine;
    QuickQanava::initialize(&engine);
    
    auto path = QUrl(QStringLiteral("qrc:/mockTest.qml"));
    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
        &app, [path](QObject* obj, const QUrl& objUrl) {
            if (!obj && path == objUrl)
                QCoreApplication::exit(-1);
        }, Qt::QueuedConnection);
 
    engine.load(path);

    itk::Image<unsigned, 2 >::Pointer image2D = itk::Image<unsigned, 2>::New();
    image2D->GetBufferedRegion();

    return app.exec();
}
//-----------------------------------------------------------------------------

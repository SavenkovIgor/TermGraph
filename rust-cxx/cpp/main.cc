#include <QtGui/QGuiApplication>
#include <QtQml/QQmlApplicationEngine>

int
main(int argc, char* argv[])
{
  QGuiApplication app(argc, argv);

  QQmlApplicationEngine engine;
  engine.loadFromModule("Main", "Main");

  return app.exec();
}

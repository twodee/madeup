#include <QApplication>
#include <QCommandLineParser>

#include "MadeupWindow.h"

int main(int argc, char **argv) {
  QSurfaceFormat format;
  format.setVersion(3, 2);
  format.setRedBufferSize(8);
  format.setGreenBufferSize(8);
  format.setBlueBufferSize(8);
  format.setAlphaBufferSize(8);
  format.setProfile(QSurfaceFormat::CompatibilityProfile);
  QSurfaceFormat::setDefaultFormat(format);

  QApplication app(argc, argv);

  QCommandLineParser parser;
  parser.setApplicationDescription("A language for making things up.");
  parser.addPositionalArgument("source", "Madeup source file.");

  parser.process(app);
  const QStringList args = parser.positionalArguments();

  if (args.size() == 0) {
    MadeupWindow *w = new MadeupWindow();
    w->show();
  } else {
    for (auto i = args.begin(); i != args.end(); ++i) {
      MadeupWindow *w = new MadeupWindow();
      w->open(i->toStdString());
      w->show();
    }
  }

  return app.exec();
}

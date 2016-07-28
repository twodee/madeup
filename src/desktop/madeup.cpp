#include <QApplication>
#include <QCommandLineParser>

#include "MadeupWindow.h"

int main(int argc, char **argv) {
  QApplication app(argc, argv);

  QCommandLineParser parser;
  parser.setApplicationDescription("A language for making things up.");
  parser.addPositionalArgument("source", "Madeup source file.");

  parser.process(app);
  const QStringList args = parser.positionalArguments();

  if (args.size() == 0) {
    MadeupWindow w;
    w.show();
  } else {
    for (auto i = args.begin(); i != args.end(); ++i) {
      MadeupWindow *w = new MadeupWindow();
      w->open(i->toStdString());
      w->show();
    }
  }

  return app.exec();
}

#include <iostream>
#include <sstream>
#include <QColorDialog>

#include "MupperWindow.h"

#include "madeup/ExpressionBlock.h"
#include "madeup/ExpressionClosure.h"
#include "madeup/GeometryMode.h"
#include "madeup/Lexer.h"
#include "madeup/Parser.h"
#include "twodee/Co.h"
#include "twodee/MessagedException.h"
#include "twodee/QVector3.h"

using namespace madeup;

/* ------------------------------------------------------------------------- */

MupperWindow::MupperWindow(QWidget *parent) :
  QMainWindow(parent),
  ui(new Ui::MupperWindow),
  editor(nullptr),
  canvas(nullptr) {
  ui->setupUi(this);

  editor = ui->centralWidget->findChild<QTextEdit *>("editor", Qt::FindChildrenRecursively);
  canvas = ui->centralWidget->findChild<MadeupCanvas *>("canvas", Qt::FindChildrenRecursively);

  editor->setText("moveto 0, 0, 0\n"
                  "repeat 4\n"
                  "  move 10\n"
                  "  yaw 90\n"
                  "end\n"
                  "dowel\n");

  connect(editor, &QTextEdit::textChanged, this, &MupperWindow::onTextChanged);
  connect(ui->actionRun, &QAction::triggered, this, &MupperWindow::onRun);
  connect(ui->actionFit, &QAction::triggered, this, &MupperWindow::onFit);
  connect(ui->actionSetBackgroundColor, &QAction::triggered, this, &MupperWindow::setBackgroundColor);

  QComboBox *settings_picker = ui->centralWidget->findChild<QComboBox *>("settingsPicker", Qt::FindChildrenRecursively);
  settings_picker->addItem("Display");
  settings_picker->addItem("Editor");
  settings_picker->addItem("Lighting");
  settings_picker->addItem("Camera");

  QStackedWidget *settings_pager = ui->centralWidget->findChild<QStackedWidget *>("settingsPager", Qt::FindChildrenRecursively);

  connect(settings_picker, static_cast<void(QComboBox::*)(int)>(&QComboBox::currentIndexChanged), [=](int index) {
    std::cout << "index: " << index << std::endl;
    settings_pager->setCurrentIndex(index);
  });
}

/* ------------------------------------------------------------------------- */

MupperWindow::~MupperWindow() {
  delete ui;
}

/* ------------------------------------------------------------------------- */

void MupperWindow::onRun() {
  std::string source = editor->toPlainText().toStdString();

  std::stringstream in(source);
  Lexer lexer(in);
  try {
    const std::vector<Token> &tokens = lexer.lex();
    Parser parser(tokens, source);
    td::Co<ExpressionBlock> program = parser.program();
    std::cout << program << std::endl;

    GeometryMode::geometry_mode_t geometry_mode = GeometryMode::SURFACE;

    Environment env;
    env.prime();
    env.setGeometryMode(geometry_mode);
    srand(time(NULL));
    rand();

    program->evaluate(env);
    td::Trimesh *trimesh = env.getMesh();

    if (geometry_mode == GeometryMode::PATH) {
      // TODO
    } else if (geometry_mode == GeometryMode::SURFACE) {
      trimesh->DisconnectFaces();

      if (trimesh->GetVertexCount() == 0) {
        std::cerr << "Uh oh. You either didn't visit any locations or didn't invoke a solidifier. I can't make any models without more information from you." << std::endl;
      }

      canvas->setTrimesh(trimesh);
    }

  } catch (std::bad_alloc e) {
    std::cerr << "Whoa! I don't have enough memory for that." << std::endl;
  } catch (td::MessagedException e) {
    std::cerr << e.GetMessage() << std::endl;
  }
}

/* ------------------------------------------------------------------------- */

void MupperWindow::onTextChanged() {
  std::cout << "foo" << std::endl;
}

/* ------------------------------------------------------------------------- */

void MupperWindow::setBackgroundColor() {
  td::QVector4<int> tcolor(canvas->getRenderer()->GetBackgroundColor() * 255.0f);
  QColor qcolor(tcolor[0], tcolor[1], tcolor[2], tcolor[3]);
  QColorDialog *picker = new QColorDialog(qcolor, this); 
  picker->setOption(QColorDialog::ShowAlphaChannel);
  picker->setOption(QColorDialog::NoButtons);
  connect(picker, &QColorDialog::currentColorChanged, [=](const QColor &color) {
    this->canvas->makeCurrent();
    this->canvas->getRenderer()->SetBackgroundColor(td::QVector4<float>(color.red() / 255.0f, color.green() / 255.0f, color.blue() / 255.0f, color.alpha() / 255.0f));
    this->canvas->update();
  });
  picker->setAttribute(Qt::WA_DeleteOnClose);
  picker->show();
}

/* ------------------------------------------------------------------------- */

void MupperWindow::onFit() {
  canvas->getRenderer()->Fit();
  canvas->update();
}

/* ------------------------------------------------------------------------- */


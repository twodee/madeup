#include <iostream>
#include <sstream>
#include <QApplication>
#include <QColorDialog>
#include <QComboBox>
#include <QDoubleSpinBox>
#include <QGroupBox>
#include <QCheckBox>
#include <QFontDialog>
#include <QFormLayout>
#include <QLabel>
#include <QMenuBar>
#include <QHBoxLayout>
#include <QPushButton>
#include <QSplitter>
#include <QStackedWidget>
#include <QStatusBar>
#include <QToolBar>

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
  editor(nullptr),
  canvas(nullptr) {

  resize(1200, 600);

  QSplitter *horizontal_splitter = new QSplitter(this);
  horizontal_splitter->setOrientation(Qt::Horizontal);

  // Left pane
  QSplitter *vertical_splitter = new QSplitter(horizontal_splitter);
  vertical_splitter->setOrientation(Qt::Vertical);

  // - Editor
  editor = new QTextEdit(vertical_splitter);
  QFont font;
  font.setFamily(QStringLiteral("Courier New"));
  editor->setFont(font);
  editor->setLineWrapMode(QTextEdit::NoWrap);

  // - Console
  console = new QTextEdit(vertical_splitter);
  console->setFont(font);

  QSizePolicy p1(QSizePolicy::Preferred, QSizePolicy::Preferred);
  p1.setHorizontalStretch(0);
  p1.setVerticalStretch(2);
  QSizePolicy p2(QSizePolicy::Preferred, QSizePolicy::Preferred);
  p2.setHorizontalStretch(0);
  p2.setVerticalStretch(0);

  editor->setSizePolicy(p1);
  console->setSizePolicy(p2);

  vertical_splitter->addWidget(editor);
  vertical_splitter->addWidget(console);

  // Middle pane
  canvas = new MadeupCanvas(horizontal_splitter);

  // Right pane
  QWidget *settings_panel;
  settings_panel = new QWidget(horizontal_splitter);

  QComboBox *settings_picker = new QComboBox();
  settings_picker->addItem("Display");
  settings_picker->addItem("Editor");
  settings_picker->addItem("Camera");
  settings_picker->addItem("Lighting");

  QStackedWidget *settings_pager = new QStackedWidget();

  // Display page
  QWidget *display_page = new QWidget();

  // Display page widgets
  QGroupBox *cartesian_group = new QGroupBox("Cartesian");

  QLabel *axis_label = new QLabel("Axis");
  QLabel *grid_label = new QLabel("Grid");
  QLabel *size_label = new QLabel("Size");
  QLabel *spacing_label = new QLabel("Spacing");

  QLabel *x_label = new QLabel("X");
  QLabel *y_label = new QLabel("Y");
  QLabel *z_label = new QLabel("Z");

  QCheckBox *x_axis_checkbox = new QCheckBox();
  QCheckBox *y_axis_checkbox = new QCheckBox();
  QCheckBox *z_axis_checkbox = new QCheckBox();

  QCheckBox *x_plane_checkbox = new QCheckBox();
  QCheckBox *y_plane_checkbox = new QCheckBox();
  QCheckBox *z_plane_checkbox = new QCheckBox();

  QDoubleSpinBox *x_size_spinner = new QDoubleSpinBox();
  QDoubleSpinBox *y_size_spinner = new QDoubleSpinBox();
  QDoubleSpinBox *z_size_spinner = new QDoubleSpinBox();

  QDoubleSpinBox *x_space_spinner = new QDoubleSpinBox();
  QDoubleSpinBox *y_space_spinner = new QDoubleSpinBox();
  QDoubleSpinBox *z_space_spinner = new QDoubleSpinBox();

  QGroupBox *path_group = new QGroupBox("Path");
  QCheckBox *show_heading_checkbox = new QCheckBox("Show heading");
  QCheckBox *show_stops_checkbox = new QCheckBox("Show stops");

  QLabel *face_label = new QLabel("Faces");
  QComboBox *face_picker = new QComboBox();
  face_picker->addItem("Clockwise");
  face_picker->addItem("Counterclockwise");
  face_picker->addItem("Both");

  QLabel *style_label = new QLabel("Style");
  QComboBox *style_picker = new QComboBox();
  style_picker->addItem("Solid shaded");
  style_picker->addItem("Wireframe shaded");
  style_picker->addItem("Wireframe unshaded");

  QPushButton *background_color_button = new QPushButton("Set background color");

  QSpacerItem *vertical_spacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

  // Display page layout
  QGridLayout *cartesian_layout = new QGridLayout(cartesian_group);
  cartesian_layout->setHorizontalSpacing(4);
  cartesian_layout->setVerticalSpacing(0);
  cartesian_layout->setContentsMargins(0, 0, 0, 0);

  int row = 0;
  cartesian_layout->addWidget(axis_label, row, 1, Qt::AlignHCenter | Qt::AlignBottom);
  cartesian_layout->addWidget(grid_label, row, 2, Qt::AlignHCenter | Qt::AlignBottom);
  cartesian_layout->addWidget(size_label, row, 3, Qt::AlignBottom);
  cartesian_layout->addWidget(spacing_label, row, 4, Qt::AlignBottom);
  ++row;

  cartesian_layout->addWidget(x_label, row, 0);
  cartesian_layout->addWidget(x_axis_checkbox, row, 1, Qt::AlignCenter);
  cartesian_layout->addWidget(x_plane_checkbox, row, 2, Qt::AlignCenter);
  cartesian_layout->addWidget(x_size_spinner, row, 3);
  cartesian_layout->addWidget(x_space_spinner, row, 4);
  ++row;

  cartesian_layout->addWidget(y_label, row, 0);
  cartesian_layout->addWidget(y_axis_checkbox, row, 1, Qt::AlignCenter);
  cartesian_layout->addWidget(y_plane_checkbox, row, 2, Qt::AlignCenter);
  cartesian_layout->addWidget(y_size_spinner, row, 3);
  cartesian_layout->addWidget(y_space_spinner, row, 4);
  ++row;

  cartesian_layout->addWidget(z_label, row, 0);
  cartesian_layout->addWidget(z_axis_checkbox, row, 1, Qt::AlignCenter);
  cartesian_layout->addWidget(z_plane_checkbox, row, 2, Qt::AlignCenter);
  cartesian_layout->addWidget(z_size_spinner, row, 3);
  cartesian_layout->addWidget(z_space_spinner, row, 4);
  ++row;

  cartesian_layout->setColumnStretch(0, 0);
  cartesian_layout->setColumnStretch(1, 0);
  cartesian_layout->setColumnStretch(2, 0);
  cartesian_layout->setColumnStretch(3, 1);
  cartesian_layout->setColumnStretch(4, 1);

  QVBoxLayout *path_group_layout = new QVBoxLayout(path_group);
  path_group_layout->setSpacing(-1);
  path_group_layout->setContentsMargins(0, 0, 0, 0);
  path_group_layout->addWidget(show_heading_checkbox);
  path_group_layout->addWidget(show_stops_checkbox);

  QGridLayout *display_page_layout = new QGridLayout(display_page);
  display_page_layout->setSpacing(-1);
  display_page_layout->setContentsMargins(0, 0, 0, 0);
  display_page_layout->addWidget(cartesian_group, 0, 0, 1, 2);
  display_page_layout->addWidget(path_group, 1, 0, 1, 2);
  display_page_layout->addWidget(face_label, 2, 0, 1, 1);
  display_page_layout->addWidget(face_picker, 2, 1, 1, 1);
  display_page_layout->addWidget(style_label, 3, 0, 1, 1);
  display_page_layout->addWidget(style_picker, 3, 1, 1, 1);
  display_page_layout->addWidget(background_color_button, 4, 0, 1, 2);
  display_page_layout->addItem(vertical_spacer, 5, 0, 1, 2);
 
  // Editor page
  QWidget *editor_page = new QWidget();

  QCheckBox *autopathify_checkbox = new QCheckBox("Autopathify");
  QCheckBox *show_console_checkbox = new QCheckBox("Show console");
  show_console_checkbox->setCheckState(Qt::Checked);
  QDoubleSpinBox *autopathify_delay_spinner = new QDoubleSpinBox();
  QPushButton *select_font_button = new QPushButton("Select Font");

  QSpacerItem *vertical_spacer3 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

  QGridLayout *editor_page_layout = new QGridLayout(editor_page);
  editor_page_layout->setSpacing(-1);
  editor_page_layout->setContentsMargins(0, 0, 0, 0);
  editor_page_layout->addWidget(autopathify_checkbox, 0, 0, 1, 2);

  editor_page_layout->addWidget(new QLabel("Autopathify delay"), 1, 0);
  editor_page_layout->addWidget(autopathify_delay_spinner, 1, 1);

  editor_page_layout->addWidget(show_console_checkbox, 2, 0, 1, 2);
  
  editor_page_layout->addWidget(select_font_button, 3, 0, 1, 2);

  editor_page_layout->addItem(vertical_spacer3, 4, 0, 1, 2);

  editor_page_layout->setColumnStretch(0, 0);
  editor_page_layout->setColumnStretch(1, 1);
 
  // Camera page
  QWidget *camera_page = new QWidget();

  QGroupBox *view_from_group = new QGroupBox("View From");
  QPushButton *negative_x_button = new QPushButton("-X");
  QPushButton *positive_x_button = new QPushButton("+X");
  QPushButton *negative_y_button = new QPushButton("-Y");
  QPushButton *positive_y_button = new QPushButton("+Y");
  QPushButton *negative_z_button = new QPushButton("-Z");
  QPushButton *positive_z_button = new QPushButton("+Z");

  QGridLayout *view_from_group_layout = new QGridLayout(view_from_group);
  view_from_group_layout->setSpacing(-1);
  view_from_group_layout->setContentsMargins(0, 0, 0, 0);
  view_from_group_layout->addWidget(negative_x_button, 0, 0);
  view_from_group_layout->addWidget(positive_x_button, 0, 1);
  view_from_group_layout->addWidget(negative_y_button, 1, 0);
  view_from_group_layout->addWidget(positive_y_button, 1, 1);
  view_from_group_layout->addWidget(negative_z_button, 2, 0);
  view_from_group_layout->addWidget(positive_z_button, 2, 1);

  QVBoxLayout *camera_page_layout = new QVBoxLayout(camera_page);
  camera_page_layout->setSpacing(-1);
  camera_page_layout->setContentsMargins(0, 0, 0, 0);
  camera_page_layout->addWidget(view_from_group);
  QSpacerItem *vertical_spacer2 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);
  camera_page_layout->addItem(vertical_spacer2);

  // Add pages
  settings_pager->addWidget(display_page);
  settings_pager->addWidget(editor_page);
  settings_pager->addWidget(camera_page);

  // Layout
  QSizePolicy horizontal_stretch(QSizePolicy::Preferred, QSizePolicy::Preferred);
  horizontal_stretch.setHorizontalStretch(1);
  horizontal_stretch.setVerticalStretch(0);
  canvas->setSizePolicy(horizontal_stretch);

  QVBoxLayout *settings_panel_layout = new QVBoxLayout(settings_panel);
  settings_panel_layout->setSpacing(6);
  settings_panel_layout->setContentsMargins(0, 6, 0, 0);
  settings_panel_layout->addWidget(settings_picker);
  settings_panel_layout->addWidget(settings_pager);

  horizontal_splitter->addWidget(vertical_splitter);
  horizontal_splitter->addWidget(canvas);
  horizontal_splitter->addWidget(settings_panel);

  // Overall
  this->setCentralWidget(horizontal_splitter);

  // Actions
  QAction *action_run = new QAction(this);
  action_run->setText("Run");
  action_run->setShortcut(Qt::CTRL + Qt::Key_R);

  QAction *action_settings = new QAction(this);
  action_settings->setText("Show settings");
  action_settings->setShortcut(Qt::CTRL + Qt::Key_P);
  action_settings->setCheckable(true);

  QAction *action_fit = new QAction(this);
  action_fit->setText("Fit");
  action_fit->setShortcut(Qt::CTRL + Qt::Key_F);

  // Toolbar
  QToolBar *toolbar = new QToolBar(this);
  toolbar->setMovable(false);
  addToolBar(Qt::TopToolBarArea, toolbar);
  toolbar->addAction(action_run);
  toolbar->addAction(action_fit);

  // Status bar
  QStatusBar *statusBar = new QStatusBar(this);
  setStatusBar(statusBar);

  // Menubar
  QMenuBar *menuBar = new QMenuBar();
  setMenuBar(menuBar);

  QMenu *menuFile = new QMenu(menuBar);
  menuFile->setTitle("File");
  menuFile->addAction(new QAction("&Open", this));
  menuFile->addAction(new QAction("&Save", this));

  QMenu *menuView = new QMenu(menuBar);
  menuView->setTitle("View");
  menuView->addAction(action_settings);

  setWindowTitle("Madeup");
  menuFile->setTitle("File");
  menuView->setTitle("View");
  menuBar->addAction(menuFile->menuAction());
  menuBar->addAction(menuView->menuAction());

  // Events
  connect(settings_picker, static_cast<void(QComboBox::*)(int)>(&QComboBox::currentIndexChanged), settings_pager, &QStackedWidget::setCurrentIndex);
  connect(action_run, &QAction::triggered, this, &MupperWindow::onRun);
  connect(action_fit, &QAction::triggered, this, &MupperWindow::onFit);
  connect(background_color_button, &QPushButton::clicked, this, &MupperWindow::selectBackgroundColor);
  connect(select_font_button, &QPushButton::clicked, this, &MupperWindow::selectFont);
  connect(editor, &QTextEdit::textChanged, this, &MupperWindow::onTextChanged);
  connect(action_settings, &QAction::triggered, [=](bool is_checked) {
    settings_panel->setVisible(is_checked);
  });
  connect(show_console_checkbox, &QCheckBox::stateChanged, [=](int state) {
    console->setVisible(state == Qt::Checked);
  });

  // Tweaks
  editor->setText("moveto 0, 0, 0\n"
                  "repeat 4\n"
                  "  move 10\n"
                  "  yaw 90\n"
                  "end\n"
                  "dowel\n");
  /* settings_panel->setVisible(false); */
}

/* ------------------------------------------------------------------------- */

MupperWindow::~MupperWindow() {
}

/* ------------------------------------------------------------------------- */

void MupperWindow::onRun() {
  std::string source = editor->toPlainText().toStdString();

  td::Trimesh *trimesh = NULL;
  std::stringstream cout_capturer;
  std::streambuf *old_cout_buffer = std::cout.rdbuf();
  std::cout.rdbuf(cout_capturer.rdbuf());

  std::stringstream in(source);
  Lexer lexer(in);
  try {
    const std::vector<Token> &tokens = lexer.lex();
    Parser parser(tokens, source);

    td::Co<ExpressionBlock> program = parser.program();

    GeometryMode::geometry_mode_t geometry_mode = GeometryMode::SURFACE;

    Environment env;
    env.prime();
    env.setGeometryMode(geometry_mode);
    srand(time(NULL));
    rand();

    program->evaluate(env);
    trimesh = env.getMesh();

    if (geometry_mode == GeometryMode::PATH) {
      // TODO
    } else if (geometry_mode == GeometryMode::SURFACE) {
      trimesh->DisconnectFaces();

      if (trimesh->GetVertexCount() == 0) {
        std::cout << "Uh oh. You either didn't visit any locations or didn't invoke a solidifier. I can't make any models without more information from you." << std::endl;
      }
    }

  } catch (std::bad_alloc e) {
    std::cout << "Whoa! I don't have enough memory for that." << std::endl;
  } catch (td::MessagedException e) {
    std::cout << e.GetMessage() << std::endl;
  }

  std::cout.rdbuf(old_cout_buffer);
  console->setText(cout_capturer.str().c_str());
  if (!trimesh) {
    trimesh = new td::Trimesh(0, 0);
  }
  canvas->setTrimesh(trimesh);
}

/* ------------------------------------------------------------------------- */

void MupperWindow::onTextChanged() {
}

/* ------------------------------------------------------------------------- */

void MupperWindow::selectBackgroundColor() {
  td::QVector4<int> tcolor(canvas->getRenderer()->GetBackgroundColor() * 255.0f);
  QColor qcolor(tcolor[0], tcolor[1], tcolor[2], tcolor[3]);
  QColorDialog *picker = new QColorDialog(qcolor, this); 
  picker->setOption(QColorDialog::ShowAlphaChannel);
  picker->setOption(QColorDialog::NoButtons);
  connect(picker, &QColorDialog::currentColorChanged, [=](const QColor &color) {
    this->canvas->makeCurrent(); // no effect without this
    this->canvas->getRenderer()->SetBackgroundColor(td::QVector4<float>(color.red() / 255.0f, color.green() / 255.0f, color.blue() / 255.0f, color.alpha() / 255.0f));
    this->canvas->update();
  });
  picker->setAttribute(Qt::WA_DeleteOnClose);
  picker->show();
}

/* ------------------------------------------------------------------------- */

void MupperWindow::selectFont() {
  QFontDialog *picker = new QFontDialog(editor->currentFont(), this); 
  picker->setOption(QFontDialog::NoButtons);
  connect(picker, &QFontDialog::currentFontChanged, [=](const QFont &font) {
    editor->setFont(font);
    console->setFont(font);
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


#include <iostream>
#include <sstream>
#include <regex>

#include <QApplication>
#include <QColorDialog>
#include <QComboBox>
#include <QDesktopServices>
#include <QDoubleSpinBox>
#include <QGroupBox>
#include <QFileDialog>
#include <QFileInfo>
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

#include "MadeupWindow.h"

#include "json/json.h"
#include "madeup/ExpressionBlock.h"
#include "madeup/ExpressionClosure.h"
#include "madeup/GeometryMode.h"
#include "madeup/Lexer.h"
#include "madeup/MeshBoolean.h"
#include "madeup/Parser.h"
#include "twodee/Co.h"
#include "twodee/MessagedException.h"
#include "twodee/QVector3.h"

using namespace madeup;

/* ------------------------------------------------------------------------- */

MadeupWindow::MadeupWindow(QWidget *parent) :
  QMainWindow(parent),
  autopathify_timer(new QTimer(this)),
  editor(nullptr),
  canvas(nullptr),
  last_directory(QDir::homePath()) {

  autopathify_timer->setSingleShot(true);
  config_path = (QStandardPaths::writableLocation(QStandardPaths::ConfigLocation) + QDir::separator() + "org.twodee.madeup.json").toStdString();
  std::cout << "config_path: " << config_path << std::endl;

  horizontal_splitter = new QSplitter(this);
  horizontal_splitter->setOrientation(Qt::Horizontal);

  // Left pane
  vertical_splitter = new QSplitter(horizontal_splitter);
  vertical_splitter->setOrientation(Qt::Vertical);

  // - Editor
  editor = new QPlainTextEdit(vertical_splitter);
  QFont font;
  font.setFamily(QStringLiteral("Courier New"));
  font.setPointSize(18);
  editor->setFont(font);
  editor->setLineWrapMode(QPlainTextEdit::NoWrap);

  // - Console
  console = new QTextBrowser(vertical_splitter);
  console->setFont(font);
  /* console->setText("<a href=\"jump?start=3&end=7\">click here</a>"); */
  console->setOpenLinks(false);

  {
    QPalette palette;
    palette.setColor(QPalette::Base, Qt::black);
    palette.setColor(QPalette::Text, Qt::white);
    editor->setPalette(palette);
    console->setPalette(palette);
  }

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
  renderer = &canvas->getRenderer();

  // Right pane
  QWidget *settings_panel;
  settings_panel = new QWidget(horizontal_splitter);
  settings_panel->setVisible(false);

  settings_picker = new QComboBox();
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
  QLabel *size_label = new QLabel("Extent");
  QLabel *spacing_label = new QLabel("Spacing");

  QLabel *x_label = new QLabel("X");
  QLabel *y_label = new QLabel("Y");
  QLabel *z_label = new QLabel("Z");

  for (int d = 0; d < 3; ++d) {
    show_axis_checkboxes[d] = new QCheckBox();
    show_grid_checkboxes[d] = new QCheckBox();
    grid_extent_spinners[d] = new QDoubleSpinBox();
    grid_extent_spinners[d]->setMinimum(0.01);
    grid_spacing_spinners[d] = new QDoubleSpinBox();
    grid_spacing_spinners[d]->setMinimum(0.01);
  }

  axis_stroke_width_spinner = new QDoubleSpinBox();  
  axis_stroke_width_spinner->setMinimum(1.0);

  grid_stroke_width_spinner = new QDoubleSpinBox();  
  grid_stroke_width_spinner->setMinimum(1.0);

  QGroupBox *path_group = new QGroupBox("Path");
  show_heading_checkbox = new QCheckBox("Show heading");
  show_stops_checkbox = new QCheckBox("Show stops");

  QLabel *path_stroke_width_label = new QLabel("Thickness");
  path_stroke_width_spinner = new QDoubleSpinBox();
  path_stroke_width_spinner->setMinimum(1.0);

  QLabel *vertex_size_label = new QLabel("Vertex size");
  vertex_size_spinner = new QDoubleSpinBox();
  vertex_size_spinner->setMinimum(1.0);

  QLabel *path_color_label = new QLabel("Path color");
  path_color_button = new QPushButton();
  path_color_button->setFlat(true);
  path_color_button->setAutoFillBackground(true);

  QLabel *vertex_color_label = new QLabel("Vertex color");
  vertex_color_button = new QPushButton();
  vertex_color_button->setFlat(true);
  vertex_color_button->setAutoFillBackground(true);

  QLabel *style_label = new QLabel("Render style");
  face_style_picker = new QComboBox();
  face_style_picker->addItem("Filled");
  face_style_picker->addItem("Wireframe");
  face_style_picker->addItem("Vertices");

  QLabel *background_color_label = new QLabel("Background color");
  background_color_button = new QPushButton();
  background_color_button->setFlat(true);
  background_color_button->setAutoFillBackground(true);

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
  cartesian_layout->addWidget(show_axis_checkboxes[0], row, 1, Qt::AlignCenter);
  cartesian_layout->addWidget(show_grid_checkboxes[0], row, 2, Qt::AlignCenter);
  cartesian_layout->addWidget(grid_extent_spinners[0], row, 3);
  cartesian_layout->addWidget(grid_spacing_spinners[0], row, 4);
  ++row;

  cartesian_layout->addWidget(y_label, row, 0);
  cartesian_layout->addWidget(show_axis_checkboxes[1], row, 1, Qt::AlignCenter);
  cartesian_layout->addWidget(show_grid_checkboxes[1], row, 2, Qt::AlignCenter);
  cartesian_layout->addWidget(grid_extent_spinners[1], row, 3);
  cartesian_layout->addWidget(grid_spacing_spinners[1], row, 4);
  ++row;

  cartesian_layout->addWidget(z_label, row, 0);
  cartesian_layout->addWidget(show_axis_checkboxes[2], row, 1, Qt::AlignCenter);
  cartesian_layout->addWidget(show_grid_checkboxes[2], row, 2, Qt::AlignCenter);
  cartesian_layout->addWidget(grid_extent_spinners[2], row, 3);
  cartesian_layout->addWidget(grid_spacing_spinners[2], row, 4);
  ++row;

  cartesian_layout->addWidget(new QLabel("Axis thickness"), row, 0, 1, 3);
  cartesian_layout->addWidget(axis_stroke_width_spinner, row, 3, 1, 2);
  ++row;

  cartesian_layout->addWidget(new QLabel("Grid thickness"), row, 0, 1, 3);
  cartesian_layout->addWidget(grid_stroke_width_spinner, row, 3, 1, 2);
  ++row;

  cartesian_layout->setColumnStretch(0, 0);
  cartesian_layout->setColumnStretch(1, 0);
  cartesian_layout->setColumnStretch(2, 0);
  cartesian_layout->setColumnStretch(3, 1);
  cartesian_layout->setColumnStretch(4, 1);

  QGridLayout *path_group_layout = new QGridLayout(path_group);
  path_group_layout->setHorizontalSpacing(3);
  path_group_layout->setVerticalSpacing(5);
  path_group_layout->setContentsMargins(0, 0, 0, 0);
  path_group_layout->addWidget(show_heading_checkbox, 0, 0, 1, 2);
  path_group_layout->addWidget(show_stops_checkbox, 1, 0, 1, 2);
  path_group_layout->addWidget(path_stroke_width_label, 2, 0, 1, 1);
  path_group_layout->addWidget(path_stroke_width_spinner, 2, 1, 1, 1);
  path_group_layout->addWidget(vertex_size_label, 3, 0, 1, 1);
  path_group_layout->addWidget(vertex_size_spinner, 3, 1, 1, 1);
  path_group_layout->addWidget(path_color_label, 4, 0, 1, 1);
  path_group_layout->addWidget(path_color_button, 4, 1, 1, 1);
  path_group_layout->addWidget(vertex_color_label, 5, 0, 1, 1);
  path_group_layout->addWidget(vertex_color_button, 5, 1, 1, 1);
  path_group_layout->setColumnStretch(0, 0);
  path_group_layout->setColumnStretch(1, 1);

  QGridLayout *display_page_layout = new QGridLayout(display_page);
  display_page_layout->setSpacing(-1);
  display_page_layout->setContentsMargins(0, 0, 0, 0);
  display_page_layout->addWidget(cartesian_group, 0, 0, 1, 2);
  display_page_layout->addWidget(path_group, 1, 0, 1, 2);
  display_page_layout->addWidget(style_label, 2, 0, 1, 1);
  display_page_layout->addWidget(face_style_picker, 2, 1, 1, 1);
  display_page_layout->addWidget(background_color_label, 3, 0, 1, 1);
  display_page_layout->addWidget(background_color_button, 3, 1, 1, 1);
  display_page_layout->addItem(vertical_spacer, 4, 0, 1, 2);

  // Editor page
  QWidget *editor_page = new QWidget();

  autopathify_checkbox = new QCheckBox("Autopathify");
  show_console_checkbox = new QCheckBox("Show console");

  autopathify_delay_spinner = new QDoubleSpinBox();
  autopathify_delay_spinner->setMinimum(0.0);
  autopathify_delay_spinner->setMaximum(10.0);
  autopathify_delay_spinner->setSingleStep(0.25);

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

  has_autorotate_checkbox = new QCheckBox("Autorotate");

  QVBoxLayout *camera_page_layout = new QVBoxLayout(camera_page);
  camera_page_layout->setSpacing(-1);
  camera_page_layout->setContentsMargins(0, 0, 0, 0);
  camera_page_layout->addWidget(view_from_group);
  camera_page_layout->addWidget(has_autorotate_checkbox);
  QSpacerItem *vertical_spacer2 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);
  camera_page_layout->addItem(vertical_spacer2);

  // Lighting page
  QWidget *lighting_page = new QWidget();

  azimuth_angle_spinner = new QDoubleSpinBox();
  azimuth_angle_spinner->setMinimum(0.0);
  azimuth_angle_spinner->setMaximum(360.0);
  azimuth_angle_spinner->setSingleStep(5.0);

  elevation_angle_spinner = new QDoubleSpinBox();
  elevation_angle_spinner->setMinimum(-90.0);
  elevation_angle_spinner->setMaximum(90.0);
  elevation_angle_spinner->setSingleStep(5.0);

  shininess_spinner = new QDoubleSpinBox();
  shininess_spinner->setMinimum(0.1);
  shininess_spinner->setMaximum(1000.0);
  shininess_spinner->setSingleStep(10.0);

  light_distance_factor_spinner = new QDoubleSpinBox();
  light_distance_factor_spinner->setMinimum(0.0);
  light_distance_factor_spinner->setMaximum(100.0);
  light_distance_factor_spinner->setSingleStep(0.1);

  has_specular_checkbox = new QCheckBox("Show specular highlights");
  has_specular_checkbox->setChecked(true);
  faceted_checkbox = new QCheckBox("Faceted");
  is_two_sided_checkbox = new QCheckBox("Two-sided");

  QGridLayout *lighting_page_layout = new QGridLayout(lighting_page);
  lighting_page_layout->setSpacing(-1);
  lighting_page_layout->setContentsMargins(0, 0, 0, 0);

  lighting_page_layout->addWidget(new QLabel("Azimuth angle"), 0, 0);
  lighting_page_layout->addWidget(azimuth_angle_spinner, 0, 1);

  lighting_page_layout->addWidget(new QLabel("Elevation angle"), 1, 0);
  lighting_page_layout->addWidget(elevation_angle_spinner, 1, 1);

  lighting_page_layout->addWidget(new QLabel("Shininess"), 2, 0);
  lighting_page_layout->addWidget(shininess_spinner, 2, 1);

  lighting_page_layout->addWidget(new QLabel("Distance factor"), 3, 0);
  lighting_page_layout->addWidget(light_distance_factor_spinner, 3, 1);

  lighting_page_layout->addWidget(has_specular_checkbox, 4, 0, 1, 2);

  lighting_page_layout->addWidget(faceted_checkbox, 5, 0, 1, 2);

  lighting_page_layout->addWidget(is_two_sided_checkbox, 6, 0, 1, 2);

  QSpacerItem *vertical_spacer4 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);
  lighting_page_layout->addItem(vertical_spacer4, 7, 0, 1, 2);

  lighting_page_layout->setColumnStretch(0, 0);
  lighting_page_layout->setColumnStretch(1, 1);

  // Add pages
  settings_pager->addWidget(display_page);
  settings_pager->addWidget(editor_page);
  settings_pager->addWidget(camera_page);
  settings_pager->addWidget(lighting_page);

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
  QAction *action_solidify = new QAction(this);
  action_solidify->setText("Solidify");
  action_solidify->setShortcut(Qt::CTRL + Qt::Key_R);

  QAction *action_pathify = new QAction(this);
  action_pathify->setText("Pathify");
  action_pathify->setShortcut(Qt::CTRL + Qt::Key_E);

  QAction *action_text_bigger = new QAction(this);
  action_text_bigger->setText("Increase font size");
  action_text_bigger->setShortcut(Qt::CTRL + Qt::Key_Plus);

  QAction *action_text_smaller = new QAction(this);
  action_text_smaller->setText("Decrease font size");
  action_text_smaller->setShortcut(Qt::CTRL + Qt::Key_Minus);

  action_settings = new QAction(this);
  action_settings->setText("Show settings");
  action_settings->setShortcut(Qt::CTRL + Qt::Key_P);
  action_settings->setCheckable(true);

  QAction *action_documentation = new QAction(this);
  action_documentation->setText("Documentation");

  QAction *action_fit = new QAction(this);
  action_fit->setText("Fit");
  action_fit->setShortcut(Qt::CTRL + Qt::Key_F);

  QAction *action_screenshot = new QAction(this);
  action_screenshot->setText("Take screenshot");
  action_screenshot->setShortcut(Qt::CTRL + Qt::Key_T);

  QAction *action_new = new QAction(this);
  action_new->setText("New window");
  action_new->setShortcut(Qt::CTRL + Qt::Key_N);

  QAction *action_open = new QAction(this);
  action_open->setText("Open");
  action_open->setShortcut(Qt::CTRL + Qt::Key_O);

  QAction *action_save = new QAction(this);
  action_save->setText("Save");
  action_save->setShortcut(Qt::CTRL + Qt::Key_S);

  QAction *action_save_as = new QAction(this);
  action_save_as->setText("Save as");
  action_save_as->setShortcut(Qt::SHIFT + Qt::CTRL + Qt::Key_S);

  QAction *action_close = new QAction(this);
  action_close->setText("Close");
  action_close->setShortcut(Qt::CTRL + Qt::Key_W);

  QAction *action_export = new QAction(this);
  action_export->setText("Export OBJ");

  // Toolbar
  QToolBar *toolbar = new QToolBar(this);
  toolbar->setMovable(false);
  addToolBar(Qt::TopToolBarArea, toolbar);
  toolbar->addAction(action_solidify);
  toolbar->addAction(action_pathify);
  toolbar->addAction(action_fit);

  // Status bar
  QStatusBar *statusBar = new QStatusBar(this);
  setStatusBar(statusBar);

  // Menubar
  QMenuBar *menuBar = new QMenuBar();
  setMenuBar(menuBar);

  QMenu *menuFile = new QMenu(menuBar);
  menuFile->setTitle("File");
  menuFile->addAction(action_open);
  menuFile->addAction(action_save);
  menuFile->addAction(action_save_as);
  menuFile->addAction(action_close);
  menuFile->addAction(action_new);
  menuFile->addSeparator();
  menuFile->addAction(action_export);

  QMenu *menuView = new QMenu(menuBar);
  menuView->setTitle("View");
  menuView->addAction(action_settings);
  menuView->addAction(action_screenshot);
  menuView->addAction(action_text_bigger);
  menuView->addAction(action_text_smaller);

  QMenu *menuHelp = new QMenu(menuBar);
  menuHelp->setTitle("Help");
  menuHelp->addAction(action_documentation);

  setWindowTitle("Madeup");
  menuBar->addAction(menuFile->menuAction());
  menuBar->addAction(menuView->menuAction());
  menuBar->addAction(menuHelp->menuAction());

  // Events
  connect(settings_picker, static_cast<void(QComboBox::*)(int)>(&QComboBox::currentIndexChanged), settings_pager, &QStackedWidget::setCurrentIndex);
  connect(action_solidify, &QAction::triggered, this, &MadeupWindow::onSolidify);
  connect(action_pathify, &QAction::triggered, this, &MadeupWindow::onPathify);
  connect(action_fit, &QAction::triggered, this, &MadeupWindow::onFit);
  connect(select_font_button, &QPushButton::clicked, this, &MadeupWindow::selectFont);
  connect(editor, &QPlainTextEdit::textChanged, this, &MadeupWindow::onTextChanged);

  connect(console, &QTextBrowser::anchorClicked, [=](const QUrl &link) {
    string command = link.toString().toStdString();

    std::smatch groups;
    if (std::regex_match(command, groups, std::regex("jump\\?start=(\\d+)&end=(\\d+)"))) {
      auto group = groups.begin();
      ++group;

      int a = td::Utilities::ToInt(*group);
      ++group;

      int b = td::Utilities::ToInt(*group);
      ++group;

      QTextCursor cursor(editor->document());
      cursor.setPosition(a, QTextCursor::MoveAnchor);
      cursor.setPosition(b + 1, QTextCursor::KeepAnchor);
      editor->setTextCursor(cursor);
    }
  });

  connect(negative_x_button, &QPushButton::clicked, [=]() {
    canvas->makeCurrent();
    renderer->viewFromAxis(0, -1);
    canvas->update();
  });

  connect(positive_x_button, &QPushButton::clicked, [=]() {
    canvas->makeCurrent();
    renderer->viewFromAxis(0, 1);
    canvas->update();
  });

  connect(negative_y_button, &QPushButton::clicked, [=]() {
    canvas->makeCurrent();
    renderer->viewFromAxis(1, -1);
    canvas->update();
  });

  connect(positive_y_button, &QPushButton::clicked, [=]() {
    canvas->makeCurrent();
    renderer->viewFromAxis(1, 1);
    canvas->update();
  });

  connect(negative_z_button, &QPushButton::clicked, [=]() {
    canvas->makeCurrent();
    renderer->viewFromAxis(2, -1);
    canvas->update();
  });

  connect(positive_z_button, &QPushButton::clicked, [=]() {
    canvas->makeCurrent();
    renderer->viewFromAxis(2, 1);
    canvas->update();
  });

  connect(action_text_bigger, &QAction::triggered, [=]() {
    QFont font = editor->font();
    font.setPointSize(font.pointSize() + 2);
    editor->setFont(font);
  });

  connect(action_text_smaller, &QAction::triggered, [=]() {
    QFont font = editor->font();
    font.setPointSize(font.pointSize() - 2);
    editor->setFont(font);
  });

  connect(autopathify_timer, &QTimer::timeout, [=]() {
    onPathify();
  });

  connect(face_style_picker, static_cast<void(QComboBox::*)(int)>(&QComboBox::currentIndexChanged), [=](int i) {
    canvas->makeCurrent(); // no effect without this
    renderer->setRenderStyle(i);
    canvas->update();
  });

  connect(action_documentation, &QAction::triggered, [=]() {
    QDesktopServices::openUrl(QUrl("http://madeup.xyz/docs/introduction.html"));
  });

  connect(action_screenshot, &QAction::triggered, this, [=]() {
    canvas->makeCurrent();
    QString path = QFileDialog::getSaveFileName(this, "Save Screenshot", last_directory, "Images (*.png *.jpg *.gif)");
    if (path.length() != 0) {
      last_directory = QFileInfo(path).absolutePath();
      canvas->makeCurrent();
      renderer->takeScreenshot(path.toStdString());
    }
  });

  connect(action_open, &QAction::triggered, [=]() {
    QString path = QFileDialog::getOpenFileName(this, "Open File", last_directory, "Madeup Programs (*.mup)");
    if (path.length() != 0) {
      last_directory = QFileInfo(path).absolutePath();
      std::cout << "last_directory: " << last_directory.toStdString() << std::endl;
      open(path.toStdString());
    }
  });

  connect(action_close, &QAction::triggered, [=]() {
    close();
  });

  connect(action_save, &QAction::triggered, [=]() {
    if (mup_path.length() != 0) {
      saveAs(mup_path); 
    } else {
      QString path = QFileDialog::getSaveFileName(this, "Save File", last_directory, "Madeup Programs (*.mup)");
      if (path.length() != 0) {
        last_directory = QFileInfo(path).absolutePath();
        saveAs(path.toStdString()); 
      }
    }
  });

  connect(action_save_as, &QAction::triggered, [=]() {
    QString path = QFileDialog::getSaveFileName(this, "Save File", last_directory, "Madeup Programs (*.mup)");
    if (path.length() != 0) {
      last_directory = QFileInfo(path).absolutePath();
      saveAs(path.toStdString());
    }
  });

  connect(action_new, &QAction::triggered, [=]() {
    MadeupWindow *new_window = new MadeupWindow();
    // This window has no parent, so who's gonna delete it? Itself.
    new_window->setAttribute(Qt::WA_DeleteOnClose);
    new_window->show();
  });

  connect(action_export, &QAction::triggered, [=]() {
    QString path = QFileDialog::getSaveFileName(this, "Export File", last_directory, "Wavefront OBJ Files (*.obj)");
    if (path.length() != 0) {
      last_directory = QFileInfo(path).absolutePath();
      renderer->exportTrimesh(path.toStdString());
    }
  });

  connect(background_color_button, &QPushButton::clicked, [=](){
    selectColor(renderer->getBackgroundColor(), [=](const QColor &color) {
      canvas->makeCurrent(); // no effect without this
      renderer->setBackgroundColor(td::QVector4<float>(color.red() / 255.0f, color.green() / 255.0f, color.blue() / 255.0f, color.alpha() / 255.0f));
      canvas->update();
      QPalette palette;
      palette.setColor(QPalette::Button, color);
      background_color_button->setPalette(palette);
    });
  });

  connect(path_color_button, &QPushButton::clicked, [=](){
    selectColor(renderer->getPathColor(), [=](const QColor &color) {
      this->canvas->makeCurrent(); // no effect without this
      this->renderer->setPathColor(td::QVector4<float>(color.red() / 255.0f, color.green() / 255.0f, color.blue() / 255.0f, color.alpha() / 255.0f));
      this->canvas->update();
      QPalette palette;
      palette.setColor(QPalette::Button, color);
      path_color_button->setPalette(palette);
    });
  });

  connect(vertex_color_button, &QPushButton::clicked, [=](){
    selectColor(renderer->getVertexColor(), [=](const QColor &color) {
      this->canvas->makeCurrent(); // no effect without this
      this->renderer->setVertexColor(td::QVector4<float>(color.red() / 255.0f, color.green() / 255.0f, color.blue() / 255.0f, color.alpha() / 255.0f));
      this->canvas->update();
      QPalette palette;
      palette.setColor(QPalette::Button, color);
      vertex_color_button->setPalette(palette);
    });
  });

  connect(action_settings, &QAction::toggled, [=](bool is_checked) {
    settings_panel->setVisible(is_checked);
      
    // If a collapsed settings panel is being made visible again, we'll have to
    // make it bigger. Otherwise only the handle will appear.
    if (is_checked && settings_panel->width() == 0) {
      // Let's resize the views. The editor and canvas can retain their current
      // sizes. The panel can be made its minimum size. We guess 1; Qt will take
      // max(1, minimum size).
      QList<int> sizes = horizontal_splitter->sizes();
      sizes[2] = 1;
      horizontal_splitter->setSizes(sizes);
    }
  });

  connect(show_console_checkbox, &QCheckBox::toggled, [=](bool is_checked) {
    console->setVisible(is_checked);
  });

  connect(faceted_checkbox, &QCheckBox::toggled, [=](bool is_checked) {
    onRun(GeometryMode::SURFACE);
  });

  connect(is_two_sided_checkbox, &QCheckBox::toggled, [=](bool is_checked) {
    canvas->makeCurrent();
    renderer->isTwoSided(is_checked);
    canvas->update();
  });

  connect(has_autorotate_checkbox, &QCheckBox::toggled, [=](bool is_checked) {
    canvas->makeCurrent();
    renderer->hasAutorotate(is_checked);
    canvas->update();
  });

  connect(has_specular_checkbox, &QCheckBox::clicked, [=](bool is_checked) {
    canvas->makeCurrent();
    renderer->hasSpecular(is_checked);
    canvas->update();
  });

  connect(show_heading_checkbox, &QCheckBox::toggled, [=](bool is_checked) {
    canvas->makeCurrent();
    renderer->showHeading(is_checked);
    canvas->update();
  });

  connect(show_stops_checkbox, &QCheckBox::toggled, [=](bool is_checked) {
    canvas->makeCurrent();
    renderer->showStops(is_checked);
    canvas->update();
  });

  for (int d = 0; d < 3; ++d) {
    // TODO need local?
    int dd = d;

    connect(show_axis_checkboxes[d], &QCheckBox::toggled, [=](bool is_checked) {
      canvas->makeCurrent();
      renderer->showAxis(dd, is_checked);
      canvas->update();
    });

    connect(show_grid_checkboxes[d], &QCheckBox::toggled, [=](bool is_checked) {
      canvas->makeCurrent();
      renderer->showGrid(dd, is_checked);
      canvas->update();
    });

    connect(grid_extent_spinners[d], static_cast<void(QDoubleSpinBox::*)(double)>(&QDoubleSpinBox::valueChanged), [=](double value) {
      canvas->makeCurrent();
      renderer->setGridExtent(dd, (float) value);
      canvas->update();
    });

    connect(grid_spacing_spinners[d], static_cast<void(QDoubleSpinBox::*)(double)>(&QDoubleSpinBox::valueChanged), [=](double value) {
      canvas->makeCurrent();
      renderer->setGridSpacing(dd, (float) value);
      canvas->update();
    });
  }

  connect(azimuth_angle_spinner, static_cast<void(QDoubleSpinBox::*)(double)>(&QDoubleSpinBox::valueChanged), [=](double value) {
    canvas->makeCurrent();
    renderer->setAzimuthAngle(value);
    canvas->update();
  });

  connect(elevation_angle_spinner, static_cast<void(QDoubleSpinBox::*)(double)>(&QDoubleSpinBox::valueChanged), [=](double value) {
    canvas->makeCurrent();
    renderer->setElevationAngle(value);
    canvas->update();
  });

  connect(shininess_spinner, static_cast<void(QDoubleSpinBox::*)(double)>(&QDoubleSpinBox::valueChanged), [=](double value) {
    canvas->makeCurrent();
    renderer->setShininess(value);
    canvas->update();
  });

  connect(light_distance_factor_spinner, static_cast<void(QDoubleSpinBox::*)(double)>(&QDoubleSpinBox::valueChanged), [=](double value) {
    canvas->makeCurrent();
    renderer->setLightDistanceFactor(value);
    canvas->update();
  });

  connect(axis_stroke_width_spinner, static_cast<void(QDoubleSpinBox::*)(double)>(&QDoubleSpinBox::valueChanged), [=](double value) {
    canvas->makeCurrent();
    renderer->setAxisStrokeWidth(value);
    canvas->update();
  });

  connect(grid_stroke_width_spinner, static_cast<void(QDoubleSpinBox::*)(double)>(&QDoubleSpinBox::valueChanged), [=](double value) {
    canvas->makeCurrent();
    renderer->setGridStrokeWidth(value);
    canvas->update();
  });

  connect(autopathify_checkbox, &QCheckBox::toggled, [=](bool is_checked) {
    // We don't need to do much here. If a previous timer was started, cancel it.
    if (!is_checked) {
      autopathify_timer->stop(); 
    }
  });

  connect(path_stroke_width_spinner, static_cast<void(QDoubleSpinBox::*)(double)>(&QDoubleSpinBox::valueChanged), [=](double d) {
    canvas->makeCurrent();
    renderer->setPathStrokeWidth((float) d);
    canvas->update();
  });

  connect(vertex_size_spinner, static_cast<void(QDoubleSpinBox::*)(double)>(&QDoubleSpinBox::valueChanged), [=](double d) {
    renderer->setVertexSize((float) d);
    canvas->update();
  });

  // Tweaks
  editor->blockSignals(true);
  editor->setPlainText("moveto 0, 0, 0\n"
                  "repeat 4\n"
                  "  move 10\n"
                  "  yaw 90\n"
                  "end\n"
                  "dowel\n");
  onTextChanged();
  setWindowTitle("Madeup");
  editor->blockSignals(false);
}

/* ------------------------------------------------------------------------- */

void MadeupWindow::open(const std::string &path) {
  std::string source = td::Utilities::Slurp(path);
  editor->setPlainText(source.c_str());
  this->setWindowTitle(("Madeup: " + path).c_str());
  mup_path = path;
}

/* ------------------------------------------------------------------------- */

MadeupWindow::~MadeupWindow() {
}

/* ------------------------------------------------------------------------- */

void MadeupWindow::onRun(GeometryMode::geometry_mode_t geometry_mode) {
  std::string source = editor->toPlainText().toStdString();

  std::stringstream cout_capturer;
  std::streambuf *old_cout_buffer = std::cout.rdbuf();
  std::cout.rdbuf(cout_capturer.rdbuf());

  std::stringstream in(source);
  Lexer lexer(in);
  try {
    const std::vector<Token> &tokens = lexer.lex();
    Parser parser(tokens, source);

    td::Co<ExpressionBlock> program = parser.program();

    Environment env;
    env.prime();
    env.setGeometryMode(geometry_mode);
    srand(time(NULL));
    rand();

    program->evaluate(env);

    if (geometry_mode == GeometryMode::PATH) {
      renderer->setPaths(env.getPaths());
    } else if (geometry_mode == GeometryMode::SURFACE) {
      td::Trimesh *trimesh = env.getMesh();
      if (faceted_checkbox->isChecked()) {
        trimesh->DisconnectFaces();
        trimesh->ComputeMeta(true);
      } else {
        td::Trimesh *sharped = madeup::MeshBoolean::compute_normals(*trimesh, 20);
        delete trimesh;
        trimesh = sharped;
        trimesh->ComputeMeta(false);
      }
      if (trimesh->GetVertexCount() == 0) {
        std::cout << "Uh oh. You either didn't visit any locations or didn't invoke a solidifier. I can't make any models without more information from you." << std::endl;
      }
      renderer->setTrimesh(trimesh);
    }

  } catch (std::bad_alloc e) {
    std::cout << "Whoa! I don't have enough memory for that." << std::endl;
  } catch (td::MessagedException e) {
    std::cout << e.GetMessage() << std::endl;
  }

  std::cout.rdbuf(old_cout_buffer);
  string output = cout_capturer.str();

  output = std::regex_replace(output, std::regex("^(\\d+)\\((\\d+)-(\\d+)\\):\\s*"), std::string("<a href=\"jump?start=$2&end=$3\">Error on line $1</a>: "));

  console->setText(output.c_str());

  canvas->update();
}

/* ------------------------------------------------------------------------- */

void MadeupWindow::onTextChanged() {
  // Stop any scheduled pathify.
  autopathify_timer->stop();

  std::string source = editor->toPlainText().toStdString();

  if (mup_path.length() == 0) {
    this->setWindowTitle("Madeup*");
  } else {
    this->setWindowTitle(("Madeup: " + mup_path + "*").c_str());
  }

  std::stringstream in(source);
  Lexer lexer(in);
  lexer.keepComments(true);

  try {
    lexer.lex();
  } catch (td::MessagedException e) {
    std::cerr << e.GetMessage() << std::endl;
  }

  const std::vector<Token> &tokens = lexer.getTokens();
  
  QTextCharFormat punctuation_format;
  punctuation_format.setForeground(Qt::blue);

  QTextCharFormat number_format;
  number_format.setForeground(QColor("#CF6A4C"));

  QTextCharFormat operator_format;
  operator_format.setForeground(QColor("#CDA869"));

  QTextCharFormat keyword_format;
  keyword_format.setForeground(QColor("#CDA869"));

  QTextCharFormat comment_format;
  comment_format.setForeground(QColor("#5F5A60"));

  QTextCharFormat identifier_format;
  identifier_format.setForeground(QColor("#7587A6"));

  QTextCharFormat string_format;
  string_format.setForeground(QColor("#8F9D6A"));

  QTextCharFormat comma_format;
  comma_format.setForeground(QColor("#FFFFFF"));

  editor->blockSignals(true);
  QTextCursor cursor(editor->document());

  // We skip the last two tokens (NEWLINE and END_OF_FILE) because they aren't
  // really in the original source.
  unsigned int ti = 0;
  for (auto token = tokens.begin(); ti + 2 < tokens.size(); ++token, ++ti) {
    SourceLocation location = token->getLocation();
    cursor.setPosition(location.getStartIndex(), QTextCursor::MoveAnchor);
    cursor.setPosition(location.getEndIndex() + 1, QTextCursor::KeepAnchor);

    switch (token->getType()) {
      case Token::COMMA:
      case Token::LEFT_PARENTHESIS:
      case Token::RIGHT_PARENTHESIS:
      case Token::LEFT_CURLY_BRACE:
      case Token::RIGHT_CURLY_BRACE:
      case Token::LEFT_BRACKET:
      case Token::RIGHT_BRACKET:
        cursor.setCharFormat(comma_format);
        break;
      case Token::INTEGER:
      case Token::REAL:
      case Token::TRUE:
      case Token::FALSE:
      case Token::NOTHING:
        cursor.setCharFormat(number_format);
        break;
      case Token::PLUS:
      case Token::COLON:
      case Token::MINUS:
      case Token::TIMES:
      case Token::DIVIDE:
      case Token::REAL_DIVIDE:
      case Token::CIRCUMFLEX:
      case Token::LESS_THAN:
      case Token::LESS_THAN_OR_EQUAL_TO:
      case Token::GREATER_THAN:
      case Token::GREATER_THAN_OR_EQUAL_TO:
      case Token::EQUAL_TO:
      case Token::NOT_EQUAL_TO:
      case Token::REMAINDER:
      case Token::ASSIGN:
      case Token::PIPE:
      case Token::RANGE:
        cursor.setCharFormat(operator_format);
        break;
      case Token::AND:
      case Token::IN:
      case Token::AROUND:
      case Token::THROUGH:
      case Token::OR:
      case Token::NOT:
      case Token::FOR:
      case Token::OF:
      case Token::BY:
      case Token::ELSE:
      case Token::IF:
      case Token::REPEAT:
      case Token::END:
      case Token::THEN:
      case Token::WHILE:
      case Token::TO:
        cursor.setCharFormat(keyword_format);
        break;
      case Token::COMMENT:
        cursor.setCharFormat(comment_format);
        break;
      case Token::ID:
        cursor.setCharFormat(identifier_format);
        break;
      case Token::STRING:
        cursor.setCharFormat(string_format);
        break;
      case Token::NEWLINE:
      case Token::END_OF_FILE:
        break;
    }
  }

  editor->blockSignals(false);

  if (canvas->isValid()) {
    if (autopathify_checkbox->isChecked()) {
      autopathify_timer->start(autopathify_delay_spinner->value() * 1000);
    }
  }
}

/* ------------------------------------------------------------------------- */

void MadeupWindow::selectColor(const td::QVector4<float> &initial_color,
                               std::function<void(const QColor &)> onSelect) {
  QColorDialog *picker = new QColorDialog(toQColor(initial_color), this); 
  picker->setOption(QColorDialog::ShowAlphaChannel);
  picker->setOption(QColorDialog::NoButtons);
  connect(picker, &QColorDialog::currentColorChanged, onSelect);
  picker->setAttribute(Qt::WA_DeleteOnClose);
  picker->show();
}

/* ------------------------------------------------------------------------- */

void MadeupWindow::selectFont() {
  QFontDialog *picker = new QFontDialog(editor->font(), this); 
  picker->setOption(QFontDialog::NoButtons);
  connect(picker, &QFontDialog::currentFontChanged, [=](const QFont &font) {
    editor->setFont(font);
    console->setFont(font);
  });
  picker->setAttribute(Qt::WA_DeleteOnClose);
  picker->show();
}

/* ------------------------------------------------------------------------- */

void MadeupWindow::onFit() {
  renderer->fit();
  canvas->update();
}

/* ------------------------------------------------------------------------- */

void MadeupWindow::onSolidify() {
  onRun(GeometryMode::SURFACE);
}

/* ------------------------------------------------------------------------- */

void MadeupWindow::onPathify() {
  onRun(GeometryMode::PATH);
}

/* ------------------------------------------------------------------------- */

QColor MadeupWindow::toQColor(const td::QVector4<float> &color) {
  td::QVector4<int> tcolor(color * 255.0f);
  return QColor(tcolor[0], tcolor[1], tcolor[2], tcolor[3]);
}

/* ------------------------------------------------------------------------- */

void MadeupWindow::saveAs(const std::string &path) {
  mup_path = path;
  std::string source = editor->toPlainText().toStdString();
  std::ofstream out(path);
  out << source;
  out.close();
  setWindowTitle(("Madeup: " + path).c_str());
}

/* ------------------------------------------------------------------------- */

void MadeupWindow::loadPreferences() {
  canvas->makeCurrent();

  try {
    std::ifstream in(config_path);
    Json::Reader reader;
    Json::Value prefs;
    reader.parse(in, prefs);

    int width = prefs.get("window.width", 1200).asUInt();
    int height = prefs.get("window.height", 600).asUInt();
    resize(width, height);

    int x = prefs.get("window.x", -1).asInt();
    int y = prefs.get("window.y", -1).asInt();
    if (x >= 0 && y >= 0) {
      move(x, y);
    }

    float stroke_width = (float) prefs.get("path.stroke.width", renderer->getPathStrokeWidth()).asDouble();
    renderer->setPathStrokeWidth(stroke_width);
    path_stroke_width_spinner->setValue(stroke_width);

    float vertex_size = (float) prefs.get("vertex.size", renderer->getVertexSize()).asDouble();
    renderer->setVertexSize(vertex_size);
    vertex_size_spinner->setValue(vertex_size);

    string font_face = prefs.get("font.face", "Courier New").asString();
    int font_size = prefs.get("font.size", 18).asUInt();

    QFont font;
    font.setFamily(font_face.c_str());
    font.setPointSize(font_size);

    editor->setFont(font);
    console->setFont(font);

    renderer->showHeading(prefs.get("show.heading", renderer->showHeading()).asBool());
    show_heading_checkbox->setChecked(renderer->showHeading());

    renderer->showStops(prefs.get("show.stops", renderer->showStops()).asBool());
    show_stops_checkbox->setChecked(renderer->showStops());

    Json::Value show_axis_node = prefs.get("show.axis", Json::nullValue);
    Json::Value show_grid_node = prefs.get("show.grid", Json::nullValue);
    Json::Value grid_extent_node = prefs.get("grid.extent", Json::nullValue);
    Json::Value grid_spacing_node = prefs.get("grid.spacing", Json::nullValue);

    for (int d = 0; d < 3; ++d) {
      bool show_axis = show_axis_node.get(Json::ArrayIndex(d), renderer->showAxis(d)).asBool();
      renderer->showAxis(d, show_axis);
      show_axis_checkboxes[d]->setChecked(renderer->showAxis(d));

      bool show_grid = show_grid_node.get(Json::ArrayIndex(d), renderer->showGrid(d)).asBool();
      renderer->showGrid(d, show_grid);
      show_grid_checkboxes[d]->setChecked(renderer->showGrid(d));

      float grid_extent = (float) grid_extent_node.get(Json::ArrayIndex(d), renderer->getGridExtent(d)).asDouble();
      renderer->setGridExtent(d, grid_extent);
      grid_extent_spinners[d]->setValue(renderer->getGridExtent(d));

      float grid_spacing = (float) grid_spacing_node.get(Json::ArrayIndex(d), renderer->getGridSpacing(d)).asDouble();
      renderer->setGridSpacing(d, grid_spacing);
      grid_spacing_spinners[d]->setValue(renderer->getGridSpacing(d));
    }

    // Background color
    Json::Value background_color_node = prefs.get("background.color", Json::nullValue);
    if (!background_color_node.isNull()) {
      td::QVector4<float> color = renderer->getBackgroundColor();
      for (int i = 0; i < 4; ++i) {
        color[i] = (float) background_color_node.get(i, 0.0).asDouble();
      }
      renderer->setBackgroundColor(color);
    }

    QPalette background_color_palette;
    background_color_palette.setColor(QPalette::Button, toQColor(renderer->getBackgroundColor()));
    background_color_button->setPalette(background_color_palette);

    // Path color
    Json::Value path_color_node = prefs.get("path.color", Json::nullValue);
    if (!path_color_node.isNull()) {
      td::QVector4<float> color = renderer->getPathColor();
      for (int i = 0; i < 4; ++i) {
        color[i] = (float) path_color_node.get(i, 0.0).asDouble();
      }
      renderer->setPathColor(color);
    }

    QPalette path_color_palette;
    path_color_palette.setColor(QPalette::Button, toQColor(renderer->getPathColor()));
    path_color_button->setPalette(path_color_palette);

    // Vertex color
    Json::Value vertex_color_node = prefs.get("vertex.color", Json::nullValue);
    if (!vertex_color_node.isNull()) {
      td::QVector4<float> color = renderer->getPathColor();
      for (int i = 0; i < 4; ++i) {
        color[i] = (float) vertex_color_node.get(i, 0.0).asDouble();
      }
      renderer->setVertexColor(color);
    }

    QPalette vertex_color_palette;
    vertex_color_palette.setColor(QPalette::Button, toQColor(renderer->getVertexColor()));
    vertex_color_button->setPalette(vertex_color_palette);

    // Face style
    int face_style = prefs.get("render.style", renderer->getRenderStyle()).asUInt();
    renderer->setRenderStyle(face_style);
    face_style_picker->setCurrentIndex(renderer->getRenderStyle());
  
    // Show preferences
    bool show_settings = prefs.get("show.settings", false).asBool();
    action_settings->setChecked(show_settings);

    // Show console
    bool show_console = prefs.get("show.console", true).asBool();
    show_console_checkbox->setChecked(show_console);

    int settings_page = prefs.get("settings.page", 0).asUInt();
    settings_picker->setCurrentIndex(settings_page);

    double autopathify_delay = prefs.get("autopathify.delay", 0.25).asDouble();
    autopathify_delay_spinner->setValue(autopathify_delay);

    float axis_stroke_width = (float) prefs.get("axis.stroke.width", renderer->getAxisStrokeWidth()).asDouble();
    axis_stroke_width_spinner->setValue(axis_stroke_width);

    float grid_stroke_width = (float) prefs.get("grid.stroke.width", renderer->getGridStrokeWidth()).asDouble();
    grid_stroke_width_spinner->setValue(grid_stroke_width);

    bool autopathify = prefs.get("autopathify", true).asBool();
    autopathify_checkbox->setChecked(autopathify);
    console->setVisible(show_console);

    bool faceted = prefs.get("show.faceted", true).asBool();
    faceted_checkbox->setChecked(faceted);

    bool autorotate = prefs.get("autorotate", renderer->hasAutorotate()).asBool();
    has_autorotate_checkbox->setChecked(autorotate);
    
    bool has_specular = prefs.get("light.has.specular", renderer->hasSpecular()).asBool();
    // TODO
    renderer->hasSpecular(has_specular);
    has_specular_checkbox->setChecked(renderer->hasSpecular());

    bool is_two_sided = prefs.get("light.two.sided", renderer->isTwoSided()).asBool();
    is_two_sided_checkbox->setChecked(is_two_sided);

    double azimuth_angle = prefs.get("light.azimuth.angle", renderer->getAzimuthAngle()).asDouble();
    azimuth_angle_spinner->setValue(azimuth_angle);

    double elevation_angle = prefs.get("light.elevation.angle", renderer->getElevationAngle()).asDouble();
    elevation_angle_spinner->setValue(elevation_angle);

    double shininess = prefs.get("light.shininess", renderer->getShininess()).asDouble();
    shininess_spinner->setValue(shininess);

    double light_distance_factor = prefs.get("light.distance.factor", renderer->getLightDistanceFactor()).asDouble();
    light_distance_factor_spinner->setValue(light_distance_factor);

    // Horizontal splitter
    Json::Value horizontal_sizes_node = prefs.get("horizontal.splitter.sizes", Json::nullValue);
    if (!horizontal_sizes_node.isNull()) {
      QList<int> sizes;
      sizes.push_back(horizontal_sizes_node.get(Json::ArrayIndex(0), -1).asUInt());
      sizes.push_back(horizontal_sizes_node.get(Json::ArrayIndex(1), -1).asUInt());
      sizes.push_back(horizontal_sizes_node.get(Json::ArrayIndex(2), -1).asUInt());
      horizontal_splitter->setSizes(sizes);
    }

    // Vertical splitter
    Json::Value vertical_sizes_node = prefs.get("vertical.splitter.sizes", Json::nullValue);
    if (!vertical_sizes_node.isNull()) {
      QList<int> sizes;
      sizes.push_back(vertical_sizes_node.get(Json::ArrayIndex(0), -1).asUInt());
      sizes.push_back(vertical_sizes_node.get(Json::ArrayIndex(1), -1).asUInt());
      vertical_splitter->setSizes(sizes);
    }

    last_directory = QString(prefs.get("last.directory", last_directory.toStdString()).asString().c_str());

  } catch (int i) {
  }

  canvas->update();
}

/* ------------------------------------------------------------------------- */

void MadeupWindow::savePreferences() {
  try {
    Json::Value prefs;

    prefs["font.size"] = editor->font().pointSize();
    prefs["font.face"] = editor->font().family().toStdString();
    prefs["path.stroke.width"] = renderer->getPathStrokeWidth();
    prefs["vertex.size"] = renderer->getVertexSize();
    prefs["show.heading"] = renderer->showHeading();
    prefs["show.stops"] = renderer->showStops();
    prefs["render.style"] = renderer->getRenderStyle();
    prefs["show.settings"] = action_settings->isChecked();
    prefs["show.console"] = show_console_checkbox->isChecked();
    prefs["autopathify"] = autopathify_checkbox->isChecked();
    prefs["autopathify.delay"] = autopathify_delay_spinner->value();
    prefs["settings.page"] = settings_picker->currentIndex();

    prefs["background.color"] = Json::Value(Json::arrayValue);
    td::QVector4<float> background_color = renderer->getBackgroundColor();
    for (int i = 0; i < 4; ++i) {
      prefs["background.color"].append(background_color[i]);
    }

    prefs["path.color"] = Json::Value(Json::arrayValue);
    td::QVector4<float> path_color = renderer->getPathColor();
    for (int i = 0; i < 4; ++i) {
      prefs["path.color"].append(path_color[i]);
    }

    prefs["vertex.color"] = Json::Value(Json::arrayValue);
    td::QVector4<float> vertex_color = renderer->getVertexColor();
    for (int i = 0; i < 4; ++i) {
      prefs["vertex.color"].append(vertex_color[i]);
    }

    const QList<int> &hsizes = horizontal_splitter->sizes();
    prefs["horizontal.splitter.sizes"] = Json::Value(Json::arrayValue);
    prefs["horizontal.splitter.sizes"].append(hsizes[0]);
    prefs["horizontal.splitter.sizes"].append(hsizes[1]);
    prefs["horizontal.splitter.sizes"].append(hsizes[2]);

    const QList<int> &vsizes = vertical_splitter->sizes();
    prefs["vertical.splitter.sizes"] = Json::Value(Json::arrayValue);
    prefs["vertical.splitter.sizes"].append(vsizes[0]);
    prefs["vertical.splitter.sizes"].append(vsizes[1]);

    prefs["window.height"] = height();
    prefs["window.width"] = width();
    prefs["window.x"] = x();
    prefs["window.y"] = y();

    prefs["show.axis"] = Json::Value(Json::arrayValue);
    prefs["show.grid"] = Json::Value(Json::arrayValue);
    prefs["grid.extent"] = Json::Value(Json::arrayValue);
    prefs["grid.spacing"] = Json::Value(Json::arrayValue);

    for (int d = 0; d < 3; ++d) {
      prefs["show.axis"].append(show_axis_checkboxes[d]->isChecked());
      prefs["show.grid"].append(show_grid_checkboxes[d]->isChecked());
      prefs["grid.extent"].append(grid_extent_spinners[d]->value());
      prefs["grid.spacing"].append(grid_spacing_spinners[d]->value());
    }

    prefs["axis.stroke.width"] = axis_stroke_width_spinner->value();
    prefs["grid.stroke.width"] = grid_stroke_width_spinner->value();

    prefs["light.azimuth.angle"] = azimuth_angle_spinner->value();
    prefs["light.elevation.angle"] = elevation_angle_spinner->value();
    prefs["light.shininess"] = shininess_spinner->value();
    prefs["light.distance.factor"] = light_distance_factor_spinner->value();
    prefs["light.two.sided"] = is_two_sided_checkbox->isChecked();
    prefs["light.has.specular"] = has_specular_checkbox->isChecked();

    prefs["show.faceted"] = faceted_checkbox->isChecked();
    prefs["autorotate"] = has_autorotate_checkbox->isChecked();

    prefs["last.directory"] = last_directory.toStdString();

    Json::StyledWriter writer;
    string json = writer.write(prefs);
    std::ofstream out(config_path);
    out << json;
    out.close();

  } catch (int i) {
  }
}

/* ------------------------------------------------------------------------- */

void MadeupWindow::showEvent(QShowEvent *event) {
  QMainWindow::showEvent(event);
  loadPreferences();
}

/* ------------------------------------------------------------------------- */

void MadeupWindow::closeEvent(QCloseEvent *event) {
  savePreferences();
  QMainWindow::closeEvent(event);
}

/* ------------------------------------------------------------------------- */


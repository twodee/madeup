#ifndef MUPPER_WINDOW_H
#define MUPPER_WINDOW_H

#include <QAction>
#include <QCheckBox>
#include <QComboBox>
#include <QDoubleSpinBox>
#include <QMainWindow>
#include <QPushButton>
#include <QSplitter>
#include <QTextBrowser>
#include <QPlainTextEdit>

#include "MadeupCanvas.h"

/* ------------------------------------------------------------------------- */

namespace Ui {
  class MadeupWindow;
}

/* ------------------------------------------------------------------------- */

class MadeupWindow : public QMainWindow {
  Q_OBJECT

  public:
    explicit MadeupWindow(QWidget *parent = 0);
    ~MadeupWindow();

    void open(const std::string &path);

  public slots:
    void onSolidify();
    void onPathify();
    void onTextChanged();
    void selectFont();
    void onFit();
    void saveAs(const std::string &path);
    void showEvent(QShowEvent *event);
    void closeEvent(QCloseEvent *event);

  private:
    void loadPreferences();
    void savePreferences();

    void selectColor(const td::QVector4<float> &initial_color,
                     std::function<void(const QColor &)> onSelect);
    void onRun(madeup::GeometryMode::geometry_mode_t mode);

    static QColor toQColor(const td::QVector4<float> &color);

    QTimer *autopathify_timer;
    QPlainTextEdit *editor;
    QTextBrowser *console;
    MadeupCanvas *canvas;
    MadeupRenderer *renderer;
    std::string mup_path;
    std::string config_path;

    QDoubleSpinBox *path_stroke_width_spinner;
    QDoubleSpinBox *vertex_size_spinner;
    QDoubleSpinBox *autopathify_delay_spinner;
    QDoubleSpinBox *grid_extent_spinners[3];
    QDoubleSpinBox *grid_spacing_spinners[3];
    QDoubleSpinBox *axis_stroke_width_spinner;
    QDoubleSpinBox *grid_stroke_width_spinner;
    QDoubleSpinBox *azimuth_angle_spinner;
    QDoubleSpinBox *elevation_angle_spinner;
    QDoubleSpinBox *shininess_spinner;
    QDoubleSpinBox *light_distance_factor_spinner;

    QCheckBox *show_heading_checkbox;
    QCheckBox *show_stops_checkbox;
    QCheckBox *show_console_checkbox;
    QCheckBox *autopathify_checkbox;
    QCheckBox *faceted_checkbox;
    QCheckBox *is_two_sided_checkbox;
    QCheckBox *has_autorotate_checkbox;
    QCheckBox *has_specular_checkbox;
    QCheckBox *show_axis_checkboxes[3];
    QCheckBox *show_grid_checkboxes[3];

    QPushButton *background_color_button;
    QPushButton *path_color_button;
    QPushButton *vertex_color_button;

    QComboBox *face_orientation_picker;
    QComboBox *face_style_picker;
    QComboBox *settings_picker;

    QAction *action_settings;

    QSplitter *horizontal_splitter;
    QSplitter *vertical_splitter;
};

/* ------------------------------------------------------------------------- */

#endif

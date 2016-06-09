#ifndef MUPPER_WINDOW_H
#define MUPPER_WINDOW_H

#include <QAction>
#include <QCheckBox>
#include <QComboBox>
#include <QDoubleSpinBox>
#include <QMainWindow>
#include <QPushButton>
#include <QSplitter>
#include <QTextEdit>

#include "MadeupCanvas.h"

/* ------------------------------------------------------------------------- */

namespace Ui {
  class MupperWindow;
}

/* ------------------------------------------------------------------------- */

class MupperWindow : public QMainWindow {
  Q_OBJECT

  public:
    explicit MupperWindow(QWidget *parent = 0);
    ~MupperWindow();

  public slots:
    void onSolidify();
    void onPathify();
    void onTextChanged();
    void selectFont();
    void onFit();
    void saveAs(const QString &path);
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
    QTextEdit *editor;
    QTextEdit *console;
    MadeupCanvas *canvas;
    MadeupRenderer *renderer;
    QString mup_path;
    std::string config_path;

    QDoubleSpinBox *path_stroke_width_spinner;
    QDoubleSpinBox *vertex_size_spinner;
    QDoubleSpinBox *autopathify_delay_spinner;
    QDoubleSpinBox *grid_extent_spinners[3];
    QDoubleSpinBox *grid_spacing_spinners[3];
    QDoubleSpinBox *axis_stroke_width_spinner;
    QDoubleSpinBox *grid_stroke_width_spinner;

    QCheckBox *show_heading_checkbox;
    QCheckBox *show_stops_checkbox;
    QCheckBox *show_console_checkbox;
    QCheckBox *autopathify_checkbox;
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

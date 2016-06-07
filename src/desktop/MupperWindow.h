#ifndef MUPPER_WINDOW_H
#define MUPPER_WINDOW_H

#include <boost/property_tree/ptree.hpp>

#include <QDoubleSpinBox>
#include <QMainWindow>
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
    void loadPreferences(const boost::property_tree::ptree &tree);
    void savePreferences();
    void savePreferences(boost::property_tree::ptree &tree);

    void selectColor(const td::QVector4<float> &initial_color,
                     std::function<void(const QColor &)> onSelect);
    void onRun(madeup::GeometryMode::geometry_mode_t mode);

    static QColor toQColor(const td::QVector4<float> &color);

    QTextEdit *editor;
    QTextEdit *console;
    MadeupCanvas *canvas;
    QString mup_path;
    std::string config_path;
    QDoubleSpinBox *path_stroke_width_spinner;
    QDoubleSpinBox *vertex_size_spinner;
};

/* ------------------------------------------------------------------------- */

#endif

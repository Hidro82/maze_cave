#ifndef SRC_VIEW_INTERFACE_H_
#define SRC_VIEW_INTERFACE_H_

#include <cairomm/surface.h>
#include <gtkmm.h>

#include <chrono>
#include <sstream>
#include <thread>

#include "../control/s21_cave_control.h"
#include "../control/s21_maze_control.h"

namespace s21 {

class CaveWin : public Gtk::ApplicationWindow {
 public:
  void cave_reader();
  void cave_drawer(Cairo::RefPtr<Cairo::Context> const &cr);
  void cave_dialog_open();
  void bd_dialog_open();
  void cave_canceler();
  void bd_canceler();
  void openfile_dialog_open();
  void manual_update();
  void auto_update();
  void on_draw(Cairo::RefPtr<Cairo::Context> const &cr, int width, int height);
  void file_reader(int response_id, Gtk::FileChooserDialog *maze_openfile);
  void fileopen_start();
  void save_dialog();
  void file_saver(int response_id, Gtk::FileChooserDialog *cave_savefile);

  CaveWin(BaseObjectType *obj, Glib::RefPtr<Gtk::Builder> const &builder);

 private:
  Glib::RefPtr<Gtk::Builder> _builder;

  Cave *cave_data = new Cave();
  CaveControl *cave_controller = new CaveControl(cave_data);
  bool cave_flag = false;

  Gtk::Window *cave_menu;
  Gtk::DrawingArea *cave_canvas;
  Gtk::MenuItem *new_cave;
  Gtk::MenuItem *open_cave;
  Gtk::MenuItem *save_cave;
  Gtk::MenuItem *step_man;

  Gtk::Dialog *cave_dialog;
  Gtk::Label *dialog_label;
  Gtk::Scale *cave_width;
  Gtk::Scale *cave_height;
  Gtk::Scale *chance;
  Gtk::Button *cave_ok;
  Gtk::Button *cave_cancel;

  Gtk::Dialog *bd_dialog;
  Gtk::Scale *b_limit;
  Gtk::Scale *d_limit;
  Gtk::Button *cave_manual;
  Gtk::Button *cave_start;
  Gtk::Scale *time_step;
  Gtk::Button *bd_close;

  Gtk::FileChooserDialog *openfile_dialog;
  Gtk::Button *open_ok;
  Gtk::Button *open_cancel;
};

class MazeWin : public Gtk::ApplicationWindow {
 public:
  void maze_reader();
  void way_reader();
  void file_reader(int response_id, Gtk::FileChooserDialog *maze_openfile);
  void maze_drawer(Cairo::RefPtr<Cairo::Context> const &cr);
  void way_drawer(Cairo::RefPtr<Cairo::Context> const &cr);
  void maze_dialog_open();
  void way_dialog_open();
  void maze_canceler();
  void way_canceler();
  void cave_mode_start();
  void on_draw(Cairo::RefPtr<Cairo::Context> const &cr, int width, int height);
  void ready_surface();
  void fileopen_start();
  void save_dialog();
  void file_saver(int response_id, Gtk::FileChooserDialog *maze_savefile);

  MazeWin(BaseObjectType *obj, Glib::RefPtr<Gtk::Builder> const &builder);

 private:
  Glib::RefPtr<Gtk::Builder> _builder;

  maze *maze_data;
  MazeControl *maze_controller;
  bool maze_flag = false;

  Gtk::Window *maze_menu;
  Gtk::DrawingArea *canvas;
  Gtk::MenuItem *new_maze;
  Gtk::MenuItem *new_sol;
  Gtk::MenuItem *cave_mode;
  Gtk::MenuItem *opener;
  Gtk::MenuItem *saver;

  Gtk::Dialog *maze_dialog;
  Gtk::Label *status_label;
  Gtk::Scale *maze_width;
  Gtk::Scale *maze_height;
  Gtk::Button *maze_ok;
  Gtk::Button *maze_cancel;

  Gtk::Dialog *way_dialog;
  Gtk::Scale *x_1;
  Gtk::Scale *y_1;
  Gtk::Scale *x_2;
  Gtk::Scale *y_2;
  Gtk::Button *way_ok;
  Gtk::Button *way_cancel;
};

}  // namespace s21

#endif  // SRC_VIEW_INTERFACE_H_

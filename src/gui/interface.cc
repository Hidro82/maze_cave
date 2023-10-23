#include "interface.h"

int main(int argc, char* argv[]) {
  Gtk::Main app(argc, argv);
  Glib::RefPtr<Gtk::Builder> builder =
      Gtk::Builder::create_from_file("glades/MainWindow.glade");
  s21::MazeWin* MazeWindow = 0;
  builder->get_widget_derived("maze_menu", MazeWindow);
  app.run(*MazeWindow);
  delete MazeWindow;
  return 0;
}

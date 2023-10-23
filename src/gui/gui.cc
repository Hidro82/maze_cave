#include "interface.h"

namespace s21 {

MazeWin::MazeWin(BaseObjectType* obj, Glib::RefPtr<Gtk::Builder> const& builder)
    : Gtk::ApplicationWindow(obj), _builder(builder) {
  // Базовые виджеты (меню и отрисовка лабиринта)
  _builder->get_widget("canvas", canvas);
  _builder->get_widget("new_maze", new_maze);
  _builder->get_widget("new_sol", new_sol);
  _builder->get_widget("cave_mode", cave_mode);
  _builder->get_widget("opener", opener);
  _builder->get_widget("saver", saver);
  // Виджеты диалогового окна для нового лабиринта
  _builder->get_widget("maze_dialog", maze_dialog);
  _builder->get_widget("status_label", status_label);
  _builder->get_widget("maze_width", maze_width);
  _builder->get_widget("maze_height", maze_height);
  _builder->get_widget("maze_ok", maze_ok);
  _builder->get_widget("maze_cancel", maze_cancel);

  _builder->get_widget("way_dialog", way_dialog);
  _builder->get_widget("x_1", x_1);
  _builder->get_widget("y_1", y_1);
  _builder->get_widget("x_2", x_2);
  _builder->get_widget("y_2", y_2);
  _builder->get_widget("way_ok", way_ok);
  _builder->get_widget("way_cancel", way_cancel);

  new_maze->signal_activate().connect(
      sigc::mem_fun(*this, &MazeWin::maze_dialog_open));
  new_sol->signal_activate().connect(
      sigc::mem_fun(*this, &MazeWin::way_dialog_open));
  opener->signal_activate().connect(
      sigc::mem_fun(*this, &MazeWin::fileopen_start));
  saver->signal_activate().connect(sigc::mem_fun(*this, &MazeWin::save_dialog));
  cave_mode->signal_activate().connect(
      sigc::mem_fun(*this, &MazeWin::cave_mode_start));
  maze_ok->signal_clicked().connect(
      sigc::mem_fun(*this, &MazeWin::maze_reader));
  maze_cancel->signal_clicked().connect(
      sigc::mem_fun(*this, &MazeWin::maze_canceler));
  way_ok->signal_clicked().connect(sigc::mem_fun(*this, &MazeWin::way_reader));
  way_cancel->signal_clicked().connect(
      sigc::mem_fun(*this, &MazeWin::way_canceler));
}

void MazeWin::fileopen_start() {
  auto maze_openfile = new Gtk::FileChooserDialog(
      "Choose a file", Gtk::FILE_CHOOSER_ACTION_OPEN);

  maze_openfile->signal_response().connect(
      sigc::bind(sigc::mem_fun(*this, &MazeWin::file_reader), maze_openfile));
  maze_openfile->add_button("_OK", Gtk::RESPONSE_OK);
  maze_openfile->add_button("_Cancel", Gtk::RESPONSE_CANCEL);

  Glib::RefPtr<Gtk::FileFilter> fileFilter = Gtk::FileFilter::create();
  fileFilter->set_name("Maze Files (*.maze)");
  fileFilter->add_pattern("*.maze");
  maze_openfile->add_filter(fileFilter);
  fileFilter = Gtk::FileFilter::create();
  fileFilter->set_name("All Files");
  fileFilter->add_pattern("*.*");
  maze_openfile->add_filter(fileFilter);

  maze_openfile->show();
}

void MazeWin::save_dialog() {
  auto maze_savefile = new Gtk::FileChooserDialog(
      "Choose a file", Gtk::FILE_CHOOSER_ACTION_SAVE);

  maze_savefile->signal_response().connect(
      sigc::bind(sigc::mem_fun(*this, &MazeWin::file_saver), maze_savefile));
  maze_savefile->add_button("_OK", Gtk::RESPONSE_OK);
  maze_savefile->add_button("_Cancel", Gtk::RESPONSE_CANCEL);

  Glib::RefPtr<Gtk::FileFilter> fileFilter = Gtk::FileFilter::create();
  fileFilter->set_name("Maze Files (*.maze)");
  fileFilter->add_pattern("*.maze");
  maze_savefile->add_filter(fileFilter);
  fileFilter = Gtk::FileFilter::create();
  fileFilter->set_name("All Files");
  fileFilter->add_pattern("*.*");
  maze_savefile->add_filter(fileFilter);

  maze_savefile->show();
}

void MazeWin::file_saver(int response_id,
                         Gtk::FileChooserDialog* maze_savefile) {
  switch (response_id) {
    case Gtk::RESPONSE_OK: {
      if (maze_flag) {
        std::string path = maze_savefile->get_file()->get_path();
        std::ofstream maze_file;
        maze_file.open(path);
        maze_file << maze_data->SaveTextMaker();
        maze_file.close();
      }
      break;
    }
    default: {
      break;
    }
  }

  delete maze_savefile;
}

void MazeWin::cave_mode_start() {
  Glib::RefPtr<Gtk::Builder> builder =
      Gtk::Builder::create_from_file("glades/CaveWindow.glade");
  s21::CaveWin* CaveWindow = 0;
  builder->get_widget_derived("cave_menu", CaveWindow);
  CaveWindow->show();
}

void MazeWin::maze_dialog_open() {
  status_label->set_text("Enter the parameters...");
  maze_dialog->show();
}

void MazeWin::way_dialog_open() {
  if (maze_flag) {
    x_1->set_fill_level(static_cast<double>(maze_controller->HorSize()));
    x_2->set_fill_level(static_cast<double>(maze_controller->HorSize()));
    y_1->set_fill_level(static_cast<double>(maze_controller->VerSize()));
    y_2->set_fill_level(static_cast<double>(maze_controller->VerSize()));
    way_dialog->show();
  }
}

void MazeWin::file_reader(int response_id,
                          Gtk::FileChooserDialog* maze_openfile) {
  switch (response_id) {
    case Gtk::RESPONSE_OK: {
      std::string path = maze_openfile->get_file()->get_path();

      maze_data = new maze();
      maze_data->ParseFromFile(path);
      maze_controller = new MazeControl(maze_data);

      auto cr = canvas->get_window()->create_cairo_context();
      maze_drawer(cr);
      maze_flag = true;
      break;
    }
    default: {
      break;
    }
  }

  delete maze_openfile;
}

void MazeWin::maze_reader() {
  int width = static_cast<int>(maze_width->get_value());
  int height = static_cast<int>(maze_height->get_value());

  maze_data = new maze(width, height);
  maze_controller = new MazeControl(maze_data);

  auto cr = canvas->get_window()->create_cairo_context();
  maze_drawer(cr);
  maze_flag = true;
  maze_dialog->close();
}

void MazeWin::way_reader() {
  int start_x = static_cast<int>(x_1->get_value()) - 1;
  int start_y = static_cast<int>(y_1->get_value()) - 1;
  int finish_x = static_cast<int>(x_2->get_value() - 1);
  int finish_y = static_cast<int>(y_2->get_value() - 1);

  std::pair<int, int> starter = std::make_pair(start_y, start_x);
  std::pair<int, int> finisher = std::make_pair(finish_y, finish_x);
  maze_controller->CreateSolution(starter, finisher);
  auto cr = canvas->get_window()->create_cairo_context();
  maze_drawer(cr);
  way_drawer(cr);
  way_dialog->close();
}

void MazeWin::maze_canceler() { maze_dialog->close(); }

void MazeWin::way_canceler() { way_dialog->close(); }

void MazeWin::maze_drawer(Cairo::RefPtr<Cairo::Context> const& cr) {
  int verSize = 500 / maze_controller->VerSize();
  int horSize = 500 / maze_controller->HorSize();
  int x_offset = 0;
  int y_offset = 0;
  // Делаем белый фон
  cr->set_source_rgb(1.0, 1.0, 1.0);
  cr->paint();
  cr->stroke();
  // Задаём цвет линии
  cr->set_source_rgb(0.0, 0.0, 0.0);
  cr->set_line_width(2);
  for (int i = 0; i < maze_controller->VerSize(); i++) {
    for (int j = 0; j < maze_controller->HorSize(); j++) {
      if (maze_controller->IsWall(i, j)) {
        cr->move_to(x_offset + horSize, y_offset);
        cr->line_to(x_offset + horSize, y_offset + verSize);
        cr->stroke();
      }
      if (maze_controller->IsFloor(i, j)) {
        cr->move_to(x_offset, y_offset + verSize);
        cr->line_to(x_offset + horSize, y_offset + verSize);
        cr->stroke();
      }
      x_offset += horSize;
    }
    y_offset += verSize;
    x_offset = 0;
  }
}

void MazeWin::way_drawer(Cairo::RefPtr<Cairo::Context> const& cr) {
  int verSize = 500 / maze_controller->VerSize();
  int horSize = 500 / maze_controller->HorSize();
  int x_offset = 0;
  int y_offset = 0;
  std::queue<std::pair<int, int>> solution = maze_controller->SolutionPlace();
  std::pair<int, int> current = solution.front();

  cr->set_source_rgb(1.0, 0.0, 0.0);
  x_offset = horSize * (current.second + 1) - horSize / 2;
  y_offset = verSize * (current.first + 1) - verSize / 2;
  cr->move_to(x_offset, y_offset);
  solution.pop();
  while (solution.size() != 0) {
    current = solution.front();
    x_offset = horSize * (current.second + 1) - horSize / 2;
    y_offset = verSize * (current.first + 1) - verSize / 2;
    cr->line_to(x_offset, y_offset);
    cr->stroke();
    cr->move_to(x_offset, y_offset);
    solution.pop();
  }
}

void MazeWin::on_draw(Cairo::RefPtr<Cairo::Context> const& cr, int width,
                      int height) {
  if (maze_flag) {
    maze_drawer(cr);
    cr->fill();
  } else {
    cr->set_source_rgb(1.0, 1.0, 1.0);
    cr->paint();
  }
}

CaveWin::CaveWin(BaseObjectType* obj, Glib::RefPtr<Gtk::Builder> const& builder)
    : Gtk::ApplicationWindow(obj), _builder(builder) {
  // Виджеты окна отрисовки пещеры
  _builder->get_widget("cave_canvas", cave_canvas);
  _builder->get_widget("new_cave", new_cave);
  _builder->get_widget("open_cave", open_cave);
  _builder->get_widget("save_cave", save_cave);
  _builder->get_widget("step_man", step_man);
  _builder->get_widget("cave_manual", cave_manual);
  _builder->get_widget("cave_start", cave_start);
  _builder->get_widget("time_step", time_step);
  // Виджеты диалогового окна для новой пещеры
  _builder->get_widget("cave_dialog", cave_dialog);
  _builder->get_widget("dialog_label", dialog_label);
  _builder->get_widget("cave_width", cave_width);
  _builder->get_widget("cave_height", cave_height);
  _builder->get_widget("b_limit", b_limit);
  _builder->get_widget("d_limit", d_limit);
  _builder->get_widget("chance", chance);
  _builder->get_widget("cave_ok", cave_ok);
  _builder->get_widget("cave_cancel", cave_cancel);
  // Виджеты окна открытия файла
  _builder->get_widget("bd_dialog", bd_dialog);
  _builder->get_widget("bd_close", bd_close);

  new_cave->signal_activate().connect(
      sigc::mem_fun(*this, &CaveWin::cave_dialog_open));
  open_cave->signal_activate().connect(
      sigc::mem_fun(*this, &CaveWin::fileopen_start));
  save_cave->signal_activate().connect(
      sigc::mem_fun(*this, &CaveWin::save_dialog));
  step_man->signal_activate().connect(
      sigc::mem_fun(*this, &CaveWin::bd_dialog_open));
  cave_ok->signal_clicked().connect(
      sigc::mem_fun(*this, &CaveWin::cave_reader));
  cave_cancel->signal_clicked().connect(
      sigc::mem_fun(*this, &CaveWin::cave_canceler));
  cave_manual->signal_clicked().connect(
      sigc::mem_fun(*this, &CaveWin::manual_update));
  cave_start->signal_clicked().connect(
      sigc::mem_fun(*this, &CaveWin::auto_update));
  bd_close->signal_clicked().connect(
      sigc::mem_fun(*this, &CaveWin::bd_canceler));
}

void CaveWin::manual_update() {
  int death = static_cast<int>(d_limit->get_value());
  int birth = static_cast<int>(b_limit->get_value());

  if (cave_flag) {
    auto cr = cave_canvas->get_window()->create_cairo_context();
    cave_controller->SetBD(birth, death);
    cave_controller->MakeUpdate();
    cave_drawer(cr);
  }
}

void CaveWin::auto_update() {
  int death = static_cast<int>(d_limit->get_value());
  int birth = static_cast<int>(b_limit->get_value());
  int timing = static_cast<int>(time_step->get_value());

  if (cave_flag) {
    cave_controller->SetBD(birth, death);
    auto cr = cave_canvas->get_window()->create_cairo_context();
    while (!cave_controller->MakeUpdate()) {
      std::this_thread::sleep_for(std::chrono::milliseconds(timing));
      cave_drawer(cr);
    }
  }
}

void CaveWin::fileopen_start() {
  auto cave_openfile = new Gtk::FileChooserDialog(
      "Choose a file", Gtk::FILE_CHOOSER_ACTION_OPEN);

  cave_openfile->signal_response().connect(
      sigc::bind(sigc::mem_fun(*this, &CaveWin::file_reader), cave_openfile));
  cave_openfile->add_button("_OK", Gtk::RESPONSE_OK);
  cave_openfile->add_button("_Cancel", Gtk::RESPONSE_CANCEL);

  Glib::RefPtr<Gtk::FileFilter> fileFilter = Gtk::FileFilter::create();
  fileFilter->set_name("Cave Files (*.cave)");
  fileFilter->add_pattern("*.cave");
  cave_openfile->add_filter(fileFilter);
  fileFilter = Gtk::FileFilter::create();
  fileFilter->set_name("All Files");
  fileFilter->add_pattern("*.*");
  cave_openfile->add_filter(fileFilter);

  cave_openfile->show();
}

void CaveWin::file_reader(int response_id,
                          Gtk::FileChooserDialog* cave_openfile) {
  switch (response_id) {
    case Gtk::RESPONSE_OK: {
      std::string path = cave_openfile->get_file()->get_path();

      cave_controller->FileParser(path);

      auto cr = cave_canvas->get_window()->create_cairo_context();
      cave_drawer(cr);
      cave_flag = true;
      bd_dialog->show();
      break;
    }
    default: {
      break;
    }
  }

  delete cave_openfile;
}

void CaveWin::save_dialog() {
  auto cave_savefile = new Gtk::FileChooserDialog(
      "Choose a file", Gtk::FILE_CHOOSER_ACTION_SAVE);

  cave_savefile->signal_response().connect(
      sigc::bind(sigc::mem_fun(*this, &CaveWin::file_saver), cave_savefile));
  cave_savefile->add_button("_OK", Gtk::RESPONSE_OK);
  cave_savefile->add_button("_Cancel", Gtk::RESPONSE_CANCEL);

  Glib::RefPtr<Gtk::FileFilter> fileFilter = Gtk::FileFilter::create();
  fileFilter->set_name("Cave Files (*.cave)");
  fileFilter->add_pattern("*.cave");
  cave_savefile->add_filter(fileFilter);
  fileFilter = Gtk::FileFilter::create();
  fileFilter->set_name("All Files");
  fileFilter->add_pattern("*.*");
  cave_savefile->add_filter(fileFilter);

  cave_savefile->show();
}

void CaveWin::file_saver(int response_id,
                         Gtk::FileChooserDialog* cave_savefile) {
  switch (response_id) {
    case Gtk::RESPONSE_OK: {
      if (cave_flag) {
        std::string path = cave_savefile->get_file()->get_path();
        std::ofstream maze_file;
        maze_file.open(path);
        maze_file << cave_data->SaveTextMaker();
        maze_file.close();
      }
      break;
    }
    default: {
      break;
    }
  }

  delete cave_savefile;
}

void CaveWin::cave_dialog_open() { cave_dialog->show(); }

void CaveWin::bd_dialog_open() { bd_dialog->show(); }

void CaveWin::cave_canceler() { cave_dialog->close(); }

void CaveWin::bd_canceler() { bd_dialog->close(); }

void CaveWin::cave_reader() {
  int width = static_cast<int>(cave_width->get_value());
  int height = static_cast<int>(cave_height->get_value());
  int bd_chance = static_cast<int>(chance->get_value());

  cave_controller->Randomizer(height, width, bd_chance);
  auto cr = cave_canvas->get_window()->create_cairo_context();

  cave_drawer(cr);
  cave_flag = true;
  bd_dialog->show();
  cave_dialog->close();
}

void CaveWin::cave_drawer(Cairo::RefPtr<Cairo::Context> const& cr) {
  int verSize = 500 / cave_controller->VerSize();
  int horSize = 500 / cave_controller->HorSize();
  int x_offset = 0;
  int y_offset = 0;
  // Делаем белый фон
  cr->set_source_rgb(1.0, 1.0, 1.0);
  cr->paint();
  cr->stroke();
  // Задаём цвет линии
  cr->set_source_rgb(0.0, 0.0, 0.0);
  cr->set_line_width(horSize);
  for (int i = 0; i < cave_controller->VerSize(); i++) {
    for (int j = 0; j < cave_controller->HorSize(); j++) {
      if (cave_controller->IsAlive(i, j)) {
        cr->move_to(x_offset + horSize / 2, y_offset);
        cr->line_to(x_offset + horSize / 2, y_offset + verSize);
        cr->stroke();
      }
      x_offset += horSize;
    }
    y_offset += verSize;
    x_offset = 0;
  }
}

void CaveWin::on_draw(Cairo::RefPtr<Cairo::Context> const& cr, int width,
                      int height) {
  if (cave_flag) {
    cave_drawer(cr);
    cr->fill();
  }
}

}  // namespace s21
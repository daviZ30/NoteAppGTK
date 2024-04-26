#ifndef GTKMM_NoteApplication_H
#define GTKMM_NoteApplication_H

#include <gtkmm.h>

class NoteApplication : public Gtk::Application
{
protected:
  NoteApplication();

public:
  static Glib::RefPtr<NoteApplication> create();

protected:
  // Overrides of default signal handlers:
  void on_startup() override;
  void on_activate() override;

private:
  void create_window();

  void generete_menu();
  void on_window_hide(Gtk::Window *window);
  void on_menu_file_new_generic();
  void on_menu_save_as();
  void on_menu_save();
  void on_menu_file_quit();
  void on_folder_dialog_response(int response_id, Gtk::FileChooserDialog *dialog);
  Glib::RefPtr<Gtk::Builder> m_refBuilder;
  std::string full_path;
};

#endif /* GTKMM_NoteApplication_H */
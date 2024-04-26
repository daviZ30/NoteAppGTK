#ifndef GTKMM_EXAMPLEWINDOW_H
#define GTKMM_EXAMPLEWINDOW_H

#include <gtkmm.h>

class MainWindow : public Gtk::ApplicationWindow
{
public:
  MainWindow();
  virtual ~MainWindow();
  void clearTextView();
  Gtk::TextView m_TextView;
  Gtk::Entry enTitle;
  Glib::RefPtr<Gtk::TextBuffer> m_refTextBuffer = Gtk::TextBuffer::create();
  ;

protected:
  // Child widgets:
  Gtk::Box m_Box;
  Gtk::Box m_Box_H;

  Gtk::ScrolledWindow m_ScrolledWindow;
};

#endif // GTKMM_EXAMPLEWINDOW_H
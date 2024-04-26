#include "MainWindow.h"
#include <iostream>

MainWindow::MainWindow()
    : Gtk::ApplicationWindow(),
      m_Box(Gtk::Orientation::VERTICAL),
      m_Box_H(Gtk::Orientation::HORIZONTAL)
{
    set_title("Main menu example");
    set_default_size(1000, 500);

    // ExampleApplication displays the menubar. Other stuff, such as a toolbar,
    // is put into the box.
    set_child(m_Box);

    m_TextView.set_buffer(m_refTextBuffer);
    m_TextView.set_margin(10);
    m_ScrolledWindow.set_child(m_TextView);

    // Only show the scrollbars when they are necessary:
    m_ScrolledWindow.set_policy(Gtk::PolicyType::AUTOMATIC, Gtk::PolicyType::AUTOMATIC);
    m_ScrolledWindow.set_expand();

    enTitle.set_text("Nuevo_Archivo");
    enTitle.set_placeholder_text("Nombre del archivo");
    m_Box_H.append(enTitle);
    m_Box.append(m_Box_H);
    m_Box.append(m_ScrolledWindow);
}
void MainWindow::clearTextView()
{
    m_refTextBuffer->set_text("");
}

MainWindow::~MainWindow()
{
}

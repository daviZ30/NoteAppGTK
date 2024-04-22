#include "NoteApplication.h"
#include "MainWindow.h"
#include <iostream>
#include <fstream>

static MainWindow *win;
NoteApplication::NoteApplication()
    : Gtk::Application("org.gtkmm.example.main_menu")
{
    Glib::set_application_name("Main Menu Example");
}

Glib::RefPtr<NoteApplication> NoteApplication::create()
{
    return Glib::make_refptr_for_instance<NoteApplication>(new NoteApplication());
}

void NoteApplication::on_startup()
{
    // Call the base class's implementation:
    Gtk::Application::on_startup();

    // Create actions for menus and toolbars.
    // We can use add_action() because Gtk::Application derives from Gio::ActionMap.

    // File|New sub menu:
    add_action("new",
               sigc::mem_fun(*this, &NoteApplication::on_menu_file_new_generic));

    add_action("save_as",
               sigc::mem_fun(*this, &NoteApplication::on_menu_save_as));

    // File menu:
    add_action("quit", sigc::mem_fun(*this, &NoteApplication::on_menu_file_quit));

    m_refBuilder = Gtk::Builder::create();

    // Layout the actions in a menubar and a menu:
    Glib::ustring ui_info =
        "<interface>"
        "  <!-- menubar -->"
        "  <menu id='menu-example'>"
        "    <submenu>"
        "      <attribute name='label' translatable='yes'>_File</attribute>"
        "      <section>"
        "        <item>"
        "          <attribute name='label' translatable='yes'>_New</attribute>"
        "          <attribute name='action'>app.new</attribute>"
        "          <attribute name='accel'>&lt;Primary&gt;n</attribute>"
        "        </item>"
        "        <item>"
        "          <attribute name='label' translatable='yes'>Save As</attribute>"
        "          <attribute name='action'>app.save_as</attribute>"
        "        </item>"
        "        <item>"
        "          <attribute name='label' translatable='yes'>_Save</attribute>"
        "          <attribute name='action'>app.save</attribute>"
        "          <attribute name='accel'>&lt;Primary&gt;s</attribute>"
        "        </item>"
        "      </section>"
        "      <section>"
        "        <item>"
        "          <attribute name='label' translatable='yes'>_Quit</attribute>"
        "          <attribute name='action'>app.quit</attribute>"
        "          <attribute name='accel'>&lt;Primary&gt;q</attribute>"
        "        </item>"
        "      </section>"
        "    </submenu>"
        "  </menu>"
        "</interface>";

    try
    {
        m_refBuilder->add_from_string(ui_info);
    }
    catch (const Glib::Error &ex)
    {
        std::cerr << "Building menus failed: " << ex.what();
    }

    // Get the menubar and the app menu, and add them to the application:
    auto object = m_refBuilder->get_object("menu-example");
    // auto object_btn = m_refBuilder->get_object("btn_save");
    // auto btn_save = std::dynamic_pointer_cast<Gio::MenuItem>(object_btn);

    auto gmenu = std::dynamic_pointer_cast<Gio::Menu>(object);

    if (!gmenu)
    {
        g_warning("GMenu not found");
    }
    else
    {
        set_menubar(gmenu);
    }
}

void NoteApplication::on_activate()
{
    // std::cout << "debug1: " << G_STRFUNC << std::endl;
    //  The application has been started, so let's show a window.
    //  A real application might want to reuse this window in on_open(),
    //  when asked to open a file, if no changes have been made yet.
    create_window();
}

void NoteApplication::create_window()
{
    auto *window = new MainWindow();
    win = window;

    // Make sure that the application runs for as long this window is still open:
    add_window(*window);

    // Delete the window when it is hidden.
    // That's enough for this simple example.
    window->signal_hide().connect(sigc::bind(
        sigc::mem_fun(*this, &NoteApplication::on_window_hide), window));

    window->set_show_menubar();
    window->show();
}

void NoteApplication::on_window_hide(Gtk::Window *window)
{
    delete window;
}

void NoteApplication::on_menu_file_new_generic()
{
    std::cout << "A File|New menu item was selected." << std::endl;
}

void NoteApplication::on_folder_dialog_response(int response_id, Gtk::FileChooserDialog *dialog)
{
    switch (response_id)
    {
    case Gtk::ResponseType::OK:
    {
        std::string folder_path = dialog->get_file()->get_path();

        std::string file_name = win->enTitle.get_text();
        full_path = folder_path + "/" + file_name + ".txt";

        std::string text = win->m_TextView.get_buffer()->get_text();

        std::ofstream file(full_path);

        if (file.is_open())
        {
            file << text << std::endl;
            file.close();
        }
        else
        {
            std::cerr << "Error al abrir el archivo." << std::endl;
        }

        std::cout << "Archivo guardado en: " << full_path << std::endl;
        break;
    }
    case Gtk::ResponseType::CANCEL:
    {
        std::cout << "Cancel clicked." << std::endl;
        break;
    }
    default:
    {
        std::cout << "Unexpected button clicked." << std::endl;
        break;
    }
    }
    delete dialog;
}
void NoteApplication::on_menu_save_as()
{

    auto dialog = new Gtk::FileChooserDialog("Please choose a folder",
                                             Gtk::FileChooser::Action::SELECT_FOLDER);
    dialog->set_transient_for(*win);
    dialog->set_modal(true);
    dialog->signal_response().connect(sigc::bind(
        sigc::mem_fun(*this, &NoteApplication::on_folder_dialog_response), dialog));

    dialog->add_button("_Cancel", Gtk::ResponseType::CANCEL);
    dialog->add_button("Select", Gtk::ResponseType::OK);

    dialog->show();

    add_action("save",
               sigc::mem_fun(*this, &NoteApplication::on_menu_save));
}
void NoteApplication::on_menu_save()
{

    std::string text = win->m_TextView.get_buffer()->get_text();

    std::ofstream file(full_path);

    if (file.is_open())
    {
        file << text << std::endl;

        file.close();
    }
    else
    {
        std::cerr << "Error al abrir el archivo." << std::endl;
    }

    std::cout << "Archivo guardado en: " << full_path << std::endl;
}

void NoteApplication::on_menu_file_quit()
{
    std::cout << G_STRFUNC << std::endl;
    quit();
    std::vector<Gtk::Window *> windows = get_windows();
    if (windows.size() > 0)
        windows[0]->hide();
}
#include <gtk/gtk.h>
#include <stdlib.h>
#include <stdio.h>

#define INI_FILE "progman.ini"
//sudo apt-get install gtk+3.0
//sudo apt-get install gtk+-3.0
//gcc -o progman progman.c `pkg-config --cflags --libs gtk+-3.0`
// Função para carregar o conteúdo do arquivo ini na listbox
void load_ini_file(GtkListStore *list_store) {
    FILE *file = fopen(INI_FILE, "r");
    if (file) {
        char line[256];
        GtkTreeIter iter;
        gtk_list_store_clear(list_store);
        while (fgets(line, sizeof(line), file)) {
            line[strcspn(line, "\r\n")] = 0; // Remove nova linha
            gtk_list_store_append(list_store, &iter);
            gtk_list_store_set(list_store, &iter, 0, line, -1);
        }
        fclose(file);
    }
}

// Função para adicionar uma nova linha ao arquivo ini
void add_line_to_ini_file(const char *line) {
    FILE *file = fopen(INI_FILE, "a");
    if (file) {
        fprintf(file, "%s\n", line);
        fclose(file);
    }
}

// Callback para o botão Run Notepad
void on_btn_run_notepad_clicked(GtkButton *button, gpointer user_data) {
    system("gedit progman.ini");
}

// Callback para o botão Run
void on_btn_run_clicked(GtkButton *button, gpointer user_data) {
    GtkWidget *entry = GTK_WIDGET(user_data);
    const char *command = gtk_entry_get_text(GTK_ENTRY(entry));
    system(command);
}

// Callback para o botão Add
void on_btn_add_clicked(GtkButton *button, gpointer user_data) {
    GtkWidget *entry = GTK_WIDGET(user_data);
    GtkListStore *list_store = GTK_LIST_STORE(g_object_get_data(G_OBJECT(button), "list_store"));
    const char *command = gtk_entry_get_text(GTK_ENTRY(entry));
    add_line_to_ini_file(command);
    load_ini_file(list_store);
}

// Callback para a seleção na listbox
void on_listbox_row_activated(GtkTreeView *tree_view, GtkTreePath *path, GtkTreeViewColumn *column, gpointer user_data) {
    GtkTreeModel *model = gtk_tree_view_get_model(tree_view);
    GtkTreeIter iter;
    if (gtk_tree_model_get_iter(model, &iter, path)) {
        char *command;
        gtk_tree_model_get(model, &iter, 0, &command, -1);
        system(command);
        g_free(command);
    }
}

int main(int argc, char *argv[]) {
    gtk_init(&argc, &argv);

    // Verifica se o arquivo ini existe, se não, cria com uma linha inicial
    FILE *file = fopen(INI_FILE, "r");
    if (!file) {
        file = fopen(INI_FILE, "w");
        if (file) {
            fprintf(file, "gedit progman.ini\n");
            fclose(file);
        }
    } else {
        fclose(file);
    }

    GtkWidget *window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), "Progman");
    gtk_window_set_default_size(GTK_WINDOW(window), 500, 300);
    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

    GtkWidget *box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);
    gtk_container_add(GTK_CONTAINER(window), box);
    gtk_widget_override_background_color(box, GTK_STATE_FLAG_NORMAL, &(GdkRGBA){1, 1, 0, 1});

    GtkListStore *list_store = gtk_list_store_new(1, G_TYPE_STRING);
    GtkWidget *tree_view = gtk_tree_view_new_with_model(GTK_TREE_MODEL(list_store));
    GtkTreeViewColumn *column = gtk_tree_view_column_new_with_attributes("Commands", gtk_cell_renderer_text_new(), "text", 0, NULL);
    gtk_tree_view_append_column(GTK_TREE_VIEW(tree_view), column);
    g_signal_connect(tree_view, "row-activated", G_CALLBACK(on_listbox_row_activated), NULL);
    gtk_box_pack_start(GTK_BOX(box), tree_view, TRUE, TRUE, 0);

    load_ini_file(list_store);

    GtkWidget *entry = gtk_entry_new();
    gtk_box_pack_start(GTK_BOX(box), entry, FALSE, FALSE, 0);

    GtkWidget *btn_run = gtk_button_new_with_label("Run");
    g_signal_connect(btn_run, "clicked", G_CALLBACK(on_btn_run_clicked), entry);
    gtk_box_pack_start(GTK_BOX(box), btn_run, FALSE, FALSE, 0);

    GtkWidget *btn_add = gtk_button_new_with_label("Add");
    g_signal_connect(btn_add, "clicked", G_CALLBACK(on_btn_add_clicked), entry);
    g_object_set_data(G_OBJECT(btn_add), "list_store", list_store);
    gtk_box_pack_start(GTK_BOX(box), btn_add, FALSE, FALSE, 0);

    GtkWidget *btn_run_notepad = gtk_button_new_with_label("Run Notepad");
    g_signal_connect(btn_run_notepad, "clicked", G_CALLBACK(on_btn_run_notepad_clicked), NULL);
    gtk_box_pack_start(GTK_BOX(box), btn_run_notepad, FALSE, FALSE, 0);

    gtk_widget_show_all(window);

    gtk_main();

    return 0;
}


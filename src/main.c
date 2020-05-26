#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include <GL/gl.h>
#include <GL/glx.h>
#include <GL/glu.h>
#include <gtk/gtk.h>

#include <stdbool.h>

//#include "rom.h"
#include <cpu.h>
//#include "gpu.h"
//#include "interrupts.h"
//#include "debug.h"
#include <keys.h>
//#include "timer.h"
#include <globals.h>
#include <memory.h>

#include <main.h>

GtkWidget *window;
GdkPixbuf *pixbuf;
GdkPixbuf *new_pixbuf;
GdkPixbuf *icon;
GError *error = NULL;

int smult=1;
int started=0;

void quit(void) {
	gtk_main_quit();
	exit(0);
}

int screen_mult4 (){
        if(started==0){
        smult = 4;
        gtk_window_resize(GTK_WINDOW(window), 160*smult,144*smult);
        }
        return 0;
}

int screen_mult2 (){
        if(started==0){
        smult = 2;
        gtk_window_resize(GTK_WINDOW(window), 160*smult,144*smult);
        }
        return 0;
}

int screen_mult1 (){
        if(started==0){
        smult = 1;
        gtk_window_resize(GTK_WINDOW(window), 160*smult,144*smult);
        }
        return 0;
}

int toggle_debug(){

	if(debug_toggle){
		debug_toggle = false;
		g_printerr("Debug Turned Off\n");
	}
	else
	{
		debug_toggle = true;
		g_printerr("Debug Turned On\n");
	}
	
}

void print_regs()
{
	//testing print registers
	printf("======================\n");
	printf("AF - %#06hx\n", print_reg_af());
	printf("BC - %#06hx\n", print_reg_bc());
	printf("DE - %#06hx\n", print_reg_de());
	printf("HL - %#06hx\n", print_reg_hl());
	printf("SP - %#06hx\n", print_reg_sp());
	printf("======================\n");
	return;
}

static gboolean key_release_event(GtkWidget *widget, GdkEventKey *event)
{
	char *test = gdk_keyval_name (event->keyval);
    //g_printerr("%s - Released\n", gdk_keyval_name (event->keyval));
    if(strcmp(test, "z") == 0){
		g_printerr("%s - Released\n", gdk_keyval_name (event->keyval));
                b_key = 1;
	}else if(strcmp(test, "x") == 0){
		g_printerr("%s - Released\n", gdk_keyval_name (event->keyval));
                a_key = 1;
	}else if(strcmp(test, "Return") == 0){
		g_printerr("%s - Released\n", gdk_keyval_name (event->keyval));
                start_key = 1;
	}else if(strcmp(test, "BackSpace") == 0){
		g_printerr("%s - Released\n", gdk_keyval_name (event->keyval));
                select_key = 1;
	}else if(strcmp(test, "Up") == 0){
		g_printerr("%s - Released\n", gdk_keyval_name (event->keyval));
                up_key = 1;
	}else if(strcmp(test, "Down") == 0){
		g_printerr("%s - Released\n", gdk_keyval_name (event->keyval));
                down_key = 1;
	}else if(strcmp(test, "Left") == 0){
		g_printerr("%s - Released\n", gdk_keyval_name (event->keyval));
                left_key = 1;
	}else if(strcmp(test, "Right") == 0){
		g_printerr("%s - Released\n", gdk_keyval_name (event->keyval));
                right_key = 1;
	}
return FALSE;
}

static gboolean key_event(GtkWidget *widget, GdkEventKey *event)
{
    char *test = gdk_keyval_name (event->keyval);
    //g_printerr("%s - Pressed\n", gdk_keyval_name (event->keyval));
    if(strcmp(test, "Escape") == 0){
                quit();
    }else if(strcmp(test, "z") == 0){
		g_printerr("%s - Pressed\n", gdk_keyval_name (event->keyval));
    			b_key = 0;
	}else if(strcmp(test, "x") == 0){
		g_printerr("%s - Pressed\n", gdk_keyval_name (event->keyval));
                a_key = 0;
	}else if(strcmp(test, "Return") == 0){
		g_printerr("%s - Pressed\n", gdk_keyval_name (event->keyval));
                start_key = 0;
	}else if(strcmp(test, "BackSpace") == 0){
		g_printerr("%s - Pressed\n", gdk_keyval_name (event->keyval));
                select_key = 0;
	}else if(strcmp(test, "Up") == 0){
		g_printerr("%s - Pressed\n", gdk_keyval_name (event->keyval));
                up_key = 0;
	}else if(strcmp(test, "Down") == 0){
		g_printerr("%s - Pressed\n", gdk_keyval_name (event->keyval));
                down_key = 0;
	}else if(strcmp(test, "Left") == 0){
		g_printerr("%s - Pressed\n", gdk_keyval_name (event->keyval));
                left_key = 0;
	}else if(strcmp(test, "Right") == 0){
		g_printerr("%s - Pressed\n", gdk_keyval_name (event->keyval));
                right_key = 0;
	}
    return FALSE;
}

void start_rom(char *filename){
	printf("Load function call\n");
	//g_signal_handler_disconnect(window, handler_id);

        if(!loadRom(filename)) {
                printf("Failed to load Rom!\n");
                quit();
                return;
        }else{
		printf("Loaded Rom file!\n");
		g_free (filename);
	}
	started =1;

	srand(time(NULL));

	//init cpu/mem
	initcpu();
	initmem();
	printf("Reset completed\n");
	print_regs();
		
	GdkColor color ;
        GdkGC *gc;
        gc = gdk_gc_new(window->window);
        color.red = 0, color.green = 0, color.blue = 0;
        gdk_color_alloc(gdk_colormap_get_system(),&color);
        gdk_gc_set_foreground(gc, &color);
	pixbuf = gdk_pixbuf_new (GDK_COLORSPACE_RGB, FALSE, 8, 160*smult, 144*smult);;
        gdk_pixbuf_fill(pixbuf, 0x55555555);
	
	const int MAXCYCLES = 69905 ;
  	int cyclesThisUpdate = 0 ;
	while(1) {
		while (cyclesThisUpdate < MAXCYCLES)
		{	
		//cpuStep();
		//cyclesThisUpdate = ticks;	
		//timerTick(cyclesThisUpdate);
		//gpuStep();
		//interruptStep();
	    cyclesThisUpdate += 4; // required for framework debugging prior to implementing CPU
		}
		cyclesThisUpdate = 0 ;
		gtk_main_iteration_do(FALSE);
	}
	
	quit();
	
	//return 0;
}

int choose_rom_file (){
        GtkWidget *dialog;
        dialog = gtk_file_chooser_dialog_new ("Open File",
                                      GTK_WINDOW (window),
                                      GTK_FILE_CHOOSER_ACTION_OPEN,
                                      GTK_STOCK_CANCEL, GTK_RESPONSE_CANCEL,
                                      GTK_STOCK_OPEN, GTK_RESPONSE_ACCEPT,
                                      NULL);
        if (gtk_dialog_run (GTK_DIALOG (dialog)) == GTK_RESPONSE_ACCEPT)
        {
                char *filename;
                filename = gtk_file_chooser_get_filename (GTK_FILE_CHOOSER (dialog));
                gtk_widget_destroy (dialog);
		start_rom(filename);
                g_free (filename);
        }
        else if (gtk_dialog_run (GTK_DIALOG (dialog)) == GTK_RESPONSE_CANCEL)
        {
                gtk_widget_destroy (dialog);
        }

        return 0;
}



int main(int argc, char **argv) {
	
	GtkWidget *vbox;
	GtkWidget *menubar;
	GtkWidget *fileMenu;
	GtkWidget *fileMenu2;
	GtkWidget *fileMenu3;
	GtkWidget *fileMi;
	GtkWidget *quitMi;
	GtkWidget *romMi;
	GtkWidget *debugMi;
	GtkWidget *configMi;
	GtkWidget *romLoadMi;
	GtkWidget *setupConfMi;
	GtkWidget *setupConfMi2;
	GtkWidget *setupConfMi3;

	gtk_init(&argc, &argv);

	window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
 	gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);
  	gtk_window_set_default_size(GTK_WINDOW(window), 160, 144);
  	gtk_window_set_title(GTK_WINDOW(window), "PadraGB");

  	vbox = gtk_vbox_new(FALSE, 0);
  	gtk_container_add(GTK_CONTAINER(window), vbox);

  	menubar = gtk_menu_bar_new();
  	fileMenu = gtk_menu_new();
  	fileMenu2 = gtk_menu_new();
  	fileMenu3 = gtk_menu_new();

  	fileMi = gtk_menu_item_new_with_label("File");
  	romMi = gtk_menu_item_new_with_label("ROM");
  	configMi = gtk_menu_item_new_with_label("Config");
	debugMi = gtk_menu_item_new_with_label("Toggle Debug");

  	romLoadMi = gtk_menu_item_new_with_label("Load Rom");
  	setupConfMi2 = gtk_menu_item_new_with_label("1X Screen");
  	setupConfMi3 = gtk_menu_item_new_with_label("2X Screen");
  	setupConfMi = gtk_menu_item_new_with_label("4X Screen"); 
  	quitMi = gtk_menu_item_new_with_label("Quit");

  	gtk_menu_item_set_submenu(GTK_MENU_ITEM(fileMi), fileMenu);
  	gtk_menu_shell_append(GTK_MENU_SHELL(fileMenu), quitMi);
  	gtk_menu_shell_append(GTK_MENU_SHELL(menubar), fileMi);

  	gtk_menu_item_set_submenu(GTK_MENU_ITEM(romMi), fileMenu2);
  	gtk_menu_shell_append(GTK_MENU_SHELL(fileMenu2), romLoadMi);

  	gtk_menu_shell_append(GTK_MENU_SHELL(menubar), romMi);

  	gtk_menu_item_set_submenu(GTK_MENU_ITEM(configMi), fileMenu3);
	  gtk_menu_shell_append(GTK_MENU_SHELL(fileMenu3), debugMi);
  	gtk_menu_shell_append(GTK_MENU_SHELL(fileMenu3), setupConfMi2);
  	gtk_menu_shell_append(GTK_MENU_SHELL(fileMenu3), setupConfMi3);
  	gtk_menu_shell_append(GTK_MENU_SHELL(fileMenu3), setupConfMi);
  	gtk_menu_shell_append(GTK_MENU_SHELL(menubar), configMi);

  	gtk_box_pack_start(GTK_BOX(vbox), menubar, FALSE, FALSE, 0);

  	g_signal_connect(G_OBJECT(window), "destroy", G_CALLBACK(quit), NULL);
  	g_signal_connect(G_OBJECT(quitMi), "activate", G_CALLBACK(quit), NULL);
  	g_signal_connect(G_OBJECT(romLoadMi), "activate", G_CALLBACK(choose_rom_file), NULL);
  	g_signal_connect(G_OBJECT(setupConfMi2), "activate", G_CALLBACK(screen_mult1), NULL);
  	g_signal_connect(G_OBJECT(setupConfMi3), "activate", G_CALLBACK(screen_mult2), NULL);
  	g_signal_connect(G_OBJECT(setupConfMi), "activate", G_CALLBACK(screen_mult4), NULL);
  	g_signal_connect(G_OBJECT(debugMi), "activate", G_CALLBACK(toggle_debug), NULL);

  	//key press events
  	g_signal_connect(window, "key-press-event", G_CALLBACK(key_event), NULL);
 	g_signal_connect(window, "key-release-event", G_CALLBACK(key_release_event), NULL);
	icon = gdk_pixbuf_new_from_file("images.png", &error);//create_pixbuf("../images.png");  

	gtk_window_set_icon(window, icon);  	
  	g_object_unref(icon); 	
	gtk_widget_show_all(window);

  	gtk_main();
	return 0;

}


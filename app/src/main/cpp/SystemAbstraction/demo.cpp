#include "demo.hpp"
#include <math.h>
#include <stdio.h>

/* ===============================================================
 *
 *                          CUSTOM WIDGET
 *
 * ===============================================================*/
static int ui_piemenu(struct nk_context *ctx, struct nk_vec2 pos, float radius,
                      struct nk_image *icons, int item_count)
{
    int ret = -1;
    struct nk_rect total_space;
    struct nk_rect bounds;
    int active_item = 0;

    /* pie menu popup */
    struct nk_color border = ctx->style.window.border_color;
    struct nk_style_item background = ctx->style.window.fixed_background;
    ctx->style.window.fixed_background = nk_style_item_hide();
    ctx->style.window.border_color = nk_rgba(0,0,0,0);

    total_space  = nk_window_get_content_region(ctx);
    ctx->style.window.spacing = nk_vec2(0,0);
    ctx->style.window.padding = nk_vec2(0,0);

    if (nk_popup_begin(ctx, NK_POPUP_STATIC, "piemenu", NK_WINDOW_NO_SCROLLBAR,
                       nk_rect(pos.x - total_space.x - radius, pos.y - radius - total_space.y,
                               2*radius,2*radius)))
    {
        int i = 0;
        struct nk_command_buffer* out = nk_window_get_canvas(ctx);
        const struct nk_input *in = &ctx->input;

        total_space = nk_window_get_content_region(ctx);
        ctx->style.window.spacing = nk_vec2(4,4);
        ctx->style.window.padding = nk_vec2(8,8);
        nk_layout_row_dynamic(ctx, total_space.h, 1);
        nk_widget(&bounds, ctx);

        /* outer circle */
        nk_fill_circle(out, bounds, nk_rgb(50,50,50));
        {
            /* circle buttons */
            float step = (2 * 3.141592654f) / (float)(MAX(1,item_count));
            float a_min = 0; float a_max = step;

            struct nk_vec2 center = nk_vec2(bounds.x + bounds.w / 2.0f, bounds.y + bounds.h / 2.0f);
            struct nk_vec2 drag = nk_vec2(in->mouse.pos.x - center.x, in->mouse.pos.y - center.y);
            float angle = (float)atan2(drag.y, drag.x);
            if (angle < -0.0f) angle += 2.0f * 3.141592654f;
            active_item = (int)(angle/step);

            for (i = 0; i < item_count; ++i) {
                struct nk_rect content;
                float rx, ry, dx, dy, a;
                nk_fill_arc(out, center.x, center.y, (bounds.w/2.0f),
                            a_min, a_max, (active_item == i) ? nk_rgb(45,100,255): nk_rgb(60,60,60));

                /* separator line */
                rx = bounds.w/2.0f; ry = 0;
                dx = rx * (float)cos(a_min) - ry * (float)sin(a_min);
                dy = rx * (float)sin(a_min) + ry * (float)cos(a_min);
                nk_stroke_line(out, center.x, center.y,
                               center.x + dx, center.y + dy, 1.0f, nk_rgb(50,50,50));

                /* button content */
                a = a_min + (a_max - a_min)/2.0f;
                rx = bounds.w/2.5f; ry = 0;
                content.w = 30; content.h = 30;
                content.x = center.x + ((rx * (float)cos(a) - ry * (float)sin(a)) - content.w/2.0f);
                content.y = center.y + (rx * (float)sin(a) + ry * (float)cos(a) - content.h/2.0f);
                nk_draw_image(out, content, &icons[i], nk_rgb(255,255,255));
                a_min = a_max; a_max += step;
            }
        }
        {
            /* inner circle */
            struct nk_rect inner;
            inner.x = bounds.x + bounds.w/2 - bounds.w/4;
            inner.y = bounds.y + bounds.h/2 - bounds.h/4;
            inner.w = bounds.w/2; inner.h = bounds.h/2;
            nk_fill_circle(out, inner, nk_rgb(45,45,45));

            /* active icon content */
            bounds.w = inner.w / 2.0f;
            bounds.h = inner.h / 2.0f;
            bounds.x = inner.x + inner.w/2 - bounds.w/2;
            bounds.y = inner.y + inner.h/2 - bounds.h/2;
            nk_draw_image(out, bounds, &icons[active_item], nk_rgb(255,255,255));
        }
        nk_layout_space_end(ctx);
        if (!nk_input_is_mouse_down(&ctx->input, NK_BUTTON_RIGHT)) {
            nk_popup_close(ctx);
            ret = active_item;
        }
    } else ret = -2;
    ctx->style.window.spacing = nk_vec2(4,4);
    ctx->style.window.padding = nk_vec2(8,8);
    nk_popup_end(ctx);

    ctx->style.window.fixed_background = background;
    ctx->style.window.border_color = border;
    return ret;
}

/* ===============================================================
 *
 *                          GRID
 *
 * ===============================================================*/
void grid_demo(struct nk_context *ctx, struct media *media)
{
    static char text[3][64];
    static int text_len[3];
    static const char *items[] = {"Item 0","item 1","item 2"};
    static int selected_item = 0;
    static int check = 1;

    int i;
    nk_style_set_font(ctx, &media->font_20->handle);
    if (nk_begin(ctx, "Grid Demo", nk_rect(600, 350, 275, 250),
                 NK_WINDOW_TITLE|NK_WINDOW_BORDER|NK_WINDOW_MOVABLE|
                 NK_WINDOW_NO_SCROLLBAR))
    {
        nk_style_set_font(ctx, &media->font_18->handle);
        nk_layout_row_dynamic(ctx, 30, 2);
        nk_label(ctx, "Floating point:", NK_TEXT_RIGHT);
        nk_edit_string(ctx, NK_EDIT_FIELD, text[0], &text_len[0], 64, nk_filter_float);
        nk_label(ctx, "Hexadecimal:", NK_TEXT_RIGHT);
        nk_edit_string(ctx, NK_EDIT_FIELD, text[1], &text_len[1], 64, nk_filter_hex);
        nk_label(ctx, "Binary:", NK_TEXT_RIGHT);
        nk_edit_string(ctx, NK_EDIT_FIELD, text[2], &text_len[2], 64, nk_filter_binary);
        nk_label(ctx, "Checkbox:", NK_TEXT_RIGHT);
        nk_checkbox_label(ctx, "Check me", &check);
        nk_label(ctx, "Combobox:", NK_TEXT_RIGHT);
        if (nk_combo_begin_label(ctx, items[selected_item], nk_vec2(nk_widget_width(ctx), 200))) {
            nk_layout_row_dynamic(ctx, 25, 1);
            for (i = 0; i < 3; ++i)
                if (nk_combo_item_label(ctx, items[i], NK_TEXT_LEFT))
                    selected_item = i;
            nk_combo_end(ctx);
        }
    }
    nk_end(ctx);
    nk_style_set_font(ctx, &media->font_14->handle);
}

/* ===============================================================
 *
 *                          BUTTON DEMO
 *
 * ===============================================================*/
static void ui_header(struct nk_context *ctx, struct media *media, const char *title)
{
    nk_style_set_font(ctx, &media->font_18->handle);
    nk_layout_row_dynamic(ctx, 20, 1);
    nk_label(ctx, title, NK_TEXT_LEFT);
}

static void ui_widget(struct nk_context *ctx, struct media *media, float height)
{
    static const float ratio[] = {0.15f, 0.85f};
    nk_style_set_font(ctx, &media->font_22->handle);
    nk_layout_row(ctx, NK_DYNAMIC, height, 2, ratio);
    nk_spacing(ctx, 1);
}

static void ui_widget_centered(struct nk_context *ctx, struct media *media, float height)
{
    static const float ratio[] = {0.15f, 0.50f, 0.35f};
    nk_style_set_font(ctx, &media->font_22->handle);
    nk_layout_row(ctx, NK_DYNAMIC, height, 3, ratio);
    nk_spacing(ctx, 1);
}

void button_demo(struct nk_context *ctx, struct media *media)
{
    static int option = 1;
    static int toggle0 = 1;
    static int toggle1 = 0;
    static int toggle2 = 1;

    nk_style_set_font(ctx, &media->font_20->handle);
    nk_begin(ctx, "Button Demo", nk_rect(50,50,255,610),
             NK_WINDOW_BORDER|NK_WINDOW_MOVABLE|NK_WINDOW_SCALABLE|
             NK_WINDOW_TITLE);

    /*------------------------------------------------
     *                  MENU
     *------------------------------------------------*/
    nk_menubar_begin(ctx);
    {
        /* toolbar */
        nk_layout_row_static(ctx, 40, 40, 4);
        if (nk_menu_begin_image(ctx, "Music", media->play, nk_vec2(110,120)))
        {
            /* settings */
            nk_layout_row_dynamic(ctx, 25, 1);
            nk_menu_item_image_label(ctx, media->play, "Play", NK_TEXT_RIGHT);
            nk_menu_item_image_label(ctx, media->stop, "Stop", NK_TEXT_RIGHT);
            nk_menu_item_image_label(ctx, media->pause, "Pause", NK_TEXT_RIGHT);
            nk_menu_item_image_label(ctx, media->next, "Next", NK_TEXT_RIGHT);
            nk_menu_item_image_label(ctx, media->prev, "Prev", NK_TEXT_RIGHT);
            nk_menu_end(ctx);
        }
        nk_button_image(ctx, media->tools);
        nk_button_image(ctx, media->cloud);
        nk_button_image(ctx, media->pen);
    }
    nk_menubar_end(ctx);

    /*------------------------------------------------
     *                  BUTTON
     *------------------------------------------------*/
    ui_header(ctx, media, "Push buttons");
    ui_widget(ctx, media, 35);
    if (nk_button_label(ctx, "Push me"))
        fprintf(stdout, "pushed!\n");
    ui_widget(ctx, media, 35);
    if (nk_button_image_label(ctx, media->rocket, "Styled", NK_TEXT_CENTERED))
        fprintf(stdout, "rocket!\n");

    /*------------------------------------------------
     *                  REPEATER
     *------------------------------------------------*/
    ui_header(ctx, media, "Repeater");
    ui_widget(ctx, media, 35);
    if (nk_button_label(ctx, "Press me"))
        fprintf(stdout, "pressed!\n");

    /*------------------------------------------------
     *                  TOGGLE
     *------------------------------------------------*/
    ui_header(ctx, media, "Toggle buttons");
    ui_widget(ctx, media, 35);
    if (nk_button_image_label(ctx, (toggle0) ? media->checked: media->unchecked, "Toggle", NK_TEXT_LEFT))
        toggle0 = !toggle0;

    ui_widget(ctx, media, 35);
    if (nk_button_image_label(ctx, (toggle1) ? media->checked: media->unchecked, "Toggle", NK_TEXT_LEFT))
        toggle1 = !toggle1;

    ui_widget(ctx, media, 35);
    if (nk_button_image_label(ctx, (toggle2) ? media->checked: media->unchecked, "Toggle", NK_TEXT_LEFT))
        toggle2 = !toggle2;

    /*------------------------------------------------
     *                  RADIO
     *------------------------------------------------*/
    ui_header(ctx, media, "Radio buttons");
    ui_widget(ctx, media, 35);
    if (nk_button_symbol_label(ctx, (option == 0)?NK_SYMBOL_CIRCLE_OUTLINE:NK_SYMBOL_CIRCLE_SOLID, "Select", NK_TEXT_LEFT))
        option = 0;
    ui_widget(ctx, media, 35);
    if (nk_button_symbol_label(ctx, (option == 1)?NK_SYMBOL_CIRCLE_OUTLINE:NK_SYMBOL_CIRCLE_SOLID, "Select", NK_TEXT_LEFT))
        option = 1;
    ui_widget(ctx, media, 35);
    if (nk_button_symbol_label(ctx, (option == 2)?NK_SYMBOL_CIRCLE_OUTLINE:NK_SYMBOL_CIRCLE_SOLID, "Select", NK_TEXT_LEFT))
        option = 2;

    /*------------------------------------------------
     *                  CONTEXTUAL
     *------------------------------------------------*/
    nk_style_set_font(ctx, &media->font_18->handle);
    if (nk_contextual_begin(ctx, NK_WINDOW_NO_SCROLLBAR, nk_vec2(150, 300), nk_window_get_bounds(ctx))) {
        nk_layout_row_dynamic(ctx, 30, 1);
        if (nk_contextual_item_image_label(ctx, media->copy, "Clone", NK_TEXT_RIGHT))
            fprintf(stdout, "pressed clone!\n");
        if (nk_contextual_item_image_label(ctx, media->del, "Delete", NK_TEXT_RIGHT))
            fprintf(stdout, "pressed delete!\n");
        if (nk_contextual_item_image_label(ctx, media->convert, "Convert", NK_TEXT_RIGHT))
            fprintf(stdout, "pressed convert!\n");
        if (nk_contextual_item_image_label(ctx, media->edit, "Edit", NK_TEXT_RIGHT))
            fprintf(stdout, "pressed edit!\n");
        nk_contextual_end(ctx);
    }
    nk_style_set_font(ctx, &media->font_14->handle);
    nk_end(ctx);
}

/* ===============================================================
 *
 *                          BASIC DEMO
 *
 * ===============================================================*/
void basic_demo(struct nk_context *ctx, struct media *media)
{
    static int image_active;
    static int check0 = 1;
    static int check1 = 0;
    static size_t prog = 80;
    static int selected_item = 0;
    static int selected_image = 3;
    static int selected_icon = 0;
    static const char *items[] = {"Item 0","item 1","item 2"};
    static int piemenu_active = 0;
    static struct nk_vec2 piemenu_pos;

    int i = 0;
    nk_style_set_font(ctx, &media->font_20->handle);
    nk_begin(ctx, "Basic Demo", nk_rect(320, 50, 275, 610),
             NK_WINDOW_BORDER|NK_WINDOW_MOVABLE|NK_WINDOW_TITLE);

    /*------------------------------------------------
     *                  POPUP BUTTON
     *------------------------------------------------*/
    ui_header(ctx, media, "Popup & Scrollbar & Images");
    ui_widget(ctx, media, 35);
    if (nk_button_image_label(ctx, media->dir, "Images", NK_TEXT_CENTERED))
        image_active = !image_active;

    /*------------------------------------------------
     *                  SELECTED IMAGE
     *------------------------------------------------*/
    ui_header(ctx, media, "Selected Image");
    ui_widget_centered(ctx, media, 100);
    nk_image(ctx, media->images[selected_image]);

    /*------------------------------------------------
     *                  IMAGE POPUP
     *------------------------------------------------*/
    if (image_active) {
        struct nk_panel popup;
        if (nk_popup_begin(ctx, NK_POPUP_STATIC, "Image Popup", 0, nk_rect(265, 0, 320, 220))) {
            nk_layout_row_static(ctx, 82, 82, 3);
            for (i = 0; i < 9; ++i) {
                if (nk_button_image(ctx, media->images[i])) {
                    selected_image = i;
                    image_active = 0;
                    nk_popup_close(ctx);
                }
            }
            nk_popup_end(ctx);
        }
    }
    /*------------------------------------------------
     *                  COMBOBOX
     *------------------------------------------------*/
    ui_header(ctx, media, "Combo box");
    ui_widget(ctx, media, 40);
    if (nk_combo_begin_label(ctx, items[selected_item], nk_vec2(nk_widget_width(ctx), 200))) {
        nk_layout_row_dynamic(ctx, 35, 1);
        for (i = 0; i < 3; ++i)
            if (nk_combo_item_label(ctx, items[i], NK_TEXT_LEFT))
                selected_item = i;
        nk_combo_end(ctx);
    }

    ui_widget(ctx, media, 40);
    if (nk_combo_begin_image_label(ctx, items[selected_icon], media->images[selected_icon], nk_vec2(nk_widget_width(ctx), 200))) {
        nk_layout_row_dynamic(ctx, 35, 1);
        for (i = 0; i < 3; ++i)
            if (nk_combo_item_image_label(ctx, media->images[i], items[i], NK_TEXT_RIGHT))
                selected_icon = i;
        nk_combo_end(ctx);
    }

    /*------------------------------------------------
     *                  CHECKBOX
     *------------------------------------------------*/
    ui_header(ctx, media, "Checkbox");
    ui_widget(ctx, media, 30);
    nk_checkbox_label(ctx, "Flag 1", &check0);
    ui_widget(ctx, media, 30);
    nk_checkbox_label(ctx, "Flag 2", &check1);

    /*------------------------------------------------
     *                  PROGRESSBAR
     *------------------------------------------------*/
    ui_header(ctx, media, "Progressbar");
    ui_widget(ctx, media, 35);
    nk_progress(ctx, &prog, 100, nk_true);

    /*------------------------------------------------
     *                  PIEMENU
     *------------------------------------------------*/
    if (nk_input_is_mouse_click_down_in_rect(&ctx->input, NK_BUTTON_RIGHT,
                                             nk_window_get_bounds(ctx),nk_true)){
        piemenu_pos = ctx->input.mouse.pos;
        piemenu_active = 1;
    }

    if (piemenu_active) {
        int ret = ui_piemenu(ctx, piemenu_pos, 140, &media->menu[0], 6);
        if (ret == -2) piemenu_active = 0;
        if (ret != -1) {
            fprintf(stdout, "piemenu selected: %d\n", ret);
            piemenu_active = 0;
        }
    }
    nk_style_set_font(ctx, &media->font_14->handle);
    nk_end(ctx);
}

struct device device;
struct nk_font_atlas atlas;
struct media media;
struct nk_context ctx;

void demo_init(int width, int height)
{
    glViewport(0, 0, width, height);


    {/* GUI */
        device_init(&device);
        {
            const void *image; int w, h;
            struct nk_font_config cfg = nk_font_config(0);
            cfg.oversample_h = 3; cfg.oversample_v = 2;
            // Loading one font with different heights is only required if you want higher
            // quality text otherwise you can just set the font height directly
            // e.g.: ctx->style.font.height = 20.
            nk_font_atlas_init_default(&atlas);
            nk_font_atlas_begin(&atlas);

            //            media.font_14 = nk_font_atlas_add_from_file(&atlas, "./data/extra_font/Roboto-Regular.ttf", 14.0f, &cfg);
            //            media.font_18 = nk_font_atlas_add_from_file(&atlas, "./data/extra_font/Roboto-Regular.ttf", 18.0f, &cfg);
            //            media.font_20 = nk_font_atlas_add_from_file(&atlas, "./data/extra_font/Roboto-Regular.ttf", 20.0f, &cfg);
            //            media.font_22 = nk_font_atlas_add_from_file(&atlas, "./data/extra_font/Roboto-Regular.ttf", 22.0f, &cfg);

#           include "./data_headers/extra_font/Roboto-Regular.ttf.hpp"
            media.font_14 = nk_font_atlas_add_from_memory(&atlas, Roboto_Regular, size_of_Roboto_Regular, 14.0f, &cfg);
            media.font_18 = nk_font_atlas_add_from_memory(&atlas, Roboto_Regular, size_of_Roboto_Regular, 18.0f, &cfg);
            media.font_20 = nk_font_atlas_add_from_memory(&atlas, Roboto_Regular, size_of_Roboto_Regular, 20.0f, &cfg);
            media.font_22 = nk_font_atlas_add_from_memory(&atlas, Roboto_Regular, size_of_Roboto_Regular, 22.0f, &cfg);

            image = nk_font_atlas_bake(&atlas, &w, &h, NK_FONT_ATLAS_RGBA32);
            device_upload_atlas(&device, image, w, h);
            nk_font_atlas_end(&atlas, nk_handle_id((int)device.font_tex), &device.null);
        }
        nk_init_default(&ctx, &media.font_14->handle);
    }

    /* icons */
    glEnable(GL_TEXTURE_2D);


    //    media.unchecked = icon_load("./data/icon/unchecked.png");
    //    media.checked = icon_load("./data/icon/checked.png");
    //    media.rocket = icon_load("./data/icon/rocket.png");
    //    media.cloud = icon_load("./data/icon/cloud.png");
    //    media.pen = icon_load("./data/icon/pen.png");
    //    media.play = icon_load("./data/icon/play.png");
    //    media.pause = icon_load("./data/icon/pause.png");
    //    media.stop = icon_load("./data/icon/stop.png");
    //    media.next =  icon_load("./data/icon/next.png");
    //    media.prev =  icon_load("./data/icon/prev.png");
    //    media.tools = icon_load("./data/icon/tools.png");
    //    media.dir = icon_load("./data/icon/directory.png");
    //    media.copy = icon_load("./data/icon/copy.png");
    //    media.convert = icon_load("./data/icon/export.png");
    //    media.del = icon_load("./data/icon/delete.png");
    //    media.edit = icon_load("./data/icon/edit.png");
    //    media.menu[0] = icon_load("./data/icon/home.png");
    //    media.menu[1] = icon_load("./data/icon/phone.png");
    //    media.menu[2] = icon_load("./data/icon/plane.png");
    //    media.menu[3] = icon_load("./data/icon/wifi.png");
    //    media.menu[4] = icon_load("./data/icon/settings.png");
    //    media.menu[5] = icon_load("./data/icon/volume.png");

#include "./data_headers/icon/unchecked.png.hpp"
#include "./data_headers/icon/checked.png.hpp"
#include "./data_headers/icon/rocket.png.hpp"
#include "./data_headers/icon/cloud.png.hpp"
#include "./data_headers/icon/pen.png.hpp"
#include "./data_headers/icon/play.png.hpp"
#include "./data_headers/icon/pause.png.hpp"
#include "./data_headers/icon/stop.png.hpp"
#include "./data_headers/icon/next.png.hpp"
#include "./data_headers/icon/prev.png.hpp"
#include "./data_headers/icon/tools.png.hpp"
#include "./data_headers/icon/directory.png.hpp"
#include "./data_headers/icon/copy.png.hpp"
#include "./data_headers/icon/export.png.hpp"
#include "./data_headers/icon/delete.png.hpp"
#include "./data_headers/icon/edit.png.hpp"
#include "./data_headers/icon/home.png.hpp"
#include "./data_headers/icon/phone.png.hpp"
#include "./data_headers/icon/plane.png.hpp"
#include "./data_headers/icon/wifi.png.hpp"
#include "./data_headers/icon/settings.png.hpp"
#include "./data_headers/icon/volume.png.hpp"

    media.unchecked = icon_load_from_memory(unchecked,size_of_unchecked);
    media.checked = icon_load_from_memory(checked,size_of_checked);
    media.rocket = icon_load_from_memory(rocket,size_of_rocket);
    media.cloud = icon_load_from_memory(cloud,size_of_cloud);
    media.pen = icon_load_from_memory(pen,size_of_pen);
    media.play = icon_load_from_memory(play,size_of_play);
    media.pause = icon_load_from_memory(pause,size_of_pause);
    media.stop = icon_load_from_memory(stop,size_of_stop);
    media.next =  icon_load_from_memory(next,size_of_next);
    media.prev =  icon_load_from_memory(prev,size_of_prev);
    media.tools = icon_load_from_memory(tools,size_of_tools);
    media.dir = icon_load_from_memory(directory,size_of_directory);
    media.copy = icon_load_from_memory(copy,size_of_copy);
    media.convert = icon_load_from_memory(export_png,size_of_export_png);
    media.del = icon_load_from_memory(delete_png,size_of_delete_png);
    media.edit = icon_load_from_memory(edit,size_of_edit);
    media.menu[0] = icon_load_from_memory(home,size_of_home);
    media.menu[1] = icon_load_from_memory(phone,size_of_phone);
    media.menu[2] = icon_load_from_memory(plane,size_of_plane);
    media.menu[3] = icon_load_from_memory(wifi,size_of_wifi);
    media.menu[4] = icon_load_from_memory(settings,size_of_settings);
    media.menu[5] = icon_load_from_memory(volume,size_of_volume);


    //    for (int i = 0; i < 9; ++i)
    //    {
    //        char buffer[256];
    //        sprintf(buffer, "./data/images/image%d.png", (i+1));
    //        media.images[i] = icon_load(buffer);
    //    }


    //    media.images[1] = icon_load("./data/images/image1.png");
    //    media.images[2] = icon_load("./data/images/image2.png");
    //    media.images[3] = icon_load("./data/images/image3.png");
    //    media.images[4] = icon_load("./data/images/image4.png");
    //    media.images[5] = icon_load("./data/images/image5.png");
    //    media.images[6] = icon_load("./data/images/image6.png");
    //    media.images[7] = icon_load("./data/images/image7.png");
    //    media.images[8] = icon_load("./data/images/image8.png");
    //    media.images[9] = icon_load("./data/images/image9.png");


#include "./data_headers/images/image1.png.hpp"
#include "./data_headers/images/image2.png.hpp"
#include "./data_headers/images/image3.png.hpp"
#include "./data_headers/images/image4.png.hpp"
#include "./data_headers/images/image5.png.hpp"
#include "./data_headers/images/image6.png.hpp"
#include "./data_headers/images/image7.png.hpp"
#include "./data_headers/images/image8.png.hpp"
#include "./data_headers/images/image9.png.hpp"

    media.images[0] = icon_load_from_memory(image1, size_of_image1);
    media.images[1] = icon_load_from_memory(image2, size_of_image2);
    media.images[2] = icon_load_from_memory(image3, size_of_image3);
    media.images[3] = icon_load_from_memory(image4, size_of_image4);
    media.images[4] = icon_load_from_memory(image5, size_of_image5);
    media.images[5] = icon_load_from_memory(image6, size_of_image6);
    media.images[6] = icon_load_from_memory(image7, size_of_image7);
    media.images[7] = icon_load_from_memory(image8, size_of_image8);
    media.images[8] = icon_load_from_memory(image9, size_of_image9);
}

void demo_onCharCallback(unsigned int codepoint)
{
        nk_input_unicode(&ctx, codepoint);
}

void demo_onPointerMoveCallback(int pointerId, const struct PointerCoords *coords)
{
    nk_input_motion(&ctx, (int)coords->x, (int)coords->y);
}

void demo_onScrollCallback(double yoffset)
{
    nk_input_scroll(&ctx, nk_vec2(0, (float)yoffset));
}

void demo_onMouseButtonCallback(SystemAbstraction::MouseButton mouseButton, SystemAbstraction::ButtonEvent event, int x, int y)
{
    nk_input_button(&ctx, NK_BUTTON_LEFT, (int)x, (int)y, ((mouseButton == SystemAbstraction::MOUSE_LEFT_BUTTON) && (event == SystemAbstraction::EVENT_DOWN)) ? 1 : 0 );
    nk_input_button(&ctx, NK_BUTTON_MIDDLE, (int)x, (int)y, ((mouseButton == SystemAbstraction::MOUSE_MIDDLE_BUTTON) && (event == SystemAbstraction::EVENT_DOWN)) ? 1 : 0 );
    nk_input_button(&ctx, NK_BUTTON_RIGHT, (int)x, (int)y, ((mouseButton == SystemAbstraction::MOUSE_RIGHT_BUTTON) && (event == SystemAbstraction::EVENT_DOWN)) ? 1 : 0 );
}

void demo_onKeyCallback(SystemAbstraction::ButtonEvent event, SystemAbstraction::Key key, SystemAbstraction::Mods mods, int x, int y)
{
    nk_input_key(&ctx, NK_KEY_DEL, ((key == SystemAbstraction::KEY_DELETE) && (event == SystemAbstraction::EVENT_DOWN)) ? 1 : 0 );
    nk_input_key(&ctx, NK_KEY_ENTER, ((key == SystemAbstraction::KEY_ENTER) && (event == SystemAbstraction::EVENT_DOWN)) ? 1 : 0 );
    nk_input_key(&ctx, NK_KEY_TAB, ((key == SystemAbstraction::KEY_TAB) && (event == SystemAbstraction::EVENT_DOWN)) ? 1 : 0 );
    nk_input_key(&ctx, NK_KEY_BACKSPACE, ((key == SystemAbstraction::KEY_BACKSPACE) && (event == SystemAbstraction::EVENT_DOWN)) ? 1 : 0 );
    nk_input_key(&ctx, NK_KEY_LEFT, ((key == SystemAbstraction::KEY_LEFT) && (event == SystemAbstraction::EVENT_DOWN)) ? 1 : 0 );
    nk_input_key(&ctx, NK_KEY_RIGHT, ((key == SystemAbstraction::KEY_RIGHT) && (event == SystemAbstraction::EVENT_DOWN)) ? 1 : 0 );
    nk_input_key(&ctx, NK_KEY_UP, ((key == SystemAbstraction::KEY_UP) && (event == SystemAbstraction::EVENT_DOWN)) ? 1 : 0 );
    nk_input_key(&ctx, NK_KEY_DOWN, ((key == SystemAbstraction::KEY_DOWN) && (event == SystemAbstraction::EVENT_DOWN)) ? 1 : 0 );



    if (mods == SystemAbstraction::MOD_CONTROL)
    {
        nk_input_key(&ctx, NK_KEY_COPY, ((key == SystemAbstraction::KEY_C) && (event == SystemAbstraction::EVENT_DOWN)) ? 1 : 0 );
        nk_input_key(&ctx, NK_KEY_PASTE, ((key == SystemAbstraction::KEY_V) && (event == SystemAbstraction::EVENT_DOWN)) ? 1 : 0 );
        nk_input_key(&ctx, NK_KEY_CUT, ((key == SystemAbstraction::KEY_X) && (event == SystemAbstraction::EVENT_DOWN)) ? 1 : 0 );
        nk_input_key(&ctx, NK_KEY_CUT, ((key == SystemAbstraction::KEY_E) && (event == SystemAbstraction::EVENT_DOWN)) ? 1 : 0 );
        nk_input_key(&ctx, NK_KEY_SHIFT, 1);
    }
    else
    {
        nk_input_key(&ctx, NK_KEY_COPY, 0);
        nk_input_key(&ctx, NK_KEY_PASTE, 0);
        nk_input_key(&ctx, NK_KEY_CUT, 0);
        nk_input_key(&ctx, NK_KEY_SHIFT, 0);
    }
}

void demo_render(int fb_width, int fb_height)
{
    nk_input_end(&ctx);

    /* GUI */
    basic_demo(&ctx, &media);
    button_demo(&ctx, &media);
    grid_demo(&ctx, &media);

    /* Draw */
    glViewport(0, 0, fb_width, fb_height);
    glClear(GL_COLOR_BUFFER_BIT);
    glClearColor(0.3f, 0.3f, 0.3f, 1.0f);
    device_draw(&device, &ctx, fb_width, fb_height, NK_ANTI_ALIASING_ON);

    nk_input_begin(&ctx);
}

void demo_uninit()
{
    glDeleteTextures(1,(const GLuint*)&media.unchecked.handle.id);
    glDeleteTextures(1,(const GLuint*)&media.checked.handle.id);
    glDeleteTextures(1,(const GLuint*)&media.rocket.handle.id);
    glDeleteTextures(1,(const GLuint*)&media.cloud.handle.id);
    glDeleteTextures(1,(const GLuint*)&media.pen.handle.id);
    glDeleteTextures(1,(const GLuint*)&media.play.handle.id);
    glDeleteTextures(1,(const GLuint*)&media.pause.handle.id);
    glDeleteTextures(1,(const GLuint*)&media.stop.handle.id);
    glDeleteTextures(1,(const GLuint*)&media.next.handle.id);
    glDeleteTextures(1,(const GLuint*)&media.prev.handle.id);
    glDeleteTextures(1,(const GLuint*)&media.tools.handle.id);
    glDeleteTextures(1,(const GLuint*)&media.dir.handle.id);
    glDeleteTextures(1,(const GLuint*)&media.del.handle.id);

    nk_font_atlas_clear(&atlas);
    nk_free(&ctx);

    device_shutdown(&device);
}

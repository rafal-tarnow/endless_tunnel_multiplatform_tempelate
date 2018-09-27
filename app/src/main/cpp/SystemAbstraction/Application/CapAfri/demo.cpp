#include "demo.hpp"
#include <math.h>
#include <stdio.h>
#include "system_log.hpp"
#include <string>
#include <library_opengles_2/TextureManager/texture_manager.hpp>
#include <library_opengles_2/Resources/Resources.hpp>

using namespace std;

/* ===============================================================
 *
 *                          CUSTOM WIDGET
 *
 * ===============================================================*/

struct nk_font *font_14;
struct nk_font *font_18;
struct nk_font *font_20;
struct nk_font *font_22;
struct nk_font *font_30;
struct nk_image images[5];
struct nk_image next_png;
struct nk_image prev_png;
struct nk_image plus_png;
struct nk_image minus_png;


struct backend_device backend_device;
struct nk_font_atlas atlas;
struct nk_context ctx;

static MapEditorGuiEventListener * toolboxEventListener = nullptr;

static string mCursorMode = "CM:";
static string mFantMode = "FM:";
static float scale = 1.0f;

void demo_setCursorModeText_dbg(string cursorMode)
{
    mCursorMode = /*"CM: " +*/ cursorMode;
}
void demo_setFantModeText_dbg(string fantMode)
{
    mFantMode = /*"FM: " +*/ fantMode;
}

/* ===============================================================
 *
 *                          BUTTON DEMO
 *
 * ===============================================================*/
static void ui_header(struct nk_context *ctx, const char *title)
{
    nk_style_set_font(ctx, &font_22->handle);
    nk_layout_row_dynamic(ctx, 20*scale, 1);
    nk_label(ctx, title, NK_TEXT_CENTERED);
}

static void ui_widget(struct nk_context *ctx, float height)
{
    static const float ratio[] = {0.15f, 0.85f};
    nk_style_set_font(ctx, &font_22->handle);
    nk_layout_row(ctx, NK_DYNAMIC, height, 2, ratio);
    nk_spacing(ctx, 1);
}



void toolbox_demo(struct nk_context *ctx)
{
    static unsigned int prog_value = 0;
    static const char *items[] = {"Ground","Coin","Mushroom","Meta","Move"};
    static int selected_icon = 0;
    int i = 0;

    nk_style_set_font(ctx, &font_30->handle);

    nk_begin(ctx, "Toolbox", nk_rect(0,0,360.0f*scale,630.0f*scale),NK_WINDOW_BORDER|/* NK_WINDOW_SCALABLE | NK_WINDOW_MOVABLE |*/ NK_WINDOW_TITLE);
    {
        nk_layout_row_dynamic(ctx, 20*scale, 1);
        nk_label(ctx, mCursorMode.c_str(), NK_TEXT_LEFT);

        nk_layout_row_dynamic(ctx, 20*scale, 1);
        nk_label(ctx, mFantMode.c_str(), NK_TEXT_LEFT);


        ui_header(ctx, "Cursor Mode");

        ui_widget(ctx, 40*scale);
        if (nk_combo_begin_image_label(ctx, items[selected_icon], images[selected_icon], nk_vec2(nk_widget_width(ctx), 200*scale))) {
            nk_layout_row_dynamic(ctx, 35*scale, 1);
            for (i = 0; i < 5; ++i)
                if (nk_combo_item_image_label(ctx, images[i], items[i], NK_TEXT_RIGHT))
                {
                    selected_icon = i;
                    if(toolboxEventListener != nullptr)
                    {
                        toolboxEventListener->gui_onCursorModeChanged(selected_icon);
                    }
                }
            nk_combo_end(ctx);
        }


        ui_header(ctx, "Select map to edit:");

        nk_layout_row_template_begin(ctx, 80*scale);
        {
            nk_layout_row_template_push_static(ctx, 80*scale);
            nk_layout_row_template_push_variable(ctx, 80*scale);
            nk_layout_row_template_push_static(ctx, 80*scale);
        }
        nk_layout_row_template_end(ctx);

        if(nk_button_image(ctx, prev_png))
        {
            if(prog_value > 0)
                prog_value--;
            if(toolboxEventListener != nullptr) {
                toolboxEventListener->gui_onCurrentMapChanged(prog_value);
            }
        }
        nk_labelf(ctx, NK_TEXT_CENTERED, "Map to edit: %zu" , prog_value + 1);
        if(nk_button_image(ctx, next_png))
        {
            if(prog_value < 39)
                prog_value++;
            if(toolboxEventListener != nullptr) {
                toolboxEventListener->gui_onCurrentMapChanged(prog_value);
            }
        }


        static const float ratio[] = {0.0f, 1.0f};
        nk_style_set_font(ctx, &font_30->handle);

        nk_layout_row(ctx, NK_DYNAMIC, 105*scale, 2, ratio);
        nk_spacing(ctx, 1);

        if (nk_button_label(ctx, "Save map"))
        {
            printf("Save map clicked!\n");
            if(toolboxEventListener != nullptr) {
                toolboxEventListener->gui_onSaveMapButtonClicked();
            }
        }

        nk_layout_row(ctx, NK_DYNAMIC, 105*scale, 2, ratio);
        nk_spacing(ctx, 1);

        if (nk_button_label(ctx, "Clear map"))
        {
            printf("Clear map clicked!\n");
            if(toolboxEventListener != nullptr) {
                toolboxEventListener->gui_onClearMapButtonClicked();
            }
        }



        ui_header(ctx, "ZOOM");

        nk_layout_row_template_begin(ctx, 80*scale);
        {
            nk_layout_row_template_push_static(ctx, 80*scale);
            nk_layout_row_template_push_variable(ctx, 80*scale);
            nk_layout_row_template_push_static(ctx, 80*scale);
        }
        nk_layout_row_template_end(ctx);


        if(nk_button_image(ctx, minus_png))
        {
            if(toolboxEventListener != nullptr) {
                toolboxEventListener->gui_onZoomOut();
            }
        }
        nk_labelf(ctx, NK_TEXT_CENTERED, "-    +" , prog_value + 1);
        if(nk_button_image(ctx, plus_png))
        {
            if(toolboxEventListener != nullptr) {
                toolboxEventListener->gui_onZoomIn();
            }
        }

    }
    nk_end(ctx);
}

struct nk_image icon_load_from_TextureManager(string fileName)
{
    GLuint tex = TextureManager::getInstance()->getTextureId(fileName);
    return nk_image_id((int)tex);
}


void prepare_font_atlas()
{
    const void *image; int w, h;
    struct nk_font_config cfg = nk_font_config(0);
    cfg.oversample_h = 3; cfg.oversample_v = 2;
    // Loading one font with different heights is only required if you want higher
    // quality text otherwise you can just set the font height directly
    // e.g.: ctx->style.font.height = 20.
    nk_font_atlas_init_default(&atlas);
    nk_font_atlas_begin(&atlas);

    Resource robotoRegular("fonts/Roboto-Regular.ttf");


    font_14 = nk_font_atlas_add_from_memory(&atlas, robotoRegular.getData(), robotoRegular.getSize(), 14.0f*scale, &cfg);
    font_18 = nk_font_atlas_add_from_memory(&atlas, robotoRegular.getData(), robotoRegular.getSize(), 18.0f*scale, &cfg);
    font_20 = nk_font_atlas_add_from_memory(&atlas, robotoRegular.getData(), robotoRegular.getSize(), 20.0f*scale, &cfg);
    font_22 = nk_font_atlas_add_from_memory(&atlas, robotoRegular.getData(), robotoRegular.getSize(), 22.0f*scale, &cfg);
    font_30 = nk_font_atlas_add_from_memory(&atlas, robotoRegular.getData(), robotoRegular.getSize(), 30.0f*scale, &cfg);


    image = nk_font_atlas_bake(&atlas, &w, &h, NK_FONT_ATLAS_RGBA32);
    backend_upload_atlas(&backend_device, image, w, h);
    nk_font_atlas_end(&atlas, nk_handle_id((int)backend_device.font_tex), &backend_device.null);
}

void demo_init(int width, int height, float mscale)
{
    scale = mscale;
    glViewport(0, 0, width, height);


    backend_init(&backend_device);
    prepare_font_atlas();
    nk_init_default(&ctx, &font_14->handle);


    /* icons */
    glEnable(GL_TEXTURE_2D);

    images[0] = icon_load_from_TextureManager("textures/red_dot.png");
    images[1] = icon_load_from_TextureManager("textures/coin_2.png");
    images[2] = icon_load_from_TextureManager("textures/Tango_Style_Mushroom_icon.svg.png");
    images[3] = icon_load_from_TextureManager("textures/meta.png");
    images[4] = icon_load_from_TextureManager("textures/move.png");
    prev_png = icon_load_from_TextureManager("textures/prev.png");
    next_png = icon_load_from_TextureManager("textures/next.png");
    plus_png = icon_load_from_TextureManager("textures/plus_grey.png");
    minus_png = icon_load_from_TextureManager("textures/minus_grey.png");
}

void demo_setScale(int width, int height, float mscale)
{
    scale = mscale;
    glViewport(0, 0, width, height);

    prepare_font_atlas();

    LOGD("scale = %f\n", scale);
}

struct nk_context * demo_getContext()
{
    return &ctx;
}

void demo_onCharCallback(unsigned int codepoint)
{
    nk_input_unicode(&ctx, codepoint);
}

void demo_onScrollCallback(double yoffset)
{
    nk_input_scroll(&ctx, nk_vec2(0, (float)yoffset));
}

void demo_onMouseButtonCallback(SystemAbstraction::MouseButton mouseButton, SystemAbstraction::ButtonEvent event, int x, int y)
{
    //LOGD("demo_onMouseButtonCallback x = %d y = %d", x, y);

    //TouchEvent Support
    struct nk_input *in;
    in = &ctx.input;
    in->mouse.pos.x = (float)x;
    in->mouse.pos.y = (float)y;
    in->mouse.prev.x = (float)x;
    in->mouse.prev.y = (float)y;
    in->mouse.delta.x = 0;
    in->mouse.delta.y = 0;


    nk_input_button(&ctx, NK_BUTTON_LEFT, (int)x, (int)y, ((mouseButton == SystemAbstraction::MOUSE_LEFT_BUTTON) && (event == SystemAbstraction::EVENT_DOWN)) ? 1 : 0 );
    nk_input_button(&ctx, NK_BUTTON_MIDDLE, (int)x, (int)y, ((mouseButton == SystemAbstraction::MOUSE_MIDDLE_BUTTON) && (event == SystemAbstraction::EVENT_DOWN)) ? 1 : 0 );
    nk_input_button(&ctx, NK_BUTTON_RIGHT, (int)x, (int)y, ((mouseButton == SystemAbstraction::MOUSE_RIGHT_BUTTON) && (event == SystemAbstraction::EVENT_DOWN)) ? 1 : 0 );
}

void demo_onMouseMoveCallcack(int x, int y)
{
    nk_input_motion(&ctx, x, y);
}

void demo_onPointerMoveCallback(int pointerId, const struct PointerCoords *coords)
{
    nk_input_motion(&ctx, (int)coords->x, (int)coords->y);
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

void mapEditorGui_setEventListener(MapEditorGuiEventListener *eventListener)
{
    toolboxEventListener = eventListener;
}

int mapEditorGui_isAnyWindowHovered()
{
    return nk_window_is_any_hovered(&ctx);
}


void mapEditorGui_render(int fb_width, int fb_height)
{
    nk_input_end(&ctx);

    toolbox_demo(&ctx);

    //select_map_window(&ctx);

    backend_device_draw(&backend_device, &ctx, fb_width, fb_height, NK_ANTI_ALIASING_ON);

    nk_input_begin(&ctx);
}


void demo_uninit()
{
    nk_font_atlas_clear(&atlas);
    nk_free(&ctx);

    backdend_shutdown(&backend_device);
}


void set_style(struct nk_context *ctx, enum theme theme)
{
    struct nk_color table[NK_COLOR_COUNT];
    if (theme == THEME_WHITE) {
        table[NK_COLOR_TEXT] = nk_rgba(70, 70, 70, 255);
        table[NK_COLOR_WINDOW] = nk_rgba(175, 175, 175, 255);
        table[NK_COLOR_HEADER] = nk_rgba(175, 175, 175, 255);
        table[NK_COLOR_BORDER] = nk_rgba(0, 0, 0, 255);
        table[NK_COLOR_BUTTON] = nk_rgba(185, 185, 185, 255);
        table[NK_COLOR_BUTTON_HOVER] = nk_rgba(170, 170, 170, 255);
        table[NK_COLOR_BUTTON_ACTIVE] = nk_rgba(160, 160, 160, 255);
        table[NK_COLOR_TOGGLE] = nk_rgba(150, 150, 150, 255);
        table[NK_COLOR_TOGGLE_HOVER] = nk_rgba(120, 120, 120, 255);
        table[NK_COLOR_TOGGLE_CURSOR] = nk_rgba(175, 175, 175, 255);
        table[NK_COLOR_SELECT] = nk_rgba(190, 190, 190, 255);
        table[NK_COLOR_SELECT_ACTIVE] = nk_rgba(175, 175, 175, 255);
        table[NK_COLOR_SLIDER] = nk_rgba(190, 190, 190, 255);
        table[NK_COLOR_SLIDER_CURSOR] = nk_rgba(80, 80, 80, 255);
        table[NK_COLOR_SLIDER_CURSOR_HOVER] = nk_rgba(70, 70, 70, 255);
        table[NK_COLOR_SLIDER_CURSOR_ACTIVE] = nk_rgba(60, 60, 60, 255);
        table[NK_COLOR_PROPERTY] = nk_rgba(175, 175, 175, 255);
        table[NK_COLOR_EDIT] = nk_rgba(150, 150, 150, 255);
        table[NK_COLOR_EDIT_CURSOR] = nk_rgba(0, 0, 0, 255);
        table[NK_COLOR_COMBO] = nk_rgba(175, 175, 175, 255);
        table[NK_COLOR_CHART] = nk_rgba(160, 160, 160, 255);
        table[NK_COLOR_CHART_COLOR] = nk_rgba(45, 45, 45, 255);
        table[NK_COLOR_CHART_COLOR_HIGHLIGHT] = nk_rgba( 255, 0, 0, 255);
        table[NK_COLOR_SCROLLBAR] = nk_rgba(180, 180, 180, 255);
        table[NK_COLOR_SCROLLBAR_CURSOR] = nk_rgba(140, 140, 140, 255);
        table[NK_COLOR_SCROLLBAR_CURSOR_HOVER] = nk_rgba(150, 150, 150, 255);
        table[NK_COLOR_SCROLLBAR_CURSOR_ACTIVE] = nk_rgba(160, 160, 160, 255);
        table[NK_COLOR_TAB_HEADER] = nk_rgba(180, 180, 180, 255);
        nk_style_from_table(ctx, table);
    } else if (theme == THEME_RED) {
        table[NK_COLOR_TEXT] = nk_rgba(190, 190, 190, 255);
        table[NK_COLOR_WINDOW] = nk_rgba(30, 33, 40, 215);
        table[NK_COLOR_HEADER] = nk_rgba(181, 45, 69, 220);
        table[NK_COLOR_BORDER] = nk_rgba(51, 55, 67, 255);
        table[NK_COLOR_BUTTON] = nk_rgba(181, 45, 69, 255);
        table[NK_COLOR_BUTTON_HOVER] = nk_rgba(190, 50, 70, 255);
        table[NK_COLOR_BUTTON_ACTIVE] = nk_rgba(195, 55, 75, 255);
        table[NK_COLOR_TOGGLE] = nk_rgba(51, 55, 67, 255);
        table[NK_COLOR_TOGGLE_HOVER] = nk_rgba(45, 60, 60, 255);
        table[NK_COLOR_TOGGLE_CURSOR] = nk_rgba(181, 45, 69, 255);
        table[NK_COLOR_SELECT] = nk_rgba(51, 55, 67, 255);
        table[NK_COLOR_SELECT_ACTIVE] = nk_rgba(181, 45, 69, 255);
        table[NK_COLOR_SLIDER] = nk_rgba(51, 55, 67, 255);
        table[NK_COLOR_SLIDER_CURSOR] = nk_rgba(181, 45, 69, 255);
        table[NK_COLOR_SLIDER_CURSOR_HOVER] = nk_rgba(186, 50, 74, 255);
        table[NK_COLOR_SLIDER_CURSOR_ACTIVE] = nk_rgba(191, 55, 79, 255);
        table[NK_COLOR_PROPERTY] = nk_rgba(51, 55, 67, 255);
        table[NK_COLOR_EDIT] = nk_rgba(51, 55, 67, 225);
        table[NK_COLOR_EDIT_CURSOR] = nk_rgba(190, 190, 190, 255);
        table[NK_COLOR_COMBO] = nk_rgba(51, 55, 67, 255);
        table[NK_COLOR_CHART] = nk_rgba(51, 55, 67, 255);
        table[NK_COLOR_CHART_COLOR] = nk_rgba(170, 40, 60, 255);
        table[NK_COLOR_CHART_COLOR_HIGHLIGHT] = nk_rgba( 255, 0, 0, 255);
        table[NK_COLOR_SCROLLBAR] = nk_rgba(30, 33, 40, 255);
        table[NK_COLOR_SCROLLBAR_CURSOR] = nk_rgba(64, 84, 95, 255);
        table[NK_COLOR_SCROLLBAR_CURSOR_HOVER] = nk_rgba(70, 90, 100, 255);
        table[NK_COLOR_SCROLLBAR_CURSOR_ACTIVE] = nk_rgba(75, 95, 105, 255);
        table[NK_COLOR_TAB_HEADER] = nk_rgba(181, 45, 69, 220);
        nk_style_from_table(ctx, table);
    } else if (theme == THEME_BLUE) {
        table[NK_COLOR_TEXT] = nk_rgba(20, 20, 20, 255);
        table[NK_COLOR_WINDOW] = nk_rgba(202, 212, 214, 215);
        table[NK_COLOR_HEADER] = nk_rgba(137, 182, 224, 220);
        table[NK_COLOR_BORDER] = nk_rgba(140, 159, 173, 255);
        table[NK_COLOR_BUTTON] = nk_rgba(137, 182, 224, 255);
        table[NK_COLOR_BUTTON_HOVER] = nk_rgba(142, 187, 229, 255);
        table[NK_COLOR_BUTTON_ACTIVE] = nk_rgba(147, 192, 234, 255);
        table[NK_COLOR_TOGGLE] = nk_rgba(177, 210, 210, 255);
        table[NK_COLOR_TOGGLE_HOVER] = nk_rgba(182, 215, 215, 255);
        table[NK_COLOR_TOGGLE_CURSOR] = nk_rgba(137, 182, 224, 255);
        table[NK_COLOR_SELECT] = nk_rgba(177, 210, 210, 255);
        table[NK_COLOR_SELECT_ACTIVE] = nk_rgba(137, 182, 224, 255);
        table[NK_COLOR_SLIDER] = nk_rgba(177, 210, 210, 255);
        table[NK_COLOR_SLIDER_CURSOR] = nk_rgba(137, 182, 224, 245);
        table[NK_COLOR_SLIDER_CURSOR_HOVER] = nk_rgba(142, 188, 229, 255);
        table[NK_COLOR_SLIDER_CURSOR_ACTIVE] = nk_rgba(147, 193, 234, 255);
        table[NK_COLOR_PROPERTY] = nk_rgba(210, 210, 210, 255);
        table[NK_COLOR_EDIT] = nk_rgba(210, 210, 210, 225);
        table[NK_COLOR_EDIT_CURSOR] = nk_rgba(20, 20, 20, 255);
        table[NK_COLOR_COMBO] = nk_rgba(210, 210, 210, 255);
        table[NK_COLOR_CHART] = nk_rgba(210, 210, 210, 255);
        table[NK_COLOR_CHART_COLOR] = nk_rgba(137, 182, 224, 255);
        table[NK_COLOR_CHART_COLOR_HIGHLIGHT] = nk_rgba( 255, 0, 0, 255);
        table[NK_COLOR_SCROLLBAR] = nk_rgba(190, 200, 200, 255);
        table[NK_COLOR_SCROLLBAR_CURSOR] = nk_rgba(64, 84, 95, 255);
        table[NK_COLOR_SCROLLBAR_CURSOR_HOVER] = nk_rgba(70, 90, 100, 255);
        table[NK_COLOR_SCROLLBAR_CURSOR_ACTIVE] = nk_rgba(75, 95, 105, 255);
        table[NK_COLOR_TAB_HEADER] = nk_rgba(156, 193, 220, 255);
        nk_style_from_table(ctx, table);
    } else if (theme == THEME_DARK) {
        table[NK_COLOR_TEXT] = nk_rgba(210, 210, 210, 255);
        table[NK_COLOR_WINDOW] = nk_rgba(57, 67, 71, 215);
        table[NK_COLOR_HEADER] = nk_rgba(51, 51, 56, 220);
        table[NK_COLOR_BORDER] = nk_rgba(46, 46, 46, 255);
        table[NK_COLOR_BUTTON] = nk_rgba(48, 83, 111, 255);
        table[NK_COLOR_BUTTON_HOVER] = nk_rgba(58, 93, 121, 255);
        table[NK_COLOR_BUTTON_ACTIVE] = nk_rgba(63, 98, 126, 255);
        table[NK_COLOR_TOGGLE] = nk_rgba(50, 58, 61, 255);
        table[NK_COLOR_TOGGLE_HOVER] = nk_rgba(45, 53, 56, 255);
        table[NK_COLOR_TOGGLE_CURSOR] = nk_rgba(48, 83, 111, 255);
        table[NK_COLOR_SELECT] = nk_rgba(57, 67, 61, 255);
        table[NK_COLOR_SELECT_ACTIVE] = nk_rgba(48, 83, 111, 255);
        table[NK_COLOR_SLIDER] = nk_rgba(50, 58, 61, 255);
        table[NK_COLOR_SLIDER_CURSOR] = nk_rgba(48, 83, 111, 245);
        table[NK_COLOR_SLIDER_CURSOR_HOVER] = nk_rgba(53, 88, 116, 255);
        table[NK_COLOR_SLIDER_CURSOR_ACTIVE] = nk_rgba(58, 93, 121, 255);
        table[NK_COLOR_PROPERTY] = nk_rgba(50, 58, 61, 255);
        table[NK_COLOR_EDIT] = nk_rgba(50, 58, 61, 225);
        table[NK_COLOR_EDIT_CURSOR] = nk_rgba(210, 210, 210, 255);
        table[NK_COLOR_COMBO] = nk_rgba(50, 58, 61, 255);
        table[NK_COLOR_CHART] = nk_rgba(50, 58, 61, 255);
        table[NK_COLOR_CHART_COLOR] = nk_rgba(48, 83, 111, 255);
        table[NK_COLOR_CHART_COLOR_HIGHLIGHT] = nk_rgba(255, 0, 0, 255);
        table[NK_COLOR_SCROLLBAR] = nk_rgba(50, 58, 61, 255);
        table[NK_COLOR_SCROLLBAR_CURSOR] = nk_rgba(48, 83, 111, 255);
        table[NK_COLOR_SCROLLBAR_CURSOR_HOVER] = nk_rgba(53, 88, 116, 255);
        table[NK_COLOR_SCROLLBAR_CURSOR_ACTIVE] = nk_rgba(58, 93, 121, 255);
        table[NK_COLOR_TAB_HEADER] = nk_rgba(48, 83, 111, 255);
        nk_style_from_table(ctx, table);
    } else {
        nk_style_default(ctx);
    }
}

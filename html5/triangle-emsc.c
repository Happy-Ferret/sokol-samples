//------------------------------------------------------------------------------
//  triangle-emsc.c
//  Vertex buffer, simple shader, pipeline state object.
//------------------------------------------------------------------------------
#define GL_GLEXT_PROTOTYPES
#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>
#define SOKOL_IMPL
#define SOKOL_GLES2
#include "sokol_gfx.h"
#include "emsc.h"

sg_draw_state draw_state;
sg_pass_action pass_action = {
    .colors[0] = { .action = SG_ACTION_CLEAR, .val = { 0.0f, 0.0f, 0.0f, 1.0f } }
};
void draw();

int main() {
    /* setup WebGL context */
    emsc_init("#canvas", EMSC_TRY_WEBGL2|EMSC_ANTIALIAS);

    /* setup sokol_gfx */
    sg_desc desc = {0};
    sg_setup(&desc);
    assert(sg_isvalid());
    
    /* a vertex buffer with 3 vertices */
    float vertices[] = {
        // positions            // colors
         0.0f,  0.5f, 0.5f,     1.0f, 0.0f, 0.0f, 1.0f,
         0.5f, -0.5f, 0.5f,     0.0f, 1.0f, 0.0f, 1.0f,
        -0.5f, -0.5f, 0.5f,     0.0f, 0.0f, 1.0f, 1.0f 
    };
    sg_buffer_desc buf_desc = {
        .size = sizeof(vertices),
        .content = vertices,
    };

    /* create a shader */
    sg_shader_desc shd_desc = {
        .vs.source =
            "attribute vec4 position;\n"
            "attribute vec4 color0;\n"
            "varying vec4 color;\n"
            "void main() {\n"
            "  gl_Position = position;\n"
            "  color = color0;\n"
            "}\n",
        .fs.source =
            "precision mediump float;\n"
            "varying vec4 color;\n"
            "void main() {\n"
            "  gl_FragColor = color;\n"
            "}\n"
    };

    /* create a pipeline object (default render states are fine for triangle) */
    sg_pipeline_desc pip_desc = {
        /* if the vertex layout doesn't have gaps, don't need to provide strides and offsets */
        .shader = sg_make_shader(&shd_desc),
        .layout = {
            .attrs = {
                [0] = { .name="position", .format=SG_VERTEXFORMAT_FLOAT3 },
                [1] = { .name="color0", .format=SG_VERTEXFORMAT_FLOAT4 }
            }
        },
    };

    /* setup the draw state with resource bindings */
    draw_state = (sg_draw_state){
        .pipeline = sg_make_pipeline(&pip_desc),
        .vertex_buffers[0] = sg_make_buffer(&buf_desc)
    };

    /* hand off control to browser loop */
    emscripten_set_main_loop(draw, 0, 1);
    return 0;
}

/* draw one frame */
void draw() {
    sg_begin_default_pass(&pass_action, emsc_width(), emsc_height());
    sg_apply_draw_state(&draw_state);
    sg_draw(0, 3, 1);
    sg_end_pass();
    sg_commit();
}
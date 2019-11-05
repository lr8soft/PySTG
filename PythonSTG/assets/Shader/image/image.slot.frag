#version 330 core
uniform sampler2D tex;
uniform vec4 limit_info;
uniform vec2 offset_info;
in vec2 tex_coord;
out vec4 render_color;
void main(){
    float minX = limit_info.x;
    float maxX = limit_info.y;
    float minY = limit_info.z;
    float maxY = limit_info.w;

    if(float(tex_coord.x) >= minX && float(tex_coord.x) <= maxX){
        if(float(tex_coord.y) >= minY && float(tex_coord.y) <= maxY){
            render_color = texture(tex, tex_coord);
        }
    }
}

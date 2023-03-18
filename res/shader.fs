#version 330 core

layout(location = 0) out vec4 fragmentColor;

uniform float u_width;
uniform float u_height;

void render(inout vec3 color, in vec2 uv) {
    color.rb += uv;
    color.g = 0.0;
}

void main() {
    vec2 uv = (2.0 * gl_FragCoord.xy - vec2(u_width, u_height)) / u_height;

    vec3 color;
    render(color, uv);

    fragmentColor = vec4(color, 1.0);
}
#version 330 core

out vec4 fragmentColor;

uniform float u_width;
uniform float u_height;

const float FOV = 1.0;
const int MAX_RAY_STEPS = 256;
const float MAX_DISTANCE = 500;
const float EPSILON = 0.001;

float GetDistacnce(vec3 p) {
    //p = mod(p, 4.0) - 4.0 * 0.5;
    vec3 sphereCoord = vec3(0, 1, 4);
    float sphereRadius = 1.0;
    float dis = length(p - sphereCoord) - sphereRadius;
    return dis;
}

float RayMarching(vec3 rayOrigin, vec3 rayDirection) {
    float distance = 0.0;
    for (int i = 0; i < MAX_RAY_STEPS; i++) {
        vec3 p = rayOrigin + distance * rayDirection;
        float distanceToSurface = GetDistacnce(p);
        distance += distanceToSurface;
        if (distanceToSurface < EPSILON || distance > MAX_DISTANCE) break;
    }
    return distance;
}

void main() {
    vec2 uv = (gl_FragCoord.xy - 0.5*vec2(u_width, u_height)) / u_height;

    vec3 rayOrigin = vec3(0.0, 1.0, 0.0);
    vec3 rayDirection = normalize(vec3(uv, FOV));

    float d = RayMarching(rayOrigin, rayDirection);
    vec3 color = vec3(3.0/d);

    fragmentColor = vec4(color, 1.0);
}
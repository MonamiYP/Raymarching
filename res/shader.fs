#version 330 core

out vec4 fragmentColor;

uniform float u_width;
uniform float u_height;
uniform vec3 u_cameraPos;
uniform vec2 u_cameraAngles;
uniform vec3 u_cameraDir;
uniform vec3 u_cameraUp;
uniform mat4 u_view;

const int MAX_RAY_STEPS = 256;
const float MAX_DISTANCE = 500;
const float EPSILON = 0.001;

float sdSphere(vec3 p, float radius, vec3 centreCoords) {
    return length(p - centreCoords) - radius;
}

float sdBox(vec3 p, vec3 boxDimensions, vec3 centreCoords) {
    vec3 q = abs(p - centreCoords) - boxDimensions;
    return length(max(q, 0.0)) + min(max(q.x, max(q.y, q.z)), 0.0);
}

float opRep(vec3 p, vec3 c, vec3 o, float r) {
    vec3 q = mod(p+0.5*c, c) - 0.5*c;
    return sdSphere(q, r, o);
}

float GetDistacnce(vec3 p) {
    //float box = sdSphere(p, 1.0, vec3(0.0, 0.0, -2.0));
    float box = sdBox(p, vec3(1.0, 1.0, 1.0), vec3(0.0, 0.0, -8.0));
    return box;
    //return opRep(p, vec3(20), vec3(3), 1.0);
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

mat3 RotateY(float theta) {
    float c = cos(theta);
    float s = sin(theta);
    return mat3(
        vec3(c, 0, s),
        vec3(0, 1, 0),
        vec3(-s, 0, c)
    );
}

mat3 RotateX(float theta) {
    float c = cos(theta);
    float s = sin(theta);
    return mat3(
        vec3(1, 0, 0),
        vec3(0, c, -s),
        vec3(0, s, c)
    );
}

mat3 camera() {
    vec3 rayFront = normalize(u_cameraDir);
    vec3 rayRight = normalize(cross(vec3(0, 1, 0), rayFront));
    vec3 rayUp = normalize(cross(rayFront, rayRight));
    return mat3(-rayRight, rayUp, -rayFront);
}

vec3 getNormal(vec3 p) {
    vec2 e = vec2(1.0, -1.0) * 0.0005; // epsilon
    float r = 1.0;
    vec3 n = e.xyy * sdSphere(p + e.xyy, r, vec3(0.0)) +
    e.yyx * sdSphere(p + e.yyx, r, vec3(0.0)) +
    e.yxy * sdSphere(p + e.yxy, r, vec3(0.0)) +
    e.xxx * sdSphere(p + e.xxx, r, vec3(0.0));
    return normalize(n);
}

vec3 caluclatePhong(vec3 p, vec3 lightDirection, vec3 rayDirection) {
    vec3 normal = getNormal(p);

    // Ambient
    vec3 ambient = 0.5 * vec3(0.7, 0.5, 0.0);

    // Diffuse
    float diff = max(dot(normal, lightDirection), 0.0);
    vec3 diffuse = 0.6 * diff * vec3(0.7, 0.7, 0.0);

    // Specular
    vec3 reflectDir = reflect(-lightDirection, normal);
    float spec = pow(max(dot(rayDirection, reflectDir), 0.0), 64.0);
    vec3 specular = 0.8 * spec * vec3(1.0, 1.0, 1.0);

    return diffuse + ambient + specular;
}

void main() {
    vec2 uv = (gl_FragCoord.xy - 0.5*vec2(u_width, u_height)) / u_height;

    vec3 rayOrigin = u_cameraPos;
    //vec3 rayDirection = normalize(mat3(u_view) * normalize(vec3(uv, -1.0)));
    vec3 rayDirection = normalize(vec3(uv, -1.0));
    //vec3 rayDirection = RotateY(u_cameraAngles.y) * normalize(vec3(uv, -1.0));

    float distance = RayMarching(rayOrigin, rayDirection);

    vec3 color = vec3(0.0);
    if (distance < MAX_DISTANCE) {
        vec3 p = rayOrigin + distance * rayDirection;
        vec3 lightPosition = vec3(-3.0, -3.0, -8.0);
        vec3 lightDirection = normalize(lightPosition - p);

        vec3 light2Position = vec3(0.0, 0.0, -9.0);
        vec3 light2Direction = normalize(lightPosition - p);

        color = caluclatePhong(p, lightDirection, rayDirection) * 0.9;
        color += caluclatePhong(p, light2Direction, rayDirection) * 0.6;
    }

    fragmentColor = vec4(color, 1.0);
}
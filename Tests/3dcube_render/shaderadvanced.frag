
#version 330 core
out vec4 FragColor;

in vec3 FragPos;
in vec3 Normal;

uniform vec3 lightPos;        // world-space position of the point light
uniform vec3 lightColor;      // light color
uniform float LightIntensity; // intensity multiplier
uniform float LightRange;     // max range of the light

uniform vec4 color;           // base material color
uniform vec3 viewPos;         // camera position

void main()
{
    // Normalize normal
    vec3 norm = normalize(Normal);

    // Direction from fragment to light
    vec3 lightDir = normalize(lightPos - FragPos);

    // Distance for attenuation
    float distance = length(lightPos - FragPos);

    // Unity-style attenuation: inverse-square falloff with range cutoff
    float attenuation = 1.0 / (1.0 + distance * distance);
    attenuation *= clamp(1.0 - distance / LightRange, 0.0, 1.0);

    // Ambient
    vec3 ambient = 0.1 * lightColor;

    // Diffuse
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * lightColor;

    // Specular
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
    vec3 specular = 0.5 * spec * lightColor;

    // Combine terms, scaled by intensity and attenuation
    vec3 result = (ambient + diffuse + specular) * color.rgb * LightIntensity * attenuation;

    FragColor = vec4(result, color.a);
}
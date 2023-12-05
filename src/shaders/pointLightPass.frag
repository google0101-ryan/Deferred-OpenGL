#version 430 core

in vec2 TexCoords;

out vec4 FragColor;

uniform sampler2D colorBuffer;
uniform sampler2D positionBuffer;
uniform sampler2D normalBuffer;
uniform sampler2D shadowMap;

uniform int lightID; // Light 0 is reserved for the global directional light

struct Light
{
    vec3 Position;
    vec3 Color;
};

uniform Light light;
uniform vec3 viewPos;

uniform mat4 lightSpaceMatrix;

float ShadowCalculation(vec4 fragPosLightSpace)
{
    vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
    projCoords = projCoords * 0.5 + 0.5;
    float closestDepth = texture(shadowMap, projCoords.xy).r;
    float currentDepth = projCoords.z;
    float shadow = currentDepth > closestDepth ? 1.0 : 0.0;
    return shadow;
}

void main()
{
    vec3 FragPos = texture(positionBuffer, TexCoords).rgb;
    vec3 Normal = texture(normalBuffer, TexCoords).rgb;
    vec3 Albedo = texture(colorBuffer, TexCoords).rgb;
    float Specular = texture(colorBuffer, TexCoords).a;

    vec4 fragPosLightSpace = lightSpaceMatrix * vec4(FragPos, 1.0);

    vec3 ambient = light.Color * 0.1;

    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(light.Position - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * light.Color;

    float distance = length(light.Position - FragPos);
    float attenuation = 1.0 / (1.0 + 0.09 * distance + 0.032 * (distance * distance));
    diffuse *= attenuation;
    ambient *= attenuation;

    float shadow = ShadowCalculation(fragPosLightSpace);

    vec3 result = (ambient + (1.0 - shadow) * diffuse) * Albedo;

    FragColor = vec4(result, 1.0);
}
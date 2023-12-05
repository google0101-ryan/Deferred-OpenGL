#version 430 core

in vec2 TexCoords;

out vec4 FragColor;

uniform sampler2D colorBuffer;
uniform sampler2D positionBuffer;
uniform sampler2D normalBuffer;
uniform sampler2D shadowMap;

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
    vec3 Diffuse = texture(colorBuffer, TexCoords).rgb;
    
    vec3 lighting = Diffuse * 0.1;
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 lightDir = normalize(light.Position - FragPos);
    vec3 diffuse = max(dot(Normal, lightDir), 0.0) * Diffuse * light.Color;
    //Specular
    vec3 halfwayDir = normalize(lightDir + viewDir);
    float spec = pow(max(dot(Normal, halfwayDir), 0.0), 16.0);
    vec3 specular = light.Color * spec * 0.5;
    // Attenuation
    float distance = length(light.Position - FragPos);
    float attenuation = 1.0 / (1.0 + 0.75 * distance + 1.8 * distance * distance);
    diffuse *= attenuation;
    specular *= attenuation;
    lighting += diffuse + specular;

    vec4 fragPosLightSpace = lightSpaceMatrix * vec4(FragPos, 1.0);

    float shadow = ShadowCalculation(fragPosLightSpace);

    gl_FragDepth = shadow;

    FragColor = vec4(lighting, 1.0);
}
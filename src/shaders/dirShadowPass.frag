#version 430 core

in vec2 TexCoords;

out vec4 FragColor;

uniform sampler2D colorBuffer;
uniform sampler2D positionBuffer;
uniform sampler2D shadowMap;

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
    vec3 Albedo = texture(colorBuffer, TexCoords).rgb;

    vec3 ambient = vec3(0.1, 0.1, 0.1);

    vec4 fragPosLightSpace = lightSpaceMatrix * vec4(FragPos, 1.0);

    float shadow = ShadowCalculation(fragPosLightSpace);

    vec3 result = (ambient + (1.0 - shadow)) * Albedo;

    FragColor = vec4(result, 1.0);
}
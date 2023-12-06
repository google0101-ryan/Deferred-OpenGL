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

float ShadowCalculation(vec4 fragPosLightSpace, float bias)
{
    vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
    projCoords = projCoords * 0.5 + 0.5;
    float closestDepth = texture(shadowMap, projCoords.xy).r;
    float currentDepth = projCoords.z;
    float shadow = 0.0;
    vec2 texelSize = 1.0 / textureSize(shadowMap, 0);
    for (int x = 0; x <= 1; x++)
    {
        for (int y = -1; y <= 1; y++)
        {
            float pcfDepth = texture(shadowMap, projCoords.xy + vec2(x, y) * texelSize).r;
            shadow += currentDepth - bias > pcfDepth ? 1.0 : 0.0;
        }
    }
    shadow /= 9.0;
    return shadow;
}

void main()
{
    vec3 FragPos = texture(positionBuffer, TexCoords).rgb;
    vec3 Normal = texture(normalBuffer, TexCoords).rgb;
    vec3 Diffuse = texture(colorBuffer, TexCoords).rgb;

    vec4 fragPosLightSpace = lightSpaceMatrix * vec4(FragPos, 1.0);
    
    float constant = 1.0;
    float linear = 0.7;
    float quadratic = 1.8;
    float lightMax = max(max(light.Color.r, light.Color.g), light.Color.b);
    float radius = (-linear + sqrt(linear * linear - 4 * quadratic  * (constant - (256.0 / 5.0) * lightMax)));
    float dis = length(light.Position - FragPos);

    vec3 ambient = light.Color * 0.15;
    vec3 lighting = vec3(0.0);
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 lightDir = normalize(light.Position - FragPos);
    float diff = max(dot(Normal, lightDir), 0.0);
    vec3 diffuse = light.Color * diff;
    //Specular
    vec3 halfwayDir = normalize(lightDir + viewDir);
    float spec = pow(max(dot(Normal, halfwayDir), 0.0), 16.0);
    vec3 specular = light.Color * spec * 0.5;
    // ttenuation
    float attenuation = 1.0 / (1.0 + dis*dis);
    diffuse *= attenuation;
    specular *= attenuation;
    ambient *= attenuation;

    float bias = max(0.05 * (1.0 - dot(Normal, lightDir)), 0.005);
    float shadow = ShadowCalculation(fragPosLightSpace, bias);

    lighting = (1.0 - shadow) * ((ambient + (diffuse + specular)) * Diffuse);

    //gl_FragDepth = vec4(lighting, 1.0).r;
    //FragColor = vec4(texture(shadowMap, TexCoords));
    FragColor = vec4(lighting, 1.0);
}
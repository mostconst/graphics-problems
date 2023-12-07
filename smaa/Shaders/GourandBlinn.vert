#version 330 core
layout (location = 0) in vec3 localPosition;
layout (location = 1) in vec3 localNormal;

uniform mat4 mvpMatrix;
uniform mat4 modelViewMatrix;
uniform mat3 modelViewNormalMatrix;

struct Material
{
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
	float opacity;
};

struct Light
{
    int type;
    vec3 viewPosition;
    vec3 viewDirection;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};
  
uniform Material material;

uniform Light light; 

out vec4 vertexColor;

vec3 colorFromLightSource(vec3 viewFragmentPosition, vec3 viewFragmentNormal, Light light)
{
    vec3 ambientColor = light.ambient * material.ambient;

    vec3 viewLightDirectionRaw = vec3(0.0);
    if (light.type == 1)
    {
        viewLightDirectionRaw = -light.viewDirection;
    }
    else if (light.type == 2)
    {
        viewLightDirectionRaw = light.viewPosition - viewFragmentPosition;
    }
    vec3 viewLightDirection = normalize(viewLightDirectionRaw);

    float diffuseCoefficient = max(dot(viewFragmentNormal, viewLightDirection), 0.0);
    vec3 diffuseColor = light.diffuse * diffuseCoefficient * material.diffuse;

	vec3 viewViewerDirection = normalize(-viewFragmentPosition);
    vec3 halfVector = normalize(viewLightDirection + viewViewerDirection);
	vec3 viewReflectDirection = reflect(-viewLightDirection, viewFragmentNormal);
	float specularCoefficient = pow(max(dot(viewFragmentNormal, halfVector), 0.0), material.shininess);
	vec3 specularColor = light.specular * specularCoefficient * material.specular;

    return ambientColor + diffuseColor + specularColor;
}

void main()
{
	gl_Position = mvpMatrix * vec4(localPosition, 1.0f);
	vec3 viewFragmentPosition = vec3(modelViewMatrix * vec4(localPosition, 1.0f));
	vec3 viewFragmentNormal = normalize(modelViewNormalMatrix * localNormal);

    vec3 lightSourceColor = colorFromLightSource(viewFragmentPosition, viewFragmentNormal, light);

	vertexColor = vec4(material.ambient, 1.0);
}
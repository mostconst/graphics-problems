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
};

struct Light
{
    vec3 viewPosition;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};
  
uniform Material material;

uniform Light light; 

out vec3 vertexColor;

void main()
{
	gl_Position = mvpMatrix * vec4(localPosition, 1.0f);
	vec3 viewFragmentPosition = vec3(modelViewMatrix * vec4(localPosition, 1.0f));
	vec3 viewFragmentNormal = normalize(modelViewNormalMatrix * localNormal);

    vec3 ambientColor = light.ambient * material.ambient;

    vec3 viewLightDirection = normalize(light.viewPosition - viewFragmentPosition);
    float diffuseCoefficient = max(dot(viewFragmentNormal, viewLightDirection), 0.0);
    vec3 diffuseColor = light.diffuse * diffuseCoefficient * material.diffuse;

	vec3 viewViewerDirection = normalize(-viewFragmentPosition);
	vec3 viewReflectDirection = reflect(-viewLightDirection, viewFragmentNormal);
	float specularCoefficient = pow(max(dot(viewViewerDirection, viewReflectDirection), 0.0), material.shininess);
	vec3 specularColor = light.specular * specularCoefficient * material.specular;

	const vec3 globalAmbientLight = vec3(0.2);
	vec3 ambientColorGlobal = globalAmbientLight * material.ambient;

	vertexColor = ambientColor + ambientColorGlobal  + diffuseColor + specularColor;
}
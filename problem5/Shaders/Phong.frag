#version 330 core

in vec3 viewFragmentPosition;
in vec3 viewFragmentNormal;

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

out vec4 fragmentColor;

void main()
{
    vec3 ambientColor = light.ambient * material.ambient;

    vec3 viewLightDirection = normalize(light.viewPosition - viewFragmentPosition);
    vec3 norm = normalize(viewFragmentNormal);
    float diffuseCoefficient = max(dot(norm, viewLightDirection), 0.0);
    vec3 diffuseColor = light.diffuse * diffuseCoefficient * material.diffuse;

	vec3 viewViewerDirection = normalize(-viewFragmentPosition);
	vec3 viewReflectDirection = reflect(-viewLightDirection, norm);
	float specularCoefficient = pow(max(dot(viewViewerDirection, viewReflectDirection), 0.0), material.shininess);
	vec3 specularColor = light.specular * specularCoefficient * material.specular;

	const vec3 globalAmbientLight = vec3(0.2);
	vec3 ambientColorGlobal = globalAmbientLight * material.ambient;

	vec3 vertexColor = ambientColor + ambientColorGlobal  + diffuseColor + specularColor;
    fragmentColor = vec4(vertexColor, 1.0f);
}
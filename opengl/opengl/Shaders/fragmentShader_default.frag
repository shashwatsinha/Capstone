#version 330 core
struct Material {
    sampler2D diffuse;
    sampler2D specular;
    float shininess;
};

struct DirectionalLight {
	//vec3 position;
	vec3 direction;
  
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

};

in vec2 TexCoords;
in vec3 FragPos;
in vec3 Normal;
out vec4 color;

uniform vec3 viewPos;
uniform Material material;
uniform DirectionalLight directionalLight;

void main()
{
	vec3 ambientLight = directionalLight.ambient * vec3(texture(material.diffuse, TexCoords));

	vec3 normal = normalize(Normal);
	//vec3 lightDirection = normalize(directionalLight.position - FragPos);
	vec3 lightDirection = normalize(-directionalLight.direction);

	float diff = max(dot(normal, lightDirection), 0.0);
	vec3 diffuseLight = directionalLight.diffuse * diff * vec3(texture(material.diffuse, TexCoords));

	vec3 viewDirection = normalize(viewPos - FragPos);
	vec3 reflectDirection = reflect(-lightDirection, normal);
	float spec = pow(max(dot(viewDirection, reflectDirection), 0.0), material.shininess);
	vec3 specularLight = directionalLight.specular * spec * vec3(texture(material.specular, TexCoords));

	vec3 result = ambientLight + diffuseLight + specularLight;
	color = vec4(result, 1.0f);
}
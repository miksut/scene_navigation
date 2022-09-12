#version 330 core

in vec3 vertexColor;
in vec3 vertexNormal;
in vec3 worldPos;
in vec2 texCoord;

uniform struct Material{
        vec3 ka;
        vec3 kd;
        vec3 ks;
        float ns;
        bool ifTextureColor;
        bool hasNormalTexture;
        bool hasSpecTexture;
        bool hasObjectColor;
        vec3 color;
        float illumination;
    }mat;

uniform sampler2D diffuseTexture;

uniform sampler2D cubetex;
uniform sampler2D cubetexSpec;

uniform sampler2D cubeNormalTex;
in mat3 tbnMatrix;

uniform vec3 camPos;

layout(location = 0) out vec4 color;

uniform struct Light {
	vec3 position;
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
} light;

void main()
{
	vec3 lightDir = normalize(vec3(normalize(light.position - worldPos)));
	float diffDot = max(dot(vertexNormal, lightDir), 0.0);
	vec3 diffuseColor = diffDot * light.diffuse;
	float ambientFactor = 0.1f;
	vec3 ambientColor = (light.ambient.xyz * ambientFactor);
	vec3 colorMap = vec3 (0.7,0.5,0.5);
	if (mat.ifTextureColor){
		colorMap = vec3(texture(diffuseTexture,texCoord));
	}else{
		colorMap = vertexColor;
	}

	vec3 viewDir = vec3(normalize(camPos - worldPos));
	vec3 halfwayDir = normalize(lightDir + viewDir);
	float specDot = max(dot(vertexNormal, halfwayDir), 0.0);
	float spec = pow(specDot, 32);
	float specStrength = 1.0;
	vec3 specularColor = specStrength * spec * light.specular;
	color = vec4((ambientColor + diffuseColor) * colorMap.rgb + (specularColor)+ mat.illumination * mat.color, 1.0);
}

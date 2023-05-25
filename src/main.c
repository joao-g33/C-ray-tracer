#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <assert.h>
#include <stdbool.h>
#include <string.h>

#include "libs/vector3.h"
#include "libs/shapes/sphere.h"

#define INTERVAL(INITIAL_VALUE, FINAL_VALUE) float i = (INITIAL_VALUE); i <= (FINAL_VALUE); i += 0.1
#define INITIAL_COORD -20
#define FINAL_COORD 20

// proporções da cena (3840px x 2160px por padrão (imagem na resolução 4K))
#define SCENE_WIDTH 3840
#define SCENE_HEIGHT 2160

#define SCENE_BACKGROUND_COLOR Vector3(0, 0, 0)

typedef struct {
    const char *filename;
    const char *magic;
    const int width;
    const int height;
    const int max_color_value;
} PPM_header_t;

typedef struct {
    Vector3_t camera_origin;
    const int ray_depth;
    const int FOV;
    Sphere_t *lights;
    Sphere_t *spheres;
    const int num_lights;
    const int num_spheres;
} Scene_Parameters_t;

Vector3_t trace(int ray_depth, const Vector3_t ray_origin, const Vector3_t ray_direction, Sphere_t *lights, int num_lights, Sphere_t *spheres, int num_spheres) {
    float closest_intersection_distance = 1e10;
    float distance_to_intersection;
    int hit_sphere = -1;

    // percorrer todas as esferas da cena para encontrar aa interseção mais próxima com o vetor do raio
    for (int i = 0; i < num_spheres; ++i) {
        if (intersect_sphere(spheres[i], ray_origin, ray_direction, &distance_to_intersection)) {
            if (closest_intersection_distance > distance_to_intersection) {
                closest_intersection_distance = distance_to_intersection;
                hit_sphere = i;
            }
        }
    }
    
    // se houve interseção com uma esfera
    if (hit_sphere > -1) {
        Vector3_t intersection_point = vec3_add(ray_origin, vec3_mul_scalar(ray_direction, closest_intersection_distance));

        // definir o vetor normal no ponto de interseção
        Vector3_t normal = vec3_normalize(vec3_sub(intersection_point, spheres[hit_sphere].position));

        // calcular o vetor de reflexão
        Vector3_t reflection = vec3_sub(ray_direction, vec3_mul_scalar(normal, vec3_dot(ray_direction, normal) * 2));

        // calcular a cor do raio no ponto de interseção
        Vector3_t color = vec3_mul_scalar(spheres[hit_sphere].color, 0.1f);

        // se a esfera atingida for a primeira (chão), definir sua cor sendo um xadrez preto e branco
        if (hit_sphere == 0) {
            int checkerboard = ((int)(intersection_point.x + 1e2) + (int)(intersection_point.z + 1e2)) & 1;
            spheres[hit_sphere].color = checkerboard ? Vector3(1, 1, 1) : Vector3(0, 0, 0);
        }

        // calcular a iluminação difusa e especular
        for (int j = 0; j < num_lights; ++j) {
            Vector3_t light_direction = vec3_normalize(vec3_sub(lights[j].position, intersection_point));
            int in_shadow = 0;

            // verificar se há sombra
            for (int i = 0; i < num_spheres; ++i) {
                // verificar se ele intersecta com alguma esfera (que bloqueia o raio de luz, gerando uma sombra)
                if (intersect_sphere(spheres[i], intersection_point, light_direction, &distance_to_intersection)) {
                    in_shadow = 1;
                    break;
                }
            }
            
            // se não houver sombra
            if (!in_shadow) {
                float diffuse = fmax(0.f, vec3_dot(light_direction, normal)) * 0.7;
                float specular = powf(fmax(0.f, vec3_dot(light_direction, normal)), 50.f) * 0.3f;
            
                Vector3_t specularVec = Vector3(specular, specular, specular);
            
                color = vec3_add(color, vec3_add(vec3_mul(spheres[hit_sphere].color, vec3_mul_scalar(lights[j].color, diffuse)), specularVec));
            }
        }

        // chamada recursiva para rastrear o raio refletido e sua contribuição para a formação de cor no pixel
        // atual da imagem, repetir isso até que ray_depth < 0 e esfera reflete luz
        if (--ray_depth >= 0 && spheres[hit_sphere].reflective) {
            Vector3_t recColor = vec3_mul_scalar(trace(ray_depth, intersection_point, reflection, lights, num_lights, spheres, num_spheres), 0.51f);
            color = vec3_add(color, recColor);
        }

        return color;
    }

    // caso não haja interseção, pintar a cor do fundo da cena
    return SCENE_BACKGROUND_COLOR;
}

void render_scene(PPM_header_t ppm_header, Scene_Parameters_t scene_parameters) {
    float aspect_ratio = (float) ppm_header.width / (float) ppm_header.height;
    float FOV_radians  = scene_parameters.FOV * 3.14159 / 180.0;
    float half_height  = tanf(FOV_radians / 2.0);
    float half_width   = aspect_ratio * half_height;

    // abrir o arquivo de saída e garantir que ele foi aberto
    FILE *out = fopen(ppm_header.filename, "w");
    assert(out);

    // escrever o cabeçalho ppm no arquivo de saída
    fprintf(out, "%s\n%d %d\n%d\n", ppm_header.magic, ppm_header.width, ppm_header.height, ppm_header.max_color_value);

    for (int y = 0; y < SCENE_HEIGHT; ++y) {
        for (int x = 0; x < SCENE_WIDTH; ++x) {
            // calcular a direção do raio para cada pixel da imagem em uma projeção perspectiva
            // normalizar as coordenadas x e y dos pixels considerando o FOV e a relação de aspecto da imagem
            Vector3_t ray_direction = {
                (2.0 * ((x + 0.5) / SCENE_WIDTH) - 1.0) * half_width,
                (1.0 - 2.0 * ((y + 0.5) / SCENE_HEIGHT)) * half_height,
                -1.0
            };

            // normalizar a direção do raio
            Vector3_t direction = vec3_normalize(ray_direction);
            
            // calcular a cor do pixel com base nas interseções do raio de luz
            Vector3_t color = trace(
                scene_parameters.ray_depth, 
                scene_parameters.camera_origin, 
                direction, 
                scene_parameters.lights, scene_parameters.num_lights, 
                scene_parameters.spheres, scene_parameters.num_spheres
            );

            // calcular os valores RGB do pixel (0 .. 255)
            int r = (int) (fmin(1.f, color.x) * 255);
            int g = (int) (fmin(1.f, color.y) * 255);
            int b = (int) (fmin(1.f, color.z) * 255);

            // escrevê-los no arquivo de saída
            fprintf(out, "%d %d %d\n", r, g, b);
        }

        // mostrar a porcentagem do progresso de renderização
        float percentage_completed = 100.0 * ((float) (y + 1) / (float) SCENE_HEIGHT);
        printf("\rprogresso da renderização: %.2f%%", percentage_completed);
    }
    printf("\n");

    fclose(out);
}

int main(int argc, const char *argv[]) {
    PPM_header_t ppm_header = {
        .filename = "render.ppm",
        .magic = "P3",
        .width = SCENE_WIDTH,
        .height = SCENE_HEIGHT,
        .max_color_value = 255
    };

    Sphere_t lights[] = {
        create_sphere(Vector3(-10,  6, -20), COLOR_YELLOW_LAMP, 2, false),
        create_sphere(Vector3( 0,  6, -20),  COLOR_YELLOW_LAMP, 2, false),
        create_sphere(Vector3( 10,  6, -20), COLOR_YELLOW_LAMP, 2, false)
    };
    
    Sphere_t spheres[] = {
        create_sphere(Vector3( 0, -1000, 0), COLOR_WHITE, 1000, false),
        create_sphere(Vector3(-4,  1,    0), COLOR_RED,   1,    true),
        create_sphere(Vector3( 0,  3,   -5), COLOR_WHITE, 3,    true),
        create_sphere(Vector3( 4,  1,    0), COLOR_BLUE,  1,    true)
    };

    Scene_Parameters_t scene = {
        .camera_origin = Vector3(0, 2, 15),
        .ray_depth = 5,
        .FOV = 50,
        .lights = lights,
        .spheres = spheres,
        .num_lights = (int) sizeof(lights) / sizeof(Sphere_t),
        .num_spheres = (int) sizeof(spheres) / sizeof(Sphere_t)
    };

    printf("renderizando cena em: %s...\n", ppm_header.filename);
    
    render_scene(ppm_header, scene);

    return 0;
}

#ifndef __SPHERE_H
#define __SPHERE_H

#include <stdbool.h>

#include "../vector3.h"

// objeto: esfera
typedef struct {
    Vector3_t position, color;  // posição (x, y, z) e cor (r, g, b)
    float radius;               // raio (rad)
    bool reflective;            // a esfera reflete luz ou não (se sim, true, senão, false)
} Sphere_t;

/// @brief cria uma esfera com os parâmetros fornecidos
/// @param position posição da esfera no espaço tri-dimensional
/// @param color cor da esfera (RGB)
/// @param radius raio da esfera
/// @param reflective valor booleano indicando se a esfera é reflexiva ou não (reflete luz ou não)
/// @return esfera criada com os parâmetros fornecidos
Sphere_t create_sphere(Vector3_t position, Vector3_t color, float radius, bool reflective);

/// @brief verifica se um raio de origem e direção dada intercepta uma esfera
/// @param sphere esfera com a qual se deseja verificar a interseção
/// @param origin ponto de origem do raio
/// @param ray_direction direção do raio
/// @param intersection_distance ponteiro para a variável onde a distância de interseção será armazenada, se houver interseção
/// @return true se houver interseção, false caso contrário
bool intersect_sphere(const Sphere_t sphere, const Vector3_t origin, const Vector3_t ray_direction, float *intersection_distance);

#endif //__SPHERE_H
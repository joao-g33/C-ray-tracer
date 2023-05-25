#include <math.h>
#include <stdbool.h>

#include "../vector3.h"
#include "sphere.h"

Sphere_t create_sphere(Vector3_t position, Vector3_t color, float radius, bool reflective) {
    return (Sphere_t) {position, color, radius, reflective};
}

bool intersect_sphere(const Sphere_t sphere, const Vector3_t origin, const Vector3_t ray_direction, float *intersection_distance) {
    // vetor do ponto de origem da esfera até a origem do raio
    Vector3_t origin_to_radius = vec3_sub(origin, sphere.position);
    
    // cálcular os coeficientes da equação quadrática de interseção
    float a = vec3_dot(ray_direction, ray_direction);
    float b = vec3_dot(origin_to_radius, ray_direction) * 2;
    float c = vec3_dot(origin_to_radius, origin_to_radius) - sphere.radius * sphere.radius;
    float discriminant = b * b - 4 * a * c;
    
    // se o discriminante < 0, não há soluções reais
    if (discriminant < 0) {
        return false;
    }
    
    // cálcular bhaskara (pensei que nunca mais iria ver isso na vida)
    float denominator = 2.f * a;
    float squared_discriminant = sqrtf(discriminant);

    // calcular a primeira possível distância de interseção
    *intersection_distance = ((-b) - squared_discriminant) / denominator;
    
    // se a distância de interseção for maior que 0.1f, há interseção
    if (*intersection_distance > 0.1f) { 
        return true; 
    }
    
    // calcular a segunda possível distância de interseção
    *intersection_distance = ((-b) + squared_discriminant) / denominator;
    
    // se a distância de interseção for maior que 0.1f, há interseção
    if (*intersection_distance > 0.1f) { 
        return true; 
    }
    
    // não houve interseção do raio com algum objeto
    return false;
}

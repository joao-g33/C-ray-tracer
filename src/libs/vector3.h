#ifndef __VECTOR3_H
#define __VECTOR3_H

typedef struct {
    float x, y, z;
} Vector3_t;

/// @brief inicializa um vetor
/// @param x coordenada x
/// @param y coordenada y
/// @param z coordenada z
/// @return vetor inicializado
Vector3_t Vector3(float x, float y, float z);

/// @brief realiza a multiplicação de um vetor por um escalar
/// @param V vetor
/// @param T escalar
/// @return vetor multiplicado pelo escalar
Vector3_t vec3_mul_scalar(const Vector3_t V, float T);

/// @brief soma dois vetores U e V
/// @param U vetor 1
/// @param V vetor 2
/// @return vetor resultante
Vector3_t vec3_add(const Vector3_t U, const Vector3_t V);

/// @brief subtrai dois vetores U e V
/// @param U vetor 1
/// @param V vetor 2
/// @return vetor resultante 
Vector3_t vec3_sub(const Vector3_t U, const Vector3_t V);

/// @brief multiplica dois vetores U e V
/// @param U vetor 1
/// @param V vetor 2
/// @return vetor resultante
Vector3_t vec3_mul(const Vector3_t U, const Vector3_t V);

/// @brief obtem o comprimento de um vetor
/// @param V vetor
/// @return comprimento do vetor
float vec3_length(const Vector3_t V);

/// @brief calcula o produto escalar entre dois vetores U e V
/// @param U vetor 1
/// @param V vetor 2
/// @return vetor resultante
float vec3_dot(const Vector3_t U, const Vector3_t V);

/// @brief normaliza um vetor
/// @param V vetor
/// @return vetor normal
Vector3_t vec3_normalize(const Vector3_t V);

#define COLOR_RED   Vector3(1, 0, 0)
#define COLOR_GREEN Vector3(0, 1, 0)
#define COLOR_BLUE  Vector3(0, 0, 1)
#define COLOR_WHITE Vector3(1, 1, 1)
#define COLOR_BLACK Vector3(0, 0, 0)

// cor de luz que se assemelha a cor amarela da luz de uma lampada incandecente 
#define COLOR_YELLOW_LAMP Vector3(1, 0.9, 0.7)

#endif //__VECTOR3_H
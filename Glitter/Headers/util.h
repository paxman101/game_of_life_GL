#pragma once

#include <string>
#include <glm/glm.hpp>

char* readShader(const std::string& filename); 

inline glm::vec2 coordToVec(const std::pair<int, int>& in_pair) {
    return glm::vec2{ in_pair.first, in_pair.second };
}

namespace glm {
    // Add a translate for 2x2 vector
    template<typename T, qualifier Q>
    GLM_FUNC_QUALIFIER mat<2, 2, T, Q> translate(mat<2, 2, T, Q> const& m, vec<2, T, Q> const& v) {
        mat<2, 2, T, Q> result{ m };
        result[1] = m[0] * v[0] + m[1] * v[1];
        return result;
    }

    // Add a scale for 2x2 vector
    template<typename T, qualifier Q>
    GLM_FUNC_QUALIFIER mat<2, 2, T, Q> scale(mat<2, 2, T, Q> const& m, vec<2, T, Q> const& v) {
        mat<2, 2, T, Q> result{ m };
        result[0] = m[0] * v[0];
        result[1] = m[1] * v[1];
        return result;
    }
}

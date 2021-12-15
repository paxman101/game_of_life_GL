#pragma once

#include <string>
#include <glm/glm.hpp>

char* readShader(const std::string& filename); 

inline glm::vec2 coordToVec(const std::pair<int, int>& in_pair) {
    return glm::vec2{ in_pair.first, in_pair.second };
}


namespace glm {
    template<int size, typename T, qualifier Q>
    std::ostream& operator<<(std::ostream& out, glm::vec<size, T, Q> const& v) {
        for (int i = 0; i < size; i++) {
            if (i != 0) out << " ";
            out << v[i];
        }
        return out;
    }

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

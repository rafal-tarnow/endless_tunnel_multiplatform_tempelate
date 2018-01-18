TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

LIBS += -lGLESv2
LIBS += -lglfw

DEFINES += GLM_FORCE_SIZE_T_LENGTH
DEFINES += GLM_FORCE_RADIANS

INCLUDEPATH += . ../app/src/main/cpp/SystemAbstraction/Game/


SOURCES += main.cpp \
    ../app/src/main/cpp/SystemAbstraction/Game/glm/detail/glm.cpp \
    ../app/src/main/cpp/SystemAbstraction/Game/anim.cpp \
    ../app/src/main/cpp/SystemAbstraction/Game/ascii_to_geom.cpp \
    ../app/src/main/cpp/SystemAbstraction/Game/dialog_scene.cpp \
    ../app/src/main/cpp/SystemAbstraction/Game/indexbuf.cpp \
    ../app/src/main/cpp/SystemAbstraction/Game/obstacle.cpp \
    ../app/src/main/cpp/SystemAbstraction/Game/obstacle_generator.cpp \
    ../app/src/main/cpp/SystemAbstraction/Game/our_shader.cpp \
    ../app/src/main/cpp/SystemAbstraction/Game/play_scene.cpp \
    ../app/src/main/cpp/SystemAbstraction/Game/scene.cpp \
    ../app/src/main/cpp/SystemAbstraction/Game/scene_manager.cpp \
    ../app/src/main/cpp/SystemAbstraction/Game/shader.cpp \
    ../app/src/main/cpp/SystemAbstraction/Game/shape_renderer.cpp \
    ../app/src/main/cpp/SystemAbstraction/Game/tex_quad.cpp \
    ../app/src/main/cpp/SystemAbstraction/Game/text_renderer.cpp \
    ../app/src/main/cpp/SystemAbstraction/Game/texture.cpp \
    ../app/src/main/cpp/SystemAbstraction/Game/ui_scene.cpp \
    ../app/src/main/cpp/SystemAbstraction/Game/util.cpp \
    ../app/src/main/cpp/SystemAbstraction/Game/vertexbuf.cpp \
    ../app/src/main/cpp/SystemAbstraction/Game/welcome_scene.cpp \
    ../app/src/main/cpp/SystemAbstraction/system_abstraction.cpp \
    ../app/src/main/cpp/SystemAbstraction/system_audio.cpp \
    ../app/src/main/cpp/SystemAbstraction/Game/data/alphabet.inl \
    ../app/src/main/cpp/SystemAbstraction/Game/data/ascii_art.inl \
    ../app/src/main/cpp/SystemAbstraction/Game/data/blurb.inl \
    ../app/src/main/cpp/SystemAbstraction/Game/data/cube_geom.inl \
    ../app/src/main/cpp/SystemAbstraction/Game/data/our_shader.inl \
    ../app/src/main/cpp/SystemAbstraction/Game/data/strings.inl \
    ../app/src/main/cpp/SystemAbstraction/Game/data/tunnel_geom.inl

HEADERS += \
    opengl_includes.hpp \
    ../app/src/main/cpp/SystemAbstraction/Game/glm/detail/_features.hpp \
    ../app/src/main/cpp/SystemAbstraction/Game/glm/detail/_fixes.hpp \
    ../app/src/main/cpp/SystemAbstraction/Game/glm/detail/_literals.hpp \
    ../app/src/main/cpp/SystemAbstraction/Game/glm/detail/_noise.hpp \
    ../app/src/main/cpp/SystemAbstraction/Game/glm/detail/_swizzle.hpp \
    ../app/src/main/cpp/SystemAbstraction/Game/glm/detail/_swizzle_func.hpp \
    ../app/src/main/cpp/SystemAbstraction/Game/glm/detail/_vectorize.hpp \
    ../app/src/main/cpp/SystemAbstraction/Game/glm/detail/func_common.hpp \
    ../app/src/main/cpp/SystemAbstraction/Game/glm/detail/func_exponential.hpp \
    ../app/src/main/cpp/SystemAbstraction/Game/glm/detail/func_geometric.hpp \
    ../app/src/main/cpp/SystemAbstraction/Game/glm/detail/func_integer.hpp \
    ../app/src/main/cpp/SystemAbstraction/Game/glm/detail/func_matrix.hpp \
    ../app/src/main/cpp/SystemAbstraction/Game/glm/detail/func_noise.hpp \
    ../app/src/main/cpp/SystemAbstraction/Game/glm/detail/func_packing.hpp \
    ../app/src/main/cpp/SystemAbstraction/Game/glm/detail/func_trigonometric.hpp \
    ../app/src/main/cpp/SystemAbstraction/Game/glm/detail/func_vector_relational.hpp \
    ../app/src/main/cpp/SystemAbstraction/Game/glm/detail/hint.hpp \
    ../app/src/main/cpp/SystemAbstraction/Game/glm/detail/intrinsic_common.hpp \
    ../app/src/main/cpp/SystemAbstraction/Game/glm/detail/intrinsic_exponential.hpp \
    ../app/src/main/cpp/SystemAbstraction/Game/glm/detail/intrinsic_geometric.hpp \
    ../app/src/main/cpp/SystemAbstraction/Game/glm/detail/intrinsic_integer.hpp \
    ../app/src/main/cpp/SystemAbstraction/Game/glm/detail/intrinsic_matrix.hpp \
    ../app/src/main/cpp/SystemAbstraction/Game/glm/detail/intrinsic_trigonometric.hpp \
    ../app/src/main/cpp/SystemAbstraction/Game/glm/detail/intrinsic_vector_relational.hpp \
    ../app/src/main/cpp/SystemAbstraction/Game/glm/detail/precision.hpp \
    ../app/src/main/cpp/SystemAbstraction/Game/glm/detail/setup.hpp \
    ../app/src/main/cpp/SystemAbstraction/Game/glm/detail/type_float.hpp \
    ../app/src/main/cpp/SystemAbstraction/Game/glm/detail/type_gentype.hpp \
    ../app/src/main/cpp/SystemAbstraction/Game/glm/detail/type_half.hpp \
    ../app/src/main/cpp/SystemAbstraction/Game/glm/detail/type_int.hpp \
    ../app/src/main/cpp/SystemAbstraction/Game/glm/detail/type_mat.hpp \
    ../app/src/main/cpp/SystemAbstraction/Game/glm/detail/type_mat2x2.hpp \
    ../app/src/main/cpp/SystemAbstraction/Game/glm/detail/type_mat2x3.hpp \
    ../app/src/main/cpp/SystemAbstraction/Game/glm/detail/type_mat2x4.hpp \
    ../app/src/main/cpp/SystemAbstraction/Game/glm/detail/type_mat3x2.hpp \
    ../app/src/main/cpp/SystemAbstraction/Game/glm/detail/type_mat3x3.hpp \
    ../app/src/main/cpp/SystemAbstraction/Game/glm/detail/type_mat3x4.hpp \
    ../app/src/main/cpp/SystemAbstraction/Game/glm/detail/type_mat4x2.hpp \
    ../app/src/main/cpp/SystemAbstraction/Game/glm/detail/type_mat4x3.hpp \
    ../app/src/main/cpp/SystemAbstraction/Game/glm/detail/type_mat4x4.hpp \
    ../app/src/main/cpp/SystemAbstraction/Game/glm/detail/type_vec.hpp \
    ../app/src/main/cpp/SystemAbstraction/Game/glm/detail/type_vec1.hpp \
    ../app/src/main/cpp/SystemAbstraction/Game/glm/detail/type_vec2.hpp \
    ../app/src/main/cpp/SystemAbstraction/Game/glm/detail/type_vec3.hpp \
    ../app/src/main/cpp/SystemAbstraction/Game/glm/detail/type_vec4.hpp \
    ../app/src/main/cpp/SystemAbstraction/Game/glm/gtc/constants.hpp \
    ../app/src/main/cpp/SystemAbstraction/Game/glm/gtc/epsilon.hpp \
    ../app/src/main/cpp/SystemAbstraction/Game/glm/gtc/matrix_access.hpp \
    ../app/src/main/cpp/SystemAbstraction/Game/glm/gtc/matrix_integer.hpp \
    ../app/src/main/cpp/SystemAbstraction/Game/glm/gtc/matrix_inverse.hpp \
    ../app/src/main/cpp/SystemAbstraction/Game/glm/gtc/matrix_transform.hpp \
    ../app/src/main/cpp/SystemAbstraction/Game/glm/gtc/noise.hpp \
    ../app/src/main/cpp/SystemAbstraction/Game/glm/gtc/packing.hpp \
    ../app/src/main/cpp/SystemAbstraction/Game/glm/gtc/quaternion.hpp \
    ../app/src/main/cpp/SystemAbstraction/Game/glm/gtc/random.hpp \
    ../app/src/main/cpp/SystemAbstraction/Game/glm/gtc/reciprocal.hpp \
    ../app/src/main/cpp/SystemAbstraction/Game/glm/gtc/type_precision.hpp \
    ../app/src/main/cpp/SystemAbstraction/Game/glm/gtc/type_ptr.hpp \
    ../app/src/main/cpp/SystemAbstraction/Game/glm/gtc/ulp.hpp \
    ../app/src/main/cpp/SystemAbstraction/Game/glm/gtx/associated_min_max.hpp \
    ../app/src/main/cpp/SystemAbstraction/Game/glm/gtx/bit.hpp \
    ../app/src/main/cpp/SystemAbstraction/Game/glm/gtx/closest_point.hpp \
    ../app/src/main/cpp/SystemAbstraction/Game/glm/gtx/color_space.hpp \
    ../app/src/main/cpp/SystemAbstraction/Game/glm/gtx/color_space_YCoCg.hpp \
    ../app/src/main/cpp/SystemAbstraction/Game/glm/gtx/compatibility.hpp \
    ../app/src/main/cpp/SystemAbstraction/Game/glm/gtx/component_wise.hpp \
    ../app/src/main/cpp/SystemAbstraction/Game/glm/gtx/constants.hpp \
    ../app/src/main/cpp/SystemAbstraction/Game/glm/gtx/dual_quaternion.hpp \
    ../app/src/main/cpp/SystemAbstraction/Game/glm/gtx/epsilon.hpp \
    ../app/src/main/cpp/SystemAbstraction/Game/glm/gtx/euler_angles.hpp \
    ../app/src/main/cpp/SystemAbstraction/Game/glm/gtx/extend.hpp \
    ../app/src/main/cpp/SystemAbstraction/Game/glm/gtx/extented_min_max.hpp \
    ../app/src/main/cpp/SystemAbstraction/Game/glm/gtx/fast_exponential.hpp \
    ../app/src/main/cpp/SystemAbstraction/Game/glm/gtx/fast_square_root.hpp \
    ../app/src/main/cpp/SystemAbstraction/Game/glm/gtx/fast_trigonometry.hpp \
    ../app/src/main/cpp/SystemAbstraction/Game/glm/gtx/gradient_paint.hpp \
    ../app/src/main/cpp/SystemAbstraction/Game/glm/gtx/handed_coordinate_space.hpp \
    ../app/src/main/cpp/SystemAbstraction/Game/glm/gtx/inertia.hpp \
    ../app/src/main/cpp/SystemAbstraction/Game/glm/gtx/int_10_10_10_2.hpp \
    ../app/src/main/cpp/SystemAbstraction/Game/glm/gtx/integer.hpp \
    ../app/src/main/cpp/SystemAbstraction/Game/glm/gtx/intersect.hpp \
    ../app/src/main/cpp/SystemAbstraction/Game/glm/gtx/io.hpp \
    ../app/src/main/cpp/SystemAbstraction/Game/glm/gtx/log_base.hpp \
    ../app/src/main/cpp/SystemAbstraction/Game/glm/gtx/matrix_cross_product.hpp \
    ../app/src/main/cpp/SystemAbstraction/Game/glm/gtx/matrix_interpolation.hpp \
    ../app/src/main/cpp/SystemAbstraction/Game/glm/gtx/matrix_major_storage.hpp \
    ../app/src/main/cpp/SystemAbstraction/Game/glm/gtx/matrix_operation.hpp \
    ../app/src/main/cpp/SystemAbstraction/Game/glm/gtx/matrix_query.hpp \
    ../app/src/main/cpp/SystemAbstraction/Game/glm/gtx/matrix_transform_2d.hpp \
    ../app/src/main/cpp/SystemAbstraction/Game/glm/gtx/mixed_product.hpp \
    ../app/src/main/cpp/SystemAbstraction/Game/glm/gtx/multiple.hpp \
    ../app/src/main/cpp/SystemAbstraction/Game/glm/gtx/noise.hpp \
    ../app/src/main/cpp/SystemAbstraction/Game/glm/gtx/norm.hpp \
    ../app/src/main/cpp/SystemAbstraction/Game/glm/gtx/normal.hpp \
    ../app/src/main/cpp/SystemAbstraction/Game/glm/gtx/normalize_dot.hpp \
    ../app/src/main/cpp/SystemAbstraction/Game/glm/gtx/number_precision.hpp \
    ../app/src/main/cpp/SystemAbstraction/Game/glm/gtx/optimum_pow.hpp \
    ../app/src/main/cpp/SystemAbstraction/Game/glm/gtx/orthonormalize.hpp \
    ../app/src/main/cpp/SystemAbstraction/Game/glm/gtx/perpendicular.hpp \
    ../app/src/main/cpp/SystemAbstraction/Game/glm/gtx/polar_coordinates.hpp \
    ../app/src/main/cpp/SystemAbstraction/Game/glm/gtx/projection.hpp \
    ../app/src/main/cpp/SystemAbstraction/Game/glm/gtx/quaternion.hpp \
    ../app/src/main/cpp/SystemAbstraction/Game/glm/gtx/random.hpp \
    ../app/src/main/cpp/SystemAbstraction/Game/glm/gtx/raw_data.hpp \
    ../app/src/main/cpp/SystemAbstraction/Game/glm/gtx/reciprocal.hpp \
    ../app/src/main/cpp/SystemAbstraction/Game/glm/gtx/rotate_normalized_axis.hpp \
    ../app/src/main/cpp/SystemAbstraction/Game/glm/gtx/rotate_vector.hpp \
    ../app/src/main/cpp/SystemAbstraction/Game/glm/gtx/scalar_relational.hpp \
    ../app/src/main/cpp/SystemAbstraction/Game/glm/gtx/simd_mat4.hpp \
    ../app/src/main/cpp/SystemAbstraction/Game/glm/gtx/simd_quat.hpp \
    ../app/src/main/cpp/SystemAbstraction/Game/glm/gtx/simd_vec4.hpp \
    ../app/src/main/cpp/SystemAbstraction/Game/glm/gtx/spline.hpp \
    ../app/src/main/cpp/SystemAbstraction/Game/glm/gtx/std_based_type.hpp \
    ../app/src/main/cpp/SystemAbstraction/Game/glm/gtx/string_cast.hpp \
    ../app/src/main/cpp/SystemAbstraction/Game/glm/gtx/transform.hpp \
    ../app/src/main/cpp/SystemAbstraction/Game/glm/gtx/transform2.hpp \
    ../app/src/main/cpp/SystemAbstraction/Game/glm/gtx/ulp.hpp \
    ../app/src/main/cpp/SystemAbstraction/Game/glm/gtx/unsigned_int.hpp \
    ../app/src/main/cpp/SystemAbstraction/Game/glm/gtx/vec1.hpp \
    ../app/src/main/cpp/SystemAbstraction/Game/glm/gtx/vector_angle.hpp \
    ../app/src/main/cpp/SystemAbstraction/Game/glm/gtx/vector_query.hpp \
    ../app/src/main/cpp/SystemAbstraction/Game/glm/gtx/wrap.hpp \
    ../app/src/main/cpp/SystemAbstraction/Game/glm/virtrev/xstream.hpp \
    ../app/src/main/cpp/SystemAbstraction/Game/glm/common.hpp \
    ../app/src/main/cpp/SystemAbstraction/Game/glm/exponential.hpp \
    ../app/src/main/cpp/SystemAbstraction/Game/glm/ext.hpp \
    ../app/src/main/cpp/SystemAbstraction/Game/glm/fwd.hpp \
    ../app/src/main/cpp/SystemAbstraction/Game/glm/geometric.hpp \
    ../app/src/main/cpp/SystemAbstraction/Game/glm/glm.hpp \
    ../app/src/main/cpp/SystemAbstraction/Game/glm/integer.hpp \
    ../app/src/main/cpp/SystemAbstraction/Game/glm/mat2x2.hpp \
    ../app/src/main/cpp/SystemAbstraction/Game/glm/mat2x3.hpp \
    ../app/src/main/cpp/SystemAbstraction/Game/glm/mat2x4.hpp \
    ../app/src/main/cpp/SystemAbstraction/Game/glm/mat3x2.hpp \
    ../app/src/main/cpp/SystemAbstraction/Game/glm/mat3x3.hpp \
    ../app/src/main/cpp/SystemAbstraction/Game/glm/mat3x4.hpp \
    ../app/src/main/cpp/SystemAbstraction/Game/glm/mat4x2.hpp \
    ../app/src/main/cpp/SystemAbstraction/Game/glm/mat4x3.hpp \
    ../app/src/main/cpp/SystemAbstraction/Game/glm/mat4x4.hpp \
    ../app/src/main/cpp/SystemAbstraction/Game/glm/matrix.hpp \
    ../app/src/main/cpp/SystemAbstraction/Game/glm/packing.hpp \
    ../app/src/main/cpp/SystemAbstraction/Game/glm/trigonometric.hpp \
    ../app/src/main/cpp/SystemAbstraction/Game/glm/vec2.hpp \
    ../app/src/main/cpp/SystemAbstraction/Game/glm/vec3.hpp \
    ../app/src/main/cpp/SystemAbstraction/Game/glm/vec4.hpp \
    ../app/src/main/cpp/SystemAbstraction/Game/glm/vector_relational.hpp \
    ../app/src/main/cpp/SystemAbstraction/Game/anim.hpp \
    ../app/src/main/cpp/SystemAbstraction/Game/ascii_to_geom.hpp \
    ../app/src/main/cpp/SystemAbstraction/Game/common.hpp \
    ../app/src/main/cpp/SystemAbstraction/Game/dialog_scene.hpp \
    ../app/src/main/cpp/SystemAbstraction/Game/engine.hpp \
    ../app/src/main/cpp/SystemAbstraction/Game/game_consts.hpp \
    ../app/src/main/cpp/SystemAbstraction/Game/indexbuf.hpp \
    ../app/src/main/cpp/SystemAbstraction/Game/joystick-support.hpp \
    ../app/src/main/cpp/SystemAbstraction/Game/obstacle.hpp \
    ../app/src/main/cpp/SystemAbstraction/Game/obstacle_generator.hpp \
    ../app/src/main/cpp/SystemAbstraction/Game/our_key_codes.hpp \
    ../app/src/main/cpp/SystemAbstraction/Game/our_shader.hpp \
    ../app/src/main/cpp/SystemAbstraction/Game/play_scene.hpp \
    ../app/src/main/cpp/SystemAbstraction/Game/scene.hpp \
    ../app/src/main/cpp/SystemAbstraction/Game/scene_manager.hpp \
    ../app/src/main/cpp/SystemAbstraction/Game/shader.hpp \
    ../app/src/main/cpp/SystemAbstraction/Game/shape_renderer.hpp \
    ../app/src/main/cpp/SystemAbstraction/Game/simplegeom.hpp \
    ../app/src/main/cpp/SystemAbstraction/Game/tex_quad.hpp \
    ../app/src/main/cpp/SystemAbstraction/Game/text_renderer.hpp \
    ../app/src/main/cpp/SystemAbstraction/Game/texture.hpp \
    ../app/src/main/cpp/SystemAbstraction/Game/ui_scene.hpp \
    ../app/src/main/cpp/SystemAbstraction/Game/util.hpp \
    ../app/src/main/cpp/SystemAbstraction/Game/vertexbuf.hpp \
    ../app/src/main/cpp/SystemAbstraction/Game/welcome_scene.hpp \
    ../app/src/main/cpp/SystemAbstraction/system_abstraction.hpp \
    ../app/src/main/cpp/SystemAbstraction/system_audio.hpp \
    ../app/src/main/cpp/SystemAbstraction/system_log.hpp

DISTFILES += \
    ../app/src/main/cpp/SystemAbstraction/Game/glm/CMakeLists.txt


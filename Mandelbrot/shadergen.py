import os

directory = r'assets/shaders/'

local_sizes_xy = [
    (r'64' , r'local_size_x = 8, local_size_y = 8'), 
    (r'32' , r'local_size_x = 8, local_size_y = 4'), 
    (r'16' , r'local_size_x = 4, local_size_y = 4'),
    (r'8',   r'local_size_x = 4, local_size_y = 2')
]

colors = [
    (r'hue', """float t = float(i) / float(maxIter);
    \t    float hue = 0.95 + 10.0 * t;
    \t    vec3 rgb = vec3(0.5 + 0.5 * cos(6.28318 * (hue + vec3(0.0, 0.33, 0.67))));
    \t    color = vec4(rgb, t);"""), 
    (r'grayscale',"""float t = float(i) / float(maxIter);
    \t    color = vec4(t, t, t, t);"""),
    (r'scary',"""float t = float(i) / float(maxIter);
    \t    color = vec4(t, t*t, t*t*t, t);"""),
    (r'fourth',"""float t = float(i) / float(maxIter);
    \t    color = vec4(0.0, t, t, t);""")
]

precision_levels = [
(r'fast', r"""#version 430 core

layout ($local_size_xy$, local_size_z = 1) in;

layout(rgba32f, binding = 0) uniform image2D imgOutput;

highp uniform float scaleFactor;
highp uniform vec2 translation;
highp uniform vec2 invRes;
highp uniform float aspectRatio;
uniform uint maxIter;

void main() {
    const ivec2 size = imageSize(imgOutput);
    vec4 color = vec4(0.0, 0.0, 0.0, 1.0);
    ivec2 texelCoord = ivec2(gl_GlobalInvocationID.xy);

    highp vec2 c;
    c.x = float(texelCoord.x * invRes.x + aspectRatio) * scaleFactor + translation.x;
    c.y = float(texelCoord.y * invRes.y - 1) * scaleFactor + translation.y;
     
    highp vec2 z = vec2(0.0, 0.0);
    #pragma unroll
    for (int i = 0; i < maxIter; ++i) {
        highp float real = z.x * z.x - z.y * z.y + c.x;
        highp float imag = 2.0 * z.x * z.y + c.y;

        z = vec2(real, imag);
        if (z.x * z.x + z.y * z.y > 4.0) {
            $color$
            break;
        } 
    }
	
    imageStore(imgOutput, texelCoord, color);
}
"""),

(r'precise', r"""#version 430 core
#extension GL_ARB_gpu_shader_fp64 : require

layout ($local_size_xy$, local_size_z = 1) in;

layout(rgba32f, binding = 0) uniform image2D imgOutput;

uniform double scaleFactor;
uniform dvec2 translation;
uniform dvec2 invRes;
uniform float aspectRatio;
uniform uint maxIter;

void main() {
    const ivec2 size = imageSize(imgOutput);
    vec4 color = vec4(0.0, 0.0, 0.0, 1.0);
    ivec2 texelCoord = ivec2(gl_GlobalInvocationID.xy);

    dvec2 c;
    c.x = double(texelCoord.x * invRes.x + aspectRatio) * scaleFactor + translation.x;
    c.y = double(texelCoord.y * invRes.y - 1) * scaleFactor + translation.y;
     
    dvec2 z = dvec2(0.0, 0.0);
    #pragma unroll
    for (int i = 0; i < maxIter; ++i) {
        double real = z.x * z.x - z.y * z.y + c.x;
        double imag = 2.0 * z.x * z.y + c.y;

        z = dvec2(real, imag);
        if (z.x * z.x + z.y * z.y > 4.0) {
            $color$
            break;
        } 
    }
	
    imageStore(imgOutput, texelCoord, color);
}
""")
]

for precision_level in precision_levels:
    for local_size in local_sizes_xy:
        for color in colors:
            generated_shader = precision_level[1]
            generated_shader = generated_shader.replace('$local_size_xy$', local_size[1])
            generated_shader = generated_shader.replace('$color$', color[1])
            filename = 'mandelbrot_' + precision_level[0] + '_' + color[0] + '_' + local_size[0] + '.comp'
            with open(directory + filename, 'w') as file:
                file.write(generated_shader)
